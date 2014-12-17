public final class FixedSummer {
    protected final NaiveArray array;
    
    public FixedSummer(int cap) {
        array = new NaiveArray(cap);
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