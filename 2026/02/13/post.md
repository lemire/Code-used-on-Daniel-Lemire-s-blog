# How bad can Python stop-the-wolrd pauses get?

When programming, we need to allocate memory, and then deallocate it. If you program in C, you get used to malloc/free functions. Sadly, this leaves you vulnerable to memory leaks: unrecovered memory. Most popular programming languages today use automated memory management: Java, JavaScript, Python, C#, Go, Swift and so forth.

There are essentially two types of automated memory managements. The simplest method is reference counting. You track how many references there are to each object. When an object has no more references, then we can free the memory associated with it. Swift and Python use reference counting. The downside of reference counting are circular references. You may have you main problem reference object A, then you add object B which references object A, and you make it so that object A also reference object B. Thus object B has one reference while object A has two references. If you main problem drops its reference to object A, the both objects A and B still have a reference count of one. Yet they should be freed. To solve this problem, you could just visit all of your objects to detect which are unreachable, including A and B. However, it takes time to do so. Thus, the other popular approach of automated memory management: generational garbage collection. You use the fact that most memory gets released soon after allocation. Thus you track young objects and visit them from time to time. Then, more rarely, you do a full scan. The downside of generational garbage collection is that typical implemenations stop the world to scan the memory. In many instances, your entire program is stopped. There are many variations on the implementation, with decades of research.

The common Python implementation has both types: reference counting and generational garbage collection. The generational garbage collection component can trigger pauses. A lot of servers are written in Python. It means that your service might just become unavailable for a time. How long is this pause?

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
    create_linked_list(100)
```

And record the maximum delay between two iterations in the loop (using `time.time()`).

How bad can it get? The answer depends on the Python version.