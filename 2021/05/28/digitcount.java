class digitcount {
    final static int table[] = {9,      99,      999,      9999,
        99999, 999999, 9999999, 99999999, 
        999999999, 2147483647};

    // assume that num is positive ()>=0)
    static int count(int num) {
        // call to numberOfLeadingZeros should be fast.
        // https://bugs.java.com/bugdatabase/view_bug.do?bug_id=6823354 
        int l2 = 31 - Integer.numberOfLeadingZeros(num|1);
        int ans = ((9*l2)>>>5);
        if (num > table[ans]) { ans += 1; }
        return ans + 1;
    }

    public static void main(String[] args) {
        for(int i = 0; i>=0; i++) {
            if(count(i) != String.valueOf(i).length()) {
                throw new RuntimeException("bug" + i);
            }
        }
    }
}