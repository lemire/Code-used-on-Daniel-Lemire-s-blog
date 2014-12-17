public class ReverseFastSummer {
    
    int[] array;
    public ReverseFastSummer(int cap) {
        array = new int[cap];
    }
    
    public int compute() {
        for(int k = array.length - 1 ; k >=0; --k) 
          array[k] = k;
        int sum = 0;
        for(int k = array.length - 1 ; k >=0; --k) 
          sum += array[k];
        return sum;
    }
}
