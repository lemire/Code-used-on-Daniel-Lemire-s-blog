import java.text.DecimalFormat;
import java.util.*;

/**
 * This is "throw away code". Use at your own risks.
 * 
 * @author Daniel Lemire
 * 
 */
public class testbitmap {
  public static long[] to64Bitmap(int[] array, int Max) {
    long[] ans = new long[Max / 64];
    for (int k = 0; k < array.length; ++k) {
      ans[array[k] / 64] |= 1l << (array[k] % 64);
    }
    return ans;
  }

  public static int[] to32Bitmap(int[] array, int Max) {
    int[] ans = new int[Max / 32];
    for (int k = 0; k < array.length; ++k) {
      ans[array[k] / 32] |= 1 << (array[k] % 32);
    }
    return ans;
  }

  public static int[] convertInt(List<Integer> array) {
    int[] ret = new int[array.size()];
    Iterator<Integer> iterator = array.iterator();
    for (int i = 0; i < ret.length; i++) {
      ret[i] = iterator.next().intValue();
    }
    return ret;
  }

  public static int[] from64Bitmap(long[] bitmap, int N) {
    int[] ans = new int[N];
    int counter = 0;
    for (int k = 0; k < bitmap.length; ++k) {
      for (int c = 0; c < 64; ++c) {
        if (((1l << c) & bitmap[k]) != 0) {
          ans[counter++] = k * 64 + c;
        }
      }
    }
    return ans;
  }

  public static int[] from64BitmapFast(long[] bitmap, int N) {
    int[] ans = new int[N];
    int counter = 0;
    for (int k = 0; k < bitmap.length; ++k) {
      long v = bitmap[k];
      while (v != 0) {
        final int ntz = Long.numberOfTrailingZeros(v);
        v ^= (1l << ntz);
        ans[counter++] = ntz + k * 64;
      }
    }
    return ans;
  }

  public static int[] from32Bitmap(int[] bitmap, int N) {
    int[] ans = new int[N];
    int counter = 0;
    for (int k = 0; k < bitmap.length; ++k) {
      for (int c = 0; c < 32; ++c) {
        if (((1 << c) & bitmap[k]) != 0) {
          ans[counter++] = k * 32 + c;
        }
      }
    }
    return ans;
  }

  public static int[] from32BitmapFast(int[] bitmap, int N) {
    int[] ans = new int[N];
    int counter = 0;
    for (int k = 0; k < bitmap.length; ++k) {
      int v = bitmap[k];
      while (v != 0) {
        final int ntz = Integer.numberOfTrailingZeros(v);
        v ^= (1 << ntz);
        ans[counter++] = ntz + k * 32;
      }
    }
    return ans;
  }

  public static int count(int[] bitmap) {
    int sum = 0;
    for (int k = 0; k < bitmap.length; ++k) {
      sum += Integer.bitCount(bitmap[k]);
    }
    return sum;
  }

  public static int count(long[] bitmap) {
    int sum = 0;
    for (int k = 0; k < bitmap.length; ++k) {
      sum += Long.bitCount(bitmap[k]);
    }
    return sum;
  }

  public static void test(int N, int nbr, boolean verbose) {
    DecimalFormat df = new DecimalFormat("0.###");
    ClusteredDataGenerator cdg = new ClusteredDataGenerator();
    for (int sparsity = 0; sparsity < 10; ++sparsity) {
      String line = "";
      line += sparsity;
      int[][] data = new int[N][];
      int[][] recov = new int[N][];
      int Max = (1 << (nbr + sparsity));
      for (int k = 0; k < N; ++k)
        data[k] = cdg.generateClustered(1 << nbr, Max);
      //
      int[][] bitmaps32 = new int[N][];
      long[][] bitmaps64 = new long[N][];
      long bef, aft;

      bef = System.currentTimeMillis();
      for (int k = 0; k < N; ++k)
        bitmaps32[k] = to32Bitmap(data[k], Max);
      aft = System.currentTimeMillis();
      line += "\t"+df.format((1 << nbr) * 0.001 / (aft - bef));
      bef = System.currentTimeMillis();
      for (int k = 0; k < N; ++k)
        recov[k] = from32Bitmap(bitmaps32[k], 1 << nbr);
      aft = System.currentTimeMillis();
      line += "\t"+ df.format((1 << nbr) * 0.001 / (aft - bef));
      if (!Arrays.deepEquals(recov, data))
        throw new RuntimeException("bug32");

      bef = System.currentTimeMillis();
      for (int k = 0; k < N; ++k)
        recov[k] = from32BitmapFast(bitmaps32[k], 1 << nbr);
      aft = System.currentTimeMillis();
      line += "\t"+ df.format((1 << nbr) * 0.001 / (aft - bef));
      if (!Arrays.deepEquals(recov, data)) {
        for (int k = 0; k < 128; ++k)
          System.out.println(recov[0][k] + " " + data[0][k]);
        throw new RuntimeException("bug32-fast");
      }

      bitmaps32 = new int[N][];
      recov = new int[N][];
      bef = System.currentTimeMillis();
      for (int k = 0; k < N; ++k)
        bitmaps64[k] = to64Bitmap(data[k], Max);
      aft = System.currentTimeMillis();
      line += "\t"+ df.format((1 << nbr) * 0.001 / (aft - bef));

      bef = System.currentTimeMillis();
      for (int k = 0; k < N; ++k)
        recov[k] = from64Bitmap(bitmaps64[k], 1 << nbr);
      aft = System.currentTimeMillis();
      line += "\t"+ df.format((1 << nbr) * 0.001 / (aft - bef));

      if (!Arrays.deepEquals(recov, data))
        throw new RuntimeException("bug64 slow");

      bef = System.currentTimeMillis();
      for (int k = 0; k < N; ++k)
        recov[k] = from64BitmapFast(bitmaps64[k], 1 << nbr);
      aft = System.currentTimeMillis();
      line += "\t"+ df.format((1 << nbr) * 0.001 / (aft - bef));

      if (!Arrays.deepEquals(recov, data))
        throw new RuntimeException("bug64 fast");

      bitmaps64 = new long[N][];
      recov = new int[N][];
      if (verbose)
        System.out.println(line);
    }
  }

  public static void main(String[] args) {
    final int N = 15;
    final int nbr = 16;
    test(N, nbr, false); // dry run
    test(N, nbr, true);
  }

}