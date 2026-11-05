# A performance trap: when store forwarding fails

A store does not go straight into the cache. Waiting for that write would stall the processor. It sits in a store buffer, and a later load can take its bytes from that buffer. That is store forwarding.

Forwarding works when the load is the same size as the store, or smaller and completely inside it. It fails when the load is wider than the store, or when the two ranges only partly overlap. The load then waits until the store has reached the cache.

Here is an example in C. We have a buffer, that might have received data previously. Yet one byte is overwritten, and we try to load 8 bytes. It will typically cause a store forwarding failure.

```c
uint64_t reload_byte(char *p, unsigned char v) {
    // Copy one byte from v into the buffer.
    memcpy(p, &v, 1);
    uint64_t w;
    // Copy eight bytes from the buffer into w.
    memcpy(&w, p, 8);
    return w;
}
```


A 4-byte store followed by an 8-byte load would cause the same issue. So would  an 8-byte store followed by an 8-byte load that starts four bytes later: the ranges overlap.

You can sidestep the issue by working in a register, like the following example.
```c
uint64_t update_byte(char *p, unsigned char v) {
    uint64_t x;
    // load from p to x
    memcpy(&x, p, 8);
    x = (x & ~0xffull) | v;
    // store the modified result to p
    memcpy(p, &x, 8);
    return x;
}
```

Further, if you store eight bytes and read those same eight back, a compiler might delete the reload. 

I ran some experiments to see how many cycles various store/load operations take.

| What the iteration does | Xeon Gold 6548N | Xeon 6975P-C | EPYC 9R45 | Graviton 5 | Apple M4 Max |
|---|---:|---:|---:|---:|---:|
| store 8, load 8, same address | 1 | 1 | 1 | 5 | 5 |
| store 8, load 4 at +4 | 5 | 5 | 7 | 6 | 5 |
| store 4, load 8 | 19 | 19 | 14 | 5 | 5 |
| store 4, then store 4, load 8 | 20 | 20 | 15 | 5 | 8 |

![Cycles for a dependent store followed by a load](latency.webp)

On the Xeons and the EPYC, a load that matches the store takes one cycle. A wider load takes 19 cycles on the Xeons and 14 on the EPYC.

On the M4 and Graviton 5, a wider load takes 5 cycles. Two stores bring the M4 to 8.



https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/11/storeforwarding
