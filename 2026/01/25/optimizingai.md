# Optimizing Python scripts with AI

One of the first steps we take when we want to optimize software is to look
at profiling data. Software profilers are tools that try to identify where
your software spends its time. Though the exact approach can vary, a typical profiler samples your software (steps it at regular intervals) and collects statistics. If your software is routinely stopped in a given function, this function is likely using a lot of time. In turn, it might be where you should put your optimization efforts.

Matteo Collina recently [shared with me his work on feeding profiler data](https://github.com/platformatic/pprof-to-md) for software optimization purposes in JavaScript. Essentially, Matteo takes the profiling data, and prepares it in a way that an AI can comprehend. The insight is simple but intriguing: tell an AI how it can capture profiling data and then let it optimize your code, possibly by repeatedly profiling the code. The idea is not original since AI tools will, on their own, figure out that they can get profiling data.

How well does it work? I had to try it. 

## Case 1. Code amalgamation script

For the simdutf software library, [we use an amalgamation script](https://raw.githubusercontent.com/simdutf/simdutf/refs/heads/master/singleheader/amalgamate.py): it collects all of the C++ files on disk, does some shallow parsing and glues them together according to some rules.

I first ask the AI to optimize the script without access to profiling data. What it did immediately was to add a file cache. The script repeatedly loads the same files from disk (the script is a bit complex). This saved about 20% of the running time.

Specifically, the AI replaced this naive code...
```Python
def read_file(file):
    with open(file, 'r') as f:
        for line in f:
            yield line.rstrip()
```

by this version with caching...

```Python
def read_file(file):
    if file in file_cache:
        for line in file_cache[file]:
            yield line
    else:
        lines = []
        with open(file, 'r') as f:
            for line in f:
                line = line.rstrip()
                lines.append(line)
                yield line
        file_cache[file] = lines
```

Could the AI do better with profiling data? I instructed it to run the Python profiler: `python -m cProfile -s cumtime myprogram.py`. It found two additional optimizations:

1. It precompiled the regular expressions (`re.compile`). It replaced 

```python
  if re.match('.*generic/.*.h', file):
    # ...
```

by

```python
if generic_pattern.match(file):
    # ...
```

where elsewhere in the code, we have...

```python
generic_pattern = re.compile(r'.*generic/.*\.h')
```

2. Instead of repeatedly calling `re.sub` to do a regular expression substitution, it filtered the strings by checking for the presence of a keyword in the string first.
```python
if 'SIMDUTF_IMPLEMENTATION' in line: # This IF is the optimization
  print(uses_simdutf_implementation.sub(context.current_implementation+"\\1", line), file=fid)
else:
  print(line, file=fid) # Fast path
```

These two optimizations could probably have been arrived at by looking at the code directly, and I cannot be certain that they were driven by the profiling data. But I can tell that they do appear in the profile data.

Unfortunately, the low-hanging fruit, caching the file access, represented the bulk of the gain. The AI was not able to further optimize the code. So the profiling data did not help much.

## Case 2: Check Link Script

When I design online courses, I often use a lot of links. These links break over time. So I have a simple Python script that goes through all the links, and verifies them. 

I first ask my AI to optimize the code. It did the same regex trick, compiling the regular expression. It created a thread pool and made the script asynchronous.

```python
with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
    url_results = {url: executor.submit(check_url, url) for url in urls_to_check}
    for url, future in url_results.items():
        url_cache[url] = future.result()
```

This parallelization more than doubled the speed of the script.

It cached the URL checks in an interesting way, using `functools`:

```python
from functools import lru_cache

@lru_cache(maxsize=None)
def check(link):
    # ...
```

I did not know about this nice trick. This proved useless in my context because I rarely have several times the same link.

I then started again, and told it to use the profiler. It did much the same thing, except for the optimization of the regular expression.

As far as I can tell all optimizations were in vain, except for the multithreading. And it could do this part without the profiling data.


## Conclusion so far

The Python scripts I tried were not heavily optimized, as their performance was not critical. They are relatively simple. 

For the amalgamation, I got a 20% performance gain for 'free' thanks to the file caching. The link checker is going to be faster now that it is multithreaded. Both optimizations are valid and useful, and will make my life marginally better.

In neither case I was able to discern benefits due to the profiler data. I was initially hoping to get the AI busy optimizing the code in a loop, continuously running the profiler, but it did not happen in these simple cases. The AI optimized code segments that contributed little to the running time as per the profiler data.

To be fair, profiling data is often of limited use. The real problems are often architectural and not related to narrow bottlenecks. Even when there are identifiable bottlenecks, a simple profiling run can fail to make them clearly identifiable. Further, profilers become more useful as the code base grows, while my test cases are tiny.

Overall, I expect that the main reason for my relative failure is that I did not have the right use cases. I think that collecting profiling data and asking an AI to have a look might be a reasonable first step at this point.