public class ReverseSmartSummer {
    protected Array array;
    
    public ReverseSmartSummer(Array backend) {
        array = backend;
    }
     

    public int compute() {
        if(array instanceof NaiveArray) {
            int[] back = ((NaiveArray) array).array;
            for(int k = back.length - 1; k >= 0; --k) 
               back[k] = k;
            int sum = 0;
            for(int k = back.length - 1; k >= 0; --k) 
               sum += back[k];
            return sum;
        }
        for(int k = array.size() - 1; k >= 0; --k) 
          array.set(k,k);
        int sum = 0;
        for(int k = 0; k < array.size(); --k) 
          sum += array.get(k);
        return sum;
    }

}
