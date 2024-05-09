import digit_to_str
import timeit
print(digit_to_str.all_strings())
print(digit_to_str.all_strings_new())
print(digit_to_str.all_strings_buffered())
print(digit_to_str.digit_to_str(1))
print(digit_to_str.digit_to_str_from(1))
print(digit_to_str.digit_to_str_new(1))
print(digit_to_str.digit_to_str_buffer(1))

volume = 0
for i in range(10):
    volume += len(digit_to_str.digit_to_str(i))
print(f"Total volume: {volume} bytes, {volume/10:.3f} bytes per string")

num_iterations = 1000000
print(f"all_strings_buffered: ")
elapsed_time = timeit.timeit(lambda : digit_to_str.all_strings_buffered(), number=num_iterations)
print(f"Execution time for {num_iterations} iterations: {elapsed_time:.6f} seconds: {num_iterations*volume/elapsed_time/1e9:.3f} GB/s")

print(f"all_strings: ")
elapsed_time = timeit.timeit(lambda : digit_to_str.all_strings(), number=num_iterations)
print(f"Execution time for {num_iterations} iterations: {elapsed_time:.6f} seconds: {num_iterations*volume/elapsed_time/1e9:.3f} GB/s")

print(f"all_strings_new: ")
elapsed_time = timeit.timeit(lambda : digit_to_str.all_strings_new(), number=num_iterations)
print(f"Execution time for {num_iterations} iterations: {elapsed_time:.6f} seconds: {num_iterations*volume/elapsed_time/1e9:.3f} GB/s")


print(f"all_strings_new_with_check: ")
elapsed_time = timeit.timeit(lambda : digit_to_str.all_strings_new_with_check(), number=num_iterations)
print(f"Execution time for {num_iterations} iterations: {elapsed_time:.6f} seconds: {num_iterations*volume/elapsed_time/1e9:.3f} GB/s")


print(f"constructing from existing strings in pure python: ")
answers = [digit_to_str.digit_to_str(i) for i in range(10)]
elapsed_time = timeit.timeit(lambda : [answers[i] for i in range(10)], number=num_iterations)
print(f"Execution time for {num_iterations} iterations: {elapsed_time:.6f} seconds: {num_iterations*volume/elapsed_time/1e9:.3f} GB/s")

print(f"trivial return in python: ")
elapsed_time = timeit.timeit(lambda : answers, number=num_iterations)
print(f"Execution time for {num_iterations} iterations: {elapsed_time:.6f} seconds: {num_iterations*volume/elapsed_time/1e9:.3f} GB/s")
