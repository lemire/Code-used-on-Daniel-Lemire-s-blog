public class prime {
    // find a prime number larger or equal to min
    // we assume that x>=0,
    // note that Integer.MAX_VALUE  is prime.
    public static int nextPrime(int x) {
        // todo: check that x is positive!
        x |= 1; // go to next odd number
        for (; x <= Integer.MAX_VALUE ; x += 2) {
                if (IsPrime(x)) return x;
        }
        return -1; // todo: will only happen if x is negative
    }

    public static boolean IsPrime(int x) {
        if ((x & 1) != 0) { // odd case
            // we know that no divisor of x can be great than the sqrt
            final int limit = (int) Math.sqrt(x);
            for (int div = 3; div <= limit; div += 2) {
                  if ((x % div) == 0)
                      return false;
            }
            return true;
        }
        // x is even
        if (x == 2) {
          return true;
        }
        return false;
    }

}
