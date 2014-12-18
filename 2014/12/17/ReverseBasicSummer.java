public class ReverseBasicSummer {
    protected Array array;
    
    public ReverseBasicSummer(Array backend) {
        array = backend;
    }
     

    public int compute() {
        for(int k = array.size() - 1; k >= 0; --k) 
          array.set(k,k);
        int sum = 0;
        for(int k = array.size() - 1; k >= 0; --k) 
          sum += array.get(k);
        return sum;
    }

}
