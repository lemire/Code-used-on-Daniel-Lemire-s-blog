// javac Cardenas.java  && java Cardenas

import java.math.*;


/**
 * Implementation of Cardenas' formula in Java.
 * D. Lemire , November 2018
 */
public class Cardenas {

  public static int cardenas64(long[] cards, int n) {
    double product = 1;
    for(int k = 0;  k < cards.length; k++) {
      product *= cards[k];
    }
    return (int)Math.round(product 
         * (1- Math.pow(1 - 1.0/product,n)));
 }


 public static int bettercardenas64(long[] cards, int n) {
  double product = 1;
  for(int k = 0;  k < cards.length; k++) {
    product *= cards[k];
  }
  return (int)Math.round(product * 
    -Math.expm1(Math.log1p(-1.0/product) * n));
}
 // root ** (2^b)
 private static BigDecimal lemirepower2(BigDecimal root, int b) {
   BigDecimal answer = root;
   for(int k = 0; k < b; k++)
     answer = answer.multiply(answer, MathContext.DECIMAL128);
   return answer;
 }

 // root ^n in a numerically stable manner (power(n) is terrible)
 public static BigDecimal lemirepower(BigDecimal root, int n) {
  BigDecimal answer = BigDecimal.ONE;
   for(int b = 0; b < 31; b++) {
     if((n & (1<<b)) != 0) {
      answer = answer.multiply(lemirepower2(root,b), MathContext.DECIMAL128);
     }
   }
   return answer;
 }

  public static int cardenas128(long[] cards, int n) {
    BigDecimal product = product(cards);
    BigDecimal oneover = BigDecimal.ONE.divide(product, 
       MathContext.DECIMAL128);
    BigDecimal proba = BigDecimal.ONE.subtract(oneover, 
    MathContext.DECIMAL128);
    proba = lemirepower(proba,n);
    return (int)Math.round(product.subtract(
       product.multiply(proba, MathContext.DECIMAL128),
        MathContext.DECIMAL128).doubleValue());
    }

  public static BigDecimal product(long[] cards) {
    BigDecimal product = BigDecimal.ONE;
    for(int k = 0;  k < cards.length; k++) {
      product = product.multiply(new BigDecimal(cards[k]),
        MathContext.DECIMAL128);
    }
    return product;
  }

  public static void main(String[] args) {
     long cards[] = {16,16,15,5,2,94,21648,92,42,7,9,2,6,73,119};
     BigDecimal product = product(cards);
     System.out.println("product of column cards is "+product.doubleValue());
     int[] ns = {2, 20, 48842, 1_000_000, 10_000_000, 100_000_000, 500_000_000, 1_000_000_000
      , 2_000_000_000};
     boolean unstable64 = false;
     boolean unstableb64 = false;

     boolean unstable128 = false;
     for(int n : ns) {
       System.out.println("n = "+n);
       int ans64 = (int)Math.round(cardenas64(cards,n));
       int bans64 = (int)Math.round(bettercardenas64(cards,n));

       int ans128 = (int)Math.round(cardenas128(cards,n));
       System.out.println("64-bit floats = "+ans64);
       System.out.println("64-bit floats = "+bans64);

       System.out.println("128-bit floats = "+ans128);
       if(( ans64 > n ) || (ans64 == 0)) 
         unstable64 = true;
        if(( bans64 > n ) || (bans64 == 0)) 
        unstableb64 = true;
       if((ans128 > n ) || (ans128 == 0))
         unstable128 = true;

        System.out.println();
     }
     if(unstable64)
       System.err.println("probable numerical instability with 64-bit math.");
     if(unstableb64)
       System.err.println("probable numerical instability with better 64-bit math.");

     if(unstable128)
       System.err.println("probable numerical instability with 128-bit math.");
       
  }
}
