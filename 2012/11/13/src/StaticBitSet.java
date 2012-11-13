

import java.util.Arrays;

public class StaticBitSet {
  long[] data;

  public StaticBitSet(int sizeinbits) {
    data = new long[(sizeinbits + 63) / 64];
  }
  
  void clear() {
    Arrays.fill(data, 0);
  }
  
  public void resize(int sizeinbits) {
    
    data = Arrays.copyOf(data, (sizeinbits + 63) / 64);
  }
  
  public int cardinality() {
    int sum = 0;
    for(long l : data)
      sum += Long.bitCount(l);
    return sum;
  }

  public boolean get(int i) {
    return (data[i / 64] & (1l << (i % 64))) !=0;
  }
  public void set(int i) {
    data[i / 64] |= (1l << (i % 64));
  }

  public void unset(int i) {
    data[i / 64] &= ~(1l << (i % 64));
  }
  
  public void set(int i, boolean b) {
    if(b) set(i); else unset(i);
  }

  // for(int i=bs.nextSetBit(0); i>=0; i=bs.nextSetBit(i+1)) { // operate on
  // index i here }
  public int nextSetBit(int i) {
    int x = i / 64;
    long w = data[x];
    w >>>= (i % 64);
    if (w != 0) {
      return i + Long.numberOfTrailingZeros(w);
    }
    ++x;
    for (; x < data.length; ++x) {
      if (data[x] != 0) {
        return x * 64 + Long.numberOfTrailingZeros(data[x]);
      }
    }
    return -1;
  }

}
