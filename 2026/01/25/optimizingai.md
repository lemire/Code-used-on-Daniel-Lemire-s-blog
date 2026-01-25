# Optimizing Python scripts with AI

One of the first steps we take when we want to optimize software is to look
at profiling data. Software profilers are tools that try to identify where
your software spends its time. Though the exact approach can vary, a typical profiler samples your software (steps it at regular intervals) and collects statistics. If your software is routinely stopped in a given function, this function is likely using a lot of time. In turn, it might be where you should put your optimization efforts.

Matteo Collina recently [shared with me his work on feeding profiler data](https://github.com/platformatic/pprof-to-md) for software optimization purposes in JavaScript. Essentially, Matteo takes the profiling data, and prepares it in a way that an AI can comprehend. The insight is simple but intriguing: tell an AI how it can capture profiling data and then let it optimize your code, possibly by repeatedly profiling the code. The idea is not original since AI tools will, on their own, figure out that they can get profiling data.

How well does it work? I had to try it. 

## Case 1. Code amalgamation script

For the simdutf software library, [we use an amalgamation script](https://raw.githubusercontent.com/simdutf/simdutf/refs/heads/master/singleheader/amalgamate.py): it collects all of the C++ files on disk, does some shallow parsing and glues them together according to some rules.

I first ask the AI to optimize the script without access to profiling data. What it did immediately was to add a file cache. The script repeatedly loads the same files from disk (the script is a bit complex). This saved about 20% of the running time.

Could the AI do better with profiling data? I instructed it to run the Python profiler: `python -m cProfile -s cumtime myprogram.py`. It found two additional optimizations:

1. It precompiled the regular expressions (`re.compile`).
2. Instead of repeatedly calling `re.sub` to do a regular expression substitution, it filtered the strings by checking for the presence of a keyword in the string first.

These two optimizations could probably have been arrived at by looking at the code directly, and I cannot be certain that they were driven by the profiling data. But I can tell that they do appear in the profile data.

Unfortunately, the low-hanging fruit, caching the file access, represented the bulk of the gain. The AI was not able to further optimize the code. So the profiling data did not help much.

## Case 2: Check Link Script

When I design online courses, I often use a lot of links. These links break over time. So I have a simple Python script that goes through all the links, and verifies them. 

I first ask my AI to optimize the code. It did the same regex trick, compiling the regular expression. It created a thread pool and made the script asynchronous.

It cached the URL checks in an interesting way, using `functools`:

```python
from functools import lru_cache

@lru_cache(maxsize=None)
def check(link):
    // ...
```

I did not know about this nice trick. This proved useless in my context because I rarely have several times the same link.

I then started again, and told it to use the profiler. It did much the same thing, except for the optimization of the regular expression.

As far as I can tell all optimizations were in vain, except for the multithreading. And it could do this part without the profiling data.


## Conclusion so far

The Python scripts I tried were not optimized, as their performance is not critical. They are relatively simple. 

For the amalgamation, I got a 20% performance gain for 'free' thanks to the file caching. The link checker is going to be faster now that it is multithreaded. Both optimizations are valid and useful, and will make my life marginally better.

In neither case I was able to discern benefits due to the profiler data. I was initially hoping to get the AI busy optimizing the code in a loop, continuously running the profiler, but it did not happen in these simple cases. The AI optimized code segments that contributed little to the running time as per the profiler data.

To be fair, profiling data is often of limited use as the real problems are architectural and not related to narrow bottlenecks. And when profiling is useful, it is often with large code bases that have clearly identifiable bottlenecks. My test cases are small (a single script). So I will continue my tests!