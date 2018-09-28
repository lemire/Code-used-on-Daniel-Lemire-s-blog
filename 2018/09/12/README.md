Here's the invocation to turn off prefetchers:

sudo wrmsr -a 0x1a4 "$((2#1111))" && sudo rdmsr -a -x 0x1A4

and back on:

sudo wrmsr -a 0x1a4 "$((2#0000))" && sudo rdmsr -a -x 0x1A4

It also prints out the state to check that it worked (wrmsr might be ignored on some VMs). It should print:
f
f
f
f
when you turn them off.

https://software.intel.com/en-us/articles/disclosure-of-hw-prefetcher-control-on-some-intel-processors

Tracking cache misses:
https://stackoverflow.com/questions/23736999/is-it-possible-to-know-the-address-of-a-cache-miss
