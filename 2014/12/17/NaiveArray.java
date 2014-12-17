public final class NaiveArray implements Array {
    protected int[] array;
    
    public NaiveArray(int cap) {
        array = new int[cap];
    }
    
    public int get(int i) {
        return array[i];
    }
    
    public void set(int i, int x) {
        array[i] = x;  
    }
    
    public int size() {
        return array.length;
    }
}
