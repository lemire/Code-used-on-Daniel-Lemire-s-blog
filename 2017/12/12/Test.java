import java.util.Random;

public class Test {

   public static void main(String[] args) {
     final int N = 100000;
     int[] x = new int[N];
     int[] y = new int[N];
     int[] z = new int[N];

     Random r = new Random();
     for(int k = 0; k < N; k++) {
       x[k] = r.nextInt();
       y[k] = r.nextInt();
       z[k] = x[k] + y[k];
     }
     System.out.println(Correlation(x,y)+" "+ Correlation(x,z) );
   }
   // stolen from stackoverflow
   public static double Correlation(int[] xs, int[] ys) {
    //TODO: check here that arrays are not null, of the same length etc

    double sx = 0.0;
    double sy = 0.0;
    double sxx = 0.0;
    double syy = 0.0;
    double sxy = 0.0;

    int n = xs.length;

    for(int i = 0; i < n; ++i) {
      double x = xs[i];
      double y = ys[i];

      sx += x;
      sy += y;
      sxx += x * x;
      syy += y * y;
      sxy += x * y;
    }

    // covariation
    double cov = sxy / n - sx * sy / n / n;
    // standard error of x
    double sigmax = Math.sqrt(sxx / n -  sx * sx / n / n);
    // standard error of y
    double sigmay = Math.sqrt(syy / n -  sy * sy / n / n);

    // correlation is just a normalized covariation
    return cov / sigmax / sigmay;
  }

}
