Java provides good results (better than Go) but suffers also from poor inlining. It is possible that bound checking is hurting us as well.

```

        mvn clean install
        java -cp target/microbenchmarks-0.0.1-jar-with-dependencies.jar me.lemire.microbenchmarks.algorithms.Shuffle
```
