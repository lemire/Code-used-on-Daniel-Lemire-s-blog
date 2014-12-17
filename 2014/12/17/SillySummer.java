public class SillySummer {
    protected Array array;
    
    public SillySummer(Array backend) {
        array = backend;
    }
     

    public int compute() {
        if(array instanceof NaiveArray) {
             for(int k = 0; k < array.size(); ++k) 
               array.set(k,k);
             int sum = 0;
             for(int k = 0; k < array.size(); ++k) 
               sum += array.get(k);
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
