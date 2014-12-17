public class BasicSummer {
    protected Array array;
    
    public BasicSummer(Array backend) {
        array = backend;
    }
     

    public int compute() {
        for(int k = 0; k < array.size(); ++k) 
          array.set(k,k);
        int sum = 0;
        for(int k = 0; k < array.size(); ++k) 
          sum += array.get(k);
        return sum;
    }

}