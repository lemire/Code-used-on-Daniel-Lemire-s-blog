public class FastSummer {
    
    int[] array;
    public FastSummer(int cap) {
        array = new int[cap];
    }
    
    public int compute() {
        for(int k = 0; k < array.length; ++k) 
          array[k] = k;
        int sum = 0;
        for(int k = 0; k < array.length; ++k) 
          sum += array[k];
        return sum;
    }
}