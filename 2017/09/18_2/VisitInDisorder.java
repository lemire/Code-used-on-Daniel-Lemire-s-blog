
// see T. E. Hull and A. R. Dobell (1962). ‘Random number generators.’ Soc. Indust. Appl. Math. Rev. 4, 230–254.
public class VisitInDisorder {
    int maxrange;
    int prime;
    int index;
    int offset;

    public VisitInDisorder(int range) {
      if(range < 2) throw new IllegalArgumentException("Your range need to be greater than 1 "+range);
      int min = range / 2 ;
      maxrange = range;
      prime = selectCoPrimeResev(min, range);
      offset = java.util.concurrent.ThreadLocalRandom.current().nextInt(range);
      index = 0;
    }

    public int getCurrentValue() {
      return (int)(( (long) index * prime + offset ) % ( maxrange));
        // the multiplication and the modulo could be optimized away with 
        // some work
    }

    public boolean hasNext() {
      return index < maxrange;
    }

    public int next() {
      int answer = getCurrentValue();
      index ++;
      return answer;
    }

    public static void main(String[] args) {
      int N = args.length > 0 ? Integer.parseInt(args[0]): 32;
      VisitInDisorder vid = new VisitInDisorder(N);
      System.out.println("initialized...");
      System.out.println("Generating values between 0 and "+N+" in random order");
      java.util.HashSet<Integer> checker = new java.util.HashSet<Integer>();
      while(vid.hasNext()) {
        int n = vid.next();
        if(n < 0) throw new RuntimeException("negative");
        if(n >= N) throw new RuntimeException("too big");
        checker.add(n);
        System.out.print(n + " ");
      }
      System.out.println();
      if(N != checker.size()) throw new RuntimeException("bad set size");
    }

    final static int MAX_COUNT = 100000;

    static int selectCoPrimeResev(int min, int target) {
      int count = 0;
      int selected = 0;
      java.util.concurrent.ThreadLocalRandom rand = java.util.concurrent.ThreadLocalRandom.current();
      for(int val = min; val < target; ++val) {
        if(coprime(val,target)) {
          count += 1;
          if((count == 1) || ( rand.nextInt(count) < 1 )) {
            selected = val;
          }
        };
        if(count == MAX_COUNT) return val;
      }
      return selected;
    }


    static boolean coprime(int u, int v) {
       return gcd(u,v) == 1;
    }

    static int gcd(int u, int v) {
       int shift;
       if (u == 0) return v;
       if (v == 0) return u;
       for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
       }

       while ((u & 1) == 0)
        u >>= 1;

      do {
        while ((v & 1) == 0)
            v >>= 1;
        if (u > v) {
            int t = v;
            v = u;
            u = t;
        }
        v = v - u;
      } while (v != 0);
      return u << shift;
   }


}
