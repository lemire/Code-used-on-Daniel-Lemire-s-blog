// c:/bsd/rigel/sort/Hsort
// Date: Fri Jan 31 13:22:27 2014
// (C) OntoOO/ Dennis de Champeaux


#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

void heapSort(void **, int, int (*)(const void*, const void*));
static void heapify(void **, int, int (*)(const void*, const void*));
static void siftDown(void **, int, int, int (*)(const void*, const void*));


void heapc(void **A, int lo, int hi, int (*compare)(const void*, const void*)) {
  // printf("heapc: %d %d\n", lo, hi);
  if ( hi <= lo ) return;
  heapSort(&A[lo], hi-lo+1, compare);
} // end heapc

void heapSort(void **a, int count, int (*compare)(const void*, const void*)) {
  // input:  an unordered array a of length count
  // first place a in max-heap order
  heapify(a, count, compare);
  // in languages with zero-based arrays the children are 2*i+1 and 2*i+2
  int end = count-1; 
  while ( end > 0 ) {
    // (swap the root(maximum value) of the heap with 
    // the last element of the heap)
    // swap(a[end], a[0]);
    iswap(end, 0, a);
    // (decrease the size of the heap by one so that the 
    // previous max value will stay in its proper placement) 
    end = end - 1;
    // (put the heap back in max-heap order)
    siftDown(a, 0, end, compare);
  }
} // end heapSort
         
void heapify(void **a, int count,
                    int (*compare)(const void*, const void*)) {
  // (start is assigned the index in a of the last parent node)
  int start = (count - 2) / 2;
  while ( 0 <= start ) {
    // (sift down the node at index start to the proper place such 
    // that all nodes below the start index are in heap order)
    siftDown(a, start, count-1, compare);
    start = start - 1;
  } // (after sifting down the root all nodes/elements are in heap order)
} // end heapify

void siftDown(void **a, int start, int end,
                     int (*compare)(const void*, const void*)) {
  // input:  end represents the limit of how far down the heap to sift.
  int root = start;
  int child, swapi;
  // (While the root has at least one child)
  while ( root * 2 + 1 <= end ) {
    child = root * 2 + 1; // (root*2 + 1 points to the left child)
    swapi = root; // (keeps track of child to swap with)
    // (check if root is smaller than left child)
    // if ( a[swapi] < a[child] ) 
    if ( compare(a[swapi], a[child]) < 0 ) 
      swapi = child;
    // (check if right child exists, and if it's bigger 
    // than what we're currently swapping with)
    // if ( child+1 <= end && a[swapi] < a[child+1] )
    if ( child+1 <= end && compare(a[swapi],a[child+1]) < 0 )
      swapi = child + 1;
    // (check if we need to swap at all)
    if ( swapi != root ) {
      // swap(a[root], a[swapi]);
      iswap(root, swapi, a);
      root = swapi; // (repeat to continue sifting down the child now)
    } else return;
  }
} // end siftDown

