import gc
import time

t = time.time()
worse_case = 0.0

def gc_callback(phase, info):
    global t, worse_case
    if phase == 'start':
        t = time.time()
    elif phase == 'stop':
        fi = time.time() - t
        if fi > 0.01:  # Only print if GC took more than 10ms
            print(f"GC took {fi * 1000:.4f} milliseconds, worse case: {worse_case * 1000:.4f} milliseconds")
            if fi > worse_case:
                worse_case = fi


gc.callbacks.append(gc_callback)

class Node:
    def __init__(self, value):
        self.value = value
        self.next = None
    def add_next(self, node):
        self.next = node

def stress_no_cycles(limit=50_000_000):
    head = Node(0)
    current = head
    for i in range(1, limit):
        new_node = Node(i)
        current.add_next(new_node)
        current = new_node



if __name__ == "__main__":
    for i in range(100):
        start = time.time()
        stress_no_cycles()
        print(f"Total time: {time.time() - start:.4f} seconds")