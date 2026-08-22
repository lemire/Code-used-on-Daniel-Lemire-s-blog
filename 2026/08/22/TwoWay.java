// Crochemore-Perrin Two-Way string matching (JACM 1991).
//
// Linear time in the haystack length, constant extra memory. This is the
// algorithm glibc, musl and newlib use for strstr/memmem. The version here
// is the "short needle" form: no bad-character shift table, just the
// critical factorization and the two-sided scan.
//
// Preprocessing is 2*m character comparisons. Searching does at most
// 2*n - m comparisons.
public final class TwoWay {

  private TwoWay() {}

  public static int indexOf(String haystack, String needle) {
    int m = needle.length();
    if (m == 0) {
      return 0;
    }
    int n = haystack.length();
    if (m > n) {
      return -1;
    }
    return indexOf(haystack.toCharArray(), n, needle.toCharArray(), m);
  }

  public static int indexOf(char[] haystack, char[] needle) {
    int m = needle.length;
    if (m == 0) {
      return 0;
    }
    int n = haystack.length;
    if (m > n) {
      return -1;
    }
    return indexOf(haystack, n, needle, m);
  }

  // Returns the start of the right half (suffix). period[0] is set to the
  // period of that right half. The left half is always shorter than the
  // global period of the needle, which is what makes the shifts safe.
  static int criticalFactorization(char[] needle, int m, int[] period) {
    // Lexicographic maximal suffix. maxSuffix is the last index of the
    // left half, or -1 when the right half is the whole needle.
    int maxSuffix = -1;
    int j = 0;
    int k = 1;
    int p = 1;
    while (j + k < m) {
      char a = needle[j + k];
      char b = needle[maxSuffix + k];
      if (a < b) {
        j += k;
        k = 1;
        p = j - maxSuffix;
      } else if (a == b) {
        if (k == p) {
          j += p;
          k = 1;
        } else {
          k++;
        }
      } else {
        maxSuffix = j++;
        k = p = 1;
      }
    }
    period[0] = p;

    // Reverse-lexicographic maximal suffix.
    int maxSuffixRev = -1;
    j = 0;
    k = p = 1;
    while (j + k < m) {
      char a = needle[j + k];
      char b = needle[maxSuffixRev + k];
      if (a > b) {
        j += k;
        k = 1;
        p = j - maxSuffixRev;
      } else if (a == b) {
        if (k == p) {
          j += p;
          k = 1;
        } else {
          k++;
        }
      } else {
        maxSuffixRev = j++;
        k = p = 1;
      }
    }

    // The longer of the two suffixes is a critical factorization.
    // Return the first index of the right half.
    if (maxSuffixRev + 1 < maxSuffix + 1) {
      return maxSuffix + 1;
    }
    period[0] = p;
    return maxSuffixRev + 1;
  }

  static boolean equalRange(char[] a, int aOff, char[] b, int bOff, int len) {
    for (int i = 0; i < len; i++) {
      if (a[aOff + i] != b[bOff + i]) {
        return false;
      }
    }
    return true;
  }

  static int indexOf(char[] haystack, int n, char[] needle, int m) {
    int[] periodHolder = new int[1];
    int suffix = criticalFactorization(needle, m, periodHolder);
    int period = periodHolder[0];

    if (equalRange(needle, 0, needle, period, suffix)) {
      // Periodic needle: a mismatch can only advance by the period, so
      // remember how much of the previous window is still valid.
      int memory = 0;
      int j = 0;
      while (j <= n - m) {
        int i = suffix > memory ? suffix : memory;
        while (i < m && needle[i] == haystack[i + j]) {
          i++;
        }
        if (i >= m) {
          i = suffix - 1;
          while (memory <= i && needle[i] == haystack[i + j]) {
            i--;
          }
          if (i < memory) {
            return j;
          }
          j += period;
          memory = m - period;
        } else {
          j += i - suffix + 1;
          memory = 0;
        }
      }
    } else {
      // The two halves are distinct. Any mismatch lets us shift by more
      // than half the needle, so we need no memory.
      period = (suffix > m - suffix ? suffix : m - suffix) + 1;
      int j = 0;
      char needleSuffix = needle[suffix];
      while (j <= n - m) {
        while (haystack[suffix + j] != needleSuffix) {
          j++;
          if (j > n - m) {
            return -1;
          }
        }
        int i = suffix + 1;
        while (i < m && needle[i] == haystack[i + j]) {
          i++;
        }
        if (i >= m) {
          i = suffix - 1;
          while (i >= 0 && needle[i] == haystack[i + j]) {
            i--;
          }
          if (i < 0) {
            return j;
          }
          j += period;
        } else {
          j += i - suffix + 1;
        }
      }
    }
    return -1;
  }
}
