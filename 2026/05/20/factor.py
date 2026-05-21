def prime_factors(n):
    """
    Returns the prime factorization of n as a list of primes (with multiplicity).
    Example: prime_factors(84) -> [2, 2, 3, 7]
    """
    if n <= 0:
        raise ValueError("Only positive integers are supported")
    if n == 1:
        return []

    factors = []
    
    # Handle factor 2 separately
    while n % 2 == 0:
        factors.append(2)
        n //= 2
    
    # Check odd factors up to sqrt(n)
    i = 3
    while i * i <= n:
        while n % i == 0:
            factors.append(i)
            n //= i
        i += 2
    
    # If n is a prime number greater than 2
    if n > 1:
        factors.append(n)
    
    return factors



import random


n = random.randint(1, 2**64) # 18401055938125660800

print(f"Random number: {n}")
f = prime_factors(n)
print(f"Prime factors: {f}")

jf = set(f)
print(f"Unique prime factors: {jf}")

factor_multiplicities = {p: f.count(p) for p in jf}
print(f"Factor multiplicities: {factor_multiplicities}")

candidates = [1]


for p in factor_multiplicities:
    new_candidates = []
    for c in candidates:
        for i in range(factor_multiplicities[p] + 1):
            if c * (p ** i) < 2**32:
                new_candidates.append(c * (p ** i))
    for new_c in new_candidates:
        candidates.append(new_c)

print(f"Number of candidates: {len(candidates)}")

m = max(candidates)
print(f"Maximum candidate: {m}")

leftover = n // m
print(f"Leftover: {leftover}")

if leftover >= 2**32:
    print("Leftover is too large, cannot find a suitable candidate.")
