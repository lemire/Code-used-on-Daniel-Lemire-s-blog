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


#gc.callbacks.append(gc_callback)

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
    return head



if __name__ == "__main__":
    x = stress_no_cycles(50_000_000)
    print("Initial batch completed, starting stress test...")
    start = time.time()
    worse_case = 0.0
    for i in range(1000000):
        batch_start = time.time()
        if batch_start - start > worse_case:
            print(f"Batch {i}: delay between batch start and overall start: {(batch_start - start) * 1000:.4f} milliseconds", flush=True)
            worse_case = batch_start - start
        start = time.time()
        stress_no_cycles(100)
    print(f"Max delay between batch start and overall start: {worse_case * 1000:.4f} milliseconds", flush=True)
