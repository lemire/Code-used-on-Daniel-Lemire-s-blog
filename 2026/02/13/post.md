# How bad can Python stop-the-world pauses get?

When programming, we need to allocate memory, and then deallocate it. If you program in C, you get used to malloc/free functions. Sadly, this leaves you vulnerable to memory leaks: unrecovered memory. Most popular programming languages today use automated memory management: Java, JavaScript, Python, C#, Go, Swift and so forth.

There are essentially two types of automated memory managements. The simplest method is reference counting. You track how many references there are to each object. When an object has no more references, then we can free the memory associated with it. Swift and Python use reference counting. The downside of reference counting are circular references. You may have your main program reference object A, then you add object B which references object A, and you make it so that object A also reference object B. Thus object B has one reference while object A has two references. If your main program drops its reference to object A, the both objects A and B still have a reference count of one. Yet they should be freed. To solve this problem, you could just visit all of your objects to detect which are unreachable, including A and B. However, it takes time to do so. Thus, the other popular approach of automated memory management: generational garbage collection. You use the fact that most memory gets released soon after allocation. Thus you track young objects and visit them from time to time. Then, more rarely, you do a full scan. The downside of generational garbage collection is that typical implementations stop the world to scan the memory. In many instances, your entire program is stopped. There are many variations on the implementation, with decades of research.

The common Python implementation has both types: reference counting and generational garbage collection. The generational garbage collection component can trigger pauses. A lot of servers are written in Python. It means that your service might just become unavailable for a time. We often call them 'stop the world' pauses. How long can this pause get?

To test this out, I wrote a Python function to create a classical linked list:

```python
class Node:
    def __init__(self, value):
        self.value = value
        self.next = None
    def add_next(self, node):
        self.next = node

def create_linked_list(limit):
    """ create a linked list of length 'limit' """
    head = Node(0)
    current = head
    for i in range(1, limit):
        new_node = Node(i)
        current.add_next(new_node)
        current = new_node
    return head
```

And then I create one large linked list and then, in a tight loop, we create small linked lists that are immediately discarded.


```python
x = create_linked_list(50_000_000)
for i in range(1000000):
    create_linked_list(1000)
```

A key characteristic of my code is the 50 million linked list. It does not get released until the end of the program, but the garbage collector may still examine it. 

And I record the maximum delay between two iterations in the loop (using `time.time()`). 

How bad can it get? The answer depends on the Python version. And it is not consistent from
run-to-run. So I ran it once and picked whatever result I got. I express the delay in milliseconds.


| python version |  system  |  max delay |
|----------------|--------------|------------|
| 3.14           |   macOS (Apple M4)      | 320 ms |
| 3.12           |   Linux (Intel Ice Lake)     | 2,200 ms |


Almost all of this delay (say 320 ms) is due to the garbage collection. Creating a linked list with 1000 elements takes less than a millisecond.

How long is 320 ms? It is a third of a second, so it is long enough for human beings to notice it. For reference, a video game drawing the screen 60 times per second has less than 17 ms to draw the screen. The 2,200 ms delay could look like a server crash from the point of view of a user, and might definitely trigger a time-out (failed request). 

I ported the Python program to Go. It is the same algorithm, but a direct comparison is likely unfair. Still, it gives us a reference.


| go version |  system  |  max delay |
|----------------|--------------|------------|
| 1.25           |   macOS (Apple M4)      | 50 ms |
| 1.25           |   Linux (Intel Ice Lake)     | 33 ms |

Thus Go has pauses that are several times shorter than Python, and there is no catastrophic 2-second pause.

Should these pauses be a concern? Most Python programs do not create so many objects in memory at the same time. Thus you are not likely to see these long pauses if you have a simple web app or a script. Python gives you a few options, such as `gc.set_threshold` and `gc.freeze` which could help you tune the behaviour.