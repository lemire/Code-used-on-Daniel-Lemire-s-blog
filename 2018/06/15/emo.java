import java.util.*;

public class emo {
  static String emostring ="😂😍🎉👍";

  public static String fromCodePointToString(int code) {
    return new StringBuilder().appendCodePoint(code).toString();
  }

  public static void main(String[] args) {
    System.out.println(emostring);
    System.out.println(emostring.length());
    System.out.println(emostring.codePointCount(0, emostring.length()));

    System.out.println(emostring.charAt(0));// incorrect
    System.out.println(fromCodePointToString(emostring.codePointAt(0)));// correct
    StringBuilder bigstrbuild = new StringBuilder();
    for (int k = 0; k < 1000000; k++) {
      bigstrbuild.append(emostring);
    }
    String big = bigstrbuild.toString();
    System.out.println("string of length "+big.length());
    int true_length = big.codePointCount(0, big.length());
    Random r = new Random();
    long s = System.currentTimeMillis();
    for (int k = 0; k < 20; k++) {
      int codePointOffset = r.nextInt(true_length);
      int code = big.codePointAt(big.offsetByCodePoints(0, codePointOffset));
      System.out.print(fromCodePointToString(code));// correct
    }
    long t = System.currentTimeMillis();
    System.out.println();
    System.out.println((t-s)+"ms");
  }

}
