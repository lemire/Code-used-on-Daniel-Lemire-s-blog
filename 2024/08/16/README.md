## Batched Random

We benchmark fast shuffling functions using batched random index generation.


### Reference

* Nevin Brackett-Rozinsky, Daniel Lemire, [Batched Ranged Random Integer Generation](https://arxiv.org/abs/2408.06213), Software: Practice and Experience (to appear)


### Running Benchmarks


```
make
./stream
```

To get the C++ benchmarks, you can type `./benchmark --cpp`. They are disabled by default.
