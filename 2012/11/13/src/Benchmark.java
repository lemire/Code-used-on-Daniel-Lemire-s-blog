import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

import sparsebitmap.SparseBitmap;

import gnu.trove.iterator.TIntIterator;


public class Benchmark {
  
  // before you benchmark, make sure results are ok.
  public static void sanityCheck() {
    int numberofkeys = 1000;
    for (int offset = 10; offset < 1000; offset += 100) {
      final int N = offset * numberofkeys;
      ArrayList<Integer> answer = new ArrayList<Integer>();
      for (int i = 0; i < N; i += offset)
        answer.add(i);
      java.util.BitSet bs = new java.util.BitSet(N);
      ArrayList<Integer> answerbs = new ArrayList<Integer>();
      for (int i = 0; i < N; i += offset)
        bs.set(i);
      for (int i = bs.nextSetBit(0); i >= 0; i = bs.nextSetBit(i + 1))
        answerbs.add(i);
      if(!answer.equals(answerbs)) throw new InternalError("bug");
      StaticBitSet sbs = new StaticBitSet(N);
      ArrayList<Integer> answersbs = new ArrayList<Integer>();
      for (int i = 0; i < N; i += offset)
        sbs.set(i);
      for (int i = sbs.nextSetBit(0); i >= 0; i = sbs.nextSetBit(i + 1))
        answersbs.add(i);
      if(!answer.equals(answersbs)) throw new InternalError("bug");
      java.util.HashSet<Integer> h = new java.util.HashSet<Integer>();
      ArrayList<Integer> answerh = new ArrayList<Integer>();
      for (int i = 0; i < N; i += offset)
        h.add(i);
      for (int i : h)
        answerh.add(i);
      Collections.sort(answerh);
      if(!answer.equals(answerh)) throw new InternalError("bug");
      gnu.trove.set.TIntSet troveh = new gnu.trove.set.hash.TIntHashSet();
      ArrayList<Integer> answertroveh = new ArrayList<Integer>();
      for (int i = 0; i < N; i += offset)
        troveh.add(i);
      TIntIterator j = troveh.iterator();
      while (j.hasNext())
        answertroveh.add(j.next());
      Collections.sort(answertroveh);
      if(!answer.equals(answertroveh)) throw new InternalError("bug");
      boolean[] b = new boolean[N];
      ArrayList<Integer> answerb = new ArrayList<Integer>();
      for (int i = 0; i < N; i += offset)
        b[i] = true;
      for (int i = 0; i < b.length; ++i)
        if (b[i])
          answerb.add(i);
      if(!answer.equals(answerb)) throw new InternalError("bug");
    }

  }
  
  public static void benchmark(final boolean fastclear) {
    System.out.println("### fastclear = "+fastclear);
    int sum = 0;
    DecimalFormat df = new DecimalFormat("0.###");
    int numberofkeys = 1000;
    for (int offset = 10; offset <= 10000; offset *= 10) {
      String line = 1.0/offset+"\t";
      final int N = offset * numberofkeys;
      System.out.println("#=== N = " + N + " sparsity = "+ 1.0/offset);
      int repeat = 1000;
      long bef, aft;
      java.util.BitSet bs = new java.util.BitSet(N);
      bef = System.currentTimeMillis();
      for (int r = 0; r < repeat; ++r) {
        for (int i = 0; i < N; i += offset)
          bs.set(i);
        for (int i = bs.nextSetBit(0); i >= 0; i = bs.nextSetBit(i + 1)) {
          sum += i;
        }
        if(fastclear) {
          bs.clear();
        } else {
          for (int i = 0; i < N; i += offset)
            bs.set(i, false);
        }
      }
      aft = System.currentTimeMillis();
      System.out.println("#BitSet " + (aft - bef));
      line += df.format(numberofkeys * repeat  / (1000.0 * (aft-bef)))+"\t";
      StaticBitSet sbs = new StaticBitSet(N);
      bef = System.currentTimeMillis();
      for (int r = 0; r < repeat; ++r) {
        for (int i = 0; i < N; i += offset)
          sbs.set(i);
        for (int i = sbs.nextSetBit(0); i >= 0; i = sbs.nextSetBit(i + 1)) {
          sum += i;
        }
        if(fastclear) {
          sbs.clear();
        } else {
          for (int i = 0; i < N; i += offset)
            sbs.unset(i);
        }
      }
      aft = System.currentTimeMillis();
      System.out.println("#StaticBitSet " + (aft - bef));
      line += df.format(numberofkeys * repeat  / (1000.0 * (aft-bef)))+"\t";      
      if(fastclear)
      {
      SparseBitmap ewah = new sparsebitmap.SparseBitmap();

      bef = System.currentTimeMillis();
      for (int r = 0; r < repeat; ++r) {
        for (int i = 0; i < N; i += offset)
          ewah.set(i);
        for(int i : ewah) {
          sum += i;
        }          
        ewah.clear();
      }
      aft = System.currentTimeMillis();
      System.out.println("#SparseBitmap " + (aft - bef));
      line += df.format(numberofkeys * repeat  / (1000.0 * (aft-bef)))+"\t";
      }
      
      java.util.HashSet<Integer> h = new java.util.HashSet<Integer>();
      bef = System.currentTimeMillis();
      for (int r = 0; r < repeat; ++r) {
        for (int i = 0; i < N; i += offset)
          h.add(i);
        for (int i : h)
          sum += i;
        if(fastclear)
          h.clear();
        else {
          for (int i = 0; i < N; i += offset)
            h.remove(i);
        }
      }
      aft = System.currentTimeMillis();
      System.out.println("#HashSet " + (aft - bef));
      line += df.format(numberofkeys * repeat  / (1000.0 * (aft-bef)))+"\t";
      gnu.trove.set.TIntSet troveh = new gnu.trove.set.hash.TIntHashSet();
      bef = System.currentTimeMillis();
      for (int r = 0; r < repeat; ++r) {
        for (int i = 0; i < N; i += offset)
          troveh.add(i);
        TIntIterator j = troveh.iterator();
        while(j.hasNext())
          sum += j.next();
        if(fastclear)
          troveh.clear();
        else
          for (int i = 0; i < N; i += offset)
            troveh.remove(i);
      }
      aft = System.currentTimeMillis();
      System.out.println("#TIntHashSet " + (aft - bef));
      line += df.format(numberofkeys * repeat  / (1000.0 * (aft-bef)))+"\t";
      System.out.println(line);
    }
    System.out.println("#ignore = " + sum);

  }

  public static void main(String[] args) {
    sanityCheck();
    benchmark(false);
  }

}
