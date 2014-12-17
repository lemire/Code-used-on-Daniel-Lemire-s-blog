public class SmartSummer {
    protected Array array;
    
    public SmartSummer(Array backend) {
        array = backend;
    }
     

    public int compute() {
        if(array instanceof NaiveArray) {
            int[] back = ((NaiveArray) array).array;
            for(int k = 0; k < back.length; ++k) 
               back[k] = k;
            int sum = 0;
            for(int k = 0; k < back.length; ++k) 
               sum += back[k];
            return sum;
        }
        for(int k = 0; k < array.size(); ++k) 
          array.set(k,k);
        int sum = 0;
        for(int k = 0; k < array.size(); ++k) 
          sum += array.get(k);
        return sum;
    }

}