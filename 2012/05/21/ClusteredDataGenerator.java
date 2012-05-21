/**
 * (c) Daniel Lemire, http://lemire.me
 * 
 * This is code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 */

/**
 * This class will generate uniformly distributed lists of random integers.
 * 
 * @author Daniel Lemire
 */
class ClusteredDataGenerator {

  UniformDataGenerator unidg = new UniformDataGenerator();

  public ClusteredDataGenerator() {
  }

  void fillUniform(int[] array, int offset, int length, int Min, int Max) {
    int[] v = unidg.generateUniform(length, Max - Min);
    for (int k = 0; k < v.length; ++k)
      array[k + offset] = Min + v[k];
  }

  void fillClustered(int[] array, int offset, int length, int Min, int Max) {
    final int range = Max - Min;
    if ((range == length) || (length <= 10)) {
      fillUniform(array, offset, length, Min, Max);
      return;
    }
    final int cut = length / 2
      + ((range - length - 1 > 0) ? unidg.rand.nextInt(range - length - 1) : 0);
    final double p = unidg.rand.nextDouble();
    if (p < 0.25) {
      fillUniform(array, offset, length / 2, Min, Min + cut);
      fillClustered(array, offset + length / 2, length - length / 2, Min + cut,
        Max);
    } else if (p < 0.5) {
      fillClustered(array, offset, length / 2, Min, Min + cut);
      fillUniform(array, offset + length / 2, length - length / 2, Min + cut,
        Max);
    } else {
      fillClustered(array, offset, length / 2, Min, Min + cut);
      fillClustered(array, offset + length / 2, length - length / 2, Min + cut,
        Max);
    }
  }

  /**
   * generates randomly N distinct integers from 0 to Max.
   */
  public int[] generateClustered(int N, int Max) {
    int[] array = new int[N];
    fillClustered(array, 0, N, 0, Max);
    return array;
  }

  public static void main(String[] args) {
    int[] example = (new ClusteredDataGenerator()).generateClustered(20, 1000);
    for (int k = 0; k < example.length; ++k)
      System.out.println(example[k]);
  }

};
