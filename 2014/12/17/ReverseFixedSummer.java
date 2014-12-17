public final class ReverseFixedSummer {
    protected final NaiveArray array;
    
    public ReverseFixedSummer(int cap) {
        array = new NaiveArray(cap);
    }
     

    public int compute() {
        for(int k = array.size() - 1; k >=0 ; --k) 
          array.set(k,k);
        int sum = 0;
        for(int k = array.size() - 1; k >=0 ; --k) 
          sum += array.get(k);
        return sum;
    }

}
