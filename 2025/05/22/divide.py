# N is the total number of elements
# M is the number of chunks
# i is the index of the chunk (0-indexed)
def get_chunk_range_simple(N, M, i):
    quotient, remainder = divmod(N, M)
    # We have two cases:
    # 1. If i < remainder, the chunk size is quotient + 1
    # 2. If i >= remainder, the chunk size is quotient
    # The start of the chunk is the sum of all previous chunk
    # The end of the chunk is the start of the next chunk
    # plus the size of the current chunk
    # 
    # Let us consider the case where i < remainder:
    # We have i chunks of size quotient + 1.
    # The start of the i-th chunk is i * (quotient + 1)
    # The end of the i-th chunk is (i + 1) * (quotient + 1)
    #
    # Now let us consider the case where i >= remainder:
    # We have remainder chunks of size quotient + 1
    # and M - remainder chunks of size quotient.
    # The start of the i-th chunk is remainder * (quotient + 1) + (i - remainder) * quotient
    # The end of the i-th chunk is remainder * (quotient + 1) + (i - remainder + 1) * quotient
    #
    # i * quotient + i 
    # i * quotient + remainder 
    return (quotient * i + min(i, remainder), quotient * (i + 1) + min(i+1, remainder))

# Example usage
N = 20
M = 6

# Query specific chunk
for i in range(M):
  start, end = get_chunk_range_simple(N, M, i)
  print(f"Chunk {i} range: ({start}, {end}) size = {end - start}")