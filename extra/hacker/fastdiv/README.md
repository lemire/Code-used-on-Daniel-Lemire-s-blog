## Fast modulo reduction/division

Divisions and modulo reductions are tremendously expensive, even on modern computers. 
When the divisor is known ahead of time, we can transform a division into a single
multiplication followed by a shift.

(The "magic" multiplier ``m`` can be determined by looking for the smallest integer at least as large
as ``2^L/d`` where ``d`` is the divisor and ``L`` is the size of the shift. For ``L`` sufficiently large,
doing ``( x * m ) >> L`` is equivalent to dividing by ``d``. That is, ``( x * m ) >> L`` is the quotient.)

A modulo reduction can be further achieved
by taking the result of the division, multiplying by the divisor and doing a subtraction.

(E.g., ``x - ( ( x * m ) >> L ) * d`` is equivalent to ``x % d``.)

Thus, for example, a compiler will transform this function...

```C
uint32_t  mod23(uint32_t a) {
  return a % 23;
}
```

Into a sequence of four non-move assembly instructions  ``imul``,  ``shr``, ``imul``, ``sub``. It seems that this function has a latency of at least 3 + 1 + 3 + 1 = 8 (because  ``imul`` has a latency of 3).

(These instructions correspond to ``x - ( ( x * m ) >> L ) * d``.)

Can we do better?

Firstly, we could avoid the shift entirely by choosing ``L=64``. That is always possible for 32-bit divisions. That is, we use "word-aligned arithmetic". That might save one cycle, maybe.

Secondly, we can avoid the subtraction entirely. That part is slightly more interesting.

Let us go back to our formula: ``( x * m ) >> L`` gives us the quotient, but what about the bits we discard by the right shift ? (``( x * m ) % ((1<< L) - 1)``) These bits contain all the information necessary to compute the modulo reduction. It is a value between 0 and ``1<<L`` and all you need to do to get the actual modulo reduction is to multiply by the divisor (``d``) and shift the result by ``L``. So...

1. ``( x * m ) >> L`` is the quotient (``x / d``)
2. ``((( x * m ) % ((1<< L) - 1) )) * d ) >> L`` is the modulo reduction (``x % d``)

The latter expression looks monstrous, but if ``L`` is the number of bits in a machine word, it becomes much more economical.

Let us look at actual code:

```C
#define MUL64HIGH(rh,i1,i2) asm ("mulq %2" : "=d"(rh) : "a"(i1), "r"(i2) : "cc")

uint32_t fastmod23(uint32_t a) {
    uint64_t lowbits =  UINT64_C(802032351030850071) * a; // high 64 bits of this mult is the division
    // we use the low bits to retrieve the modulo
    uint64_t highbits;
    MUL64HIGH(highbits,lowbits,UINT64_C(23));
    return highbits;
}
```

Looking at the assembly of this function, we find only two non-move instructions (``imul`` and ``mul``). So the latency could be as low as 3 + 3 = 6.

We can design a benchmark to measure the latency of the two C functions... (see the mod23.c file) This test measures latency, and it has at least a one cycle overhead:

```C
for(uint32_t k = 0; k < maxval; ++k) sumofmods += fastmod23(k + sumofmods);
```


```
$ gcc -O3 -o mod23 mod23.c
$ ./mod23
sumofmod23(maxval):  11.27 cycles per operation
fastsumofmod23(maxval):  9.05 cycles per operation

$ icc -O3 -o mod23 mod23.c
$ ./mod23
sumofmod23(maxval):  11.23 cycles per operation
fastsumofmod23(maxval):  9.05 cycles per operation

$ clang  -O3 -o mod23 mod23.c
$ ./mod23
sumofmod23(maxval):  11.05 cycles per operation
fastsumofmod23(maxval):  9.04 cycles per operation
```

So, at a glance, we can outdo the compiler by a couple of cycles. That's what we would expect since we do away with two 1-cycle instructions along the data-dependency path (``shr`` and ``sub``).

Further work: What about 64-bit divisions? What about signed divisions? 

Further reading: Warren, Henry S. Hacker's delight. Pearson Education, 2013.

## Fast division

```

div23(unsigned long):                              # @div23(unsigned long)
        movabs  rcx, 7218291159277650633
        mov     rax, rdi
        mul     rcx
        sub     rdi, rdx
        shr     rdi
        lea     rax, [rdi + rdx]
        shr     rax, 4
        ret

fastdiv23(unsigned long):                          # @fastdiv23(unsigned long)
        movabs  rcx, 802032351030850071
        mov     rax, rdi
        mul     rcx
        mov     rax, rdx
        ret
```

