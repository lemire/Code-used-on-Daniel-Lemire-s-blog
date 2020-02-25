There are fast (floating point) approximations to the binomial coefficients. Would these do? If they would do, then it is probably by far the simplest approach and probably the fastest too... because there are lots of good, well-tested implementations out there.



If not, then it seems that one has options regarding how to compute from scratch the binomial coefficient.

You can brute force it, computing the factorial of the numerator and the factorial of the denominator. This will overflow quickly if n is "a tiny bit large". So one has to rely on big-integer implementations. It is fine, but I suspect it is not going to give you optimal performance.
You can do away with big-integer arithmetic, assuming that the binomial coefficient itself fits in a machine word, by replacing the two multiplications that each iteration does by a multiplication followed by a division. Of course, the division is much slower than the multiplication, so it is going to be harmful. Yet you can optimize away this issue... I'd need to do some programming, but I think you can get pretty close to the performance of your current code, without having to worry with overflows and without having to use some big-integer implementation.
Then there are fancier approach like Goetgheluck's that identify prime powers and so forth, but I am skeptical that they would work well in practice. They may work well if n is very large, but if it is, then the binomial coefficient may not fit in a machine word and you have other problems.
