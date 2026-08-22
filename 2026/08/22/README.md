# Quadratic `String.indexOf` versus Two-Way

Java's `String.indexOf` can take time Θ(*n*·*m*) on adversarial inputs.
The Two-Way algorithm of Crochemore and Perrin is linear.

```
javac Adversarial.java TwoWay.java Benchmark.java
java Benchmark              # comparison, default haystack 1 MiB
java Adversarial            # original indexOf-only sweep
```

Pass a haystack length if you want a size other than 1,048,576.

`Adversarial.java` is the original benchmark. `TwoWay.java` is a Java
translation of the short-needle Crochemore–Perrin searcher. `Benchmark.java`
runs both on the same inputs and checks that Two-Way agrees with
`String.indexOf` on random and adversarial strings.

Results in `results.txt` were collected with OpenJDK 25.0.1 on an Apple M4 Max.
