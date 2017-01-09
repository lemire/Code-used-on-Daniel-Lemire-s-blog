How efficient is ARM Neon's population count function? 

``
gcc -O3 -o pop pop.c  -mfpu=neon

./pop && cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq

minneon = 1.322266 ns/byte
``

So at 1.2 GHz or 1.2 cycles per nanosecond, we take 1.3 nanosecond to process a byte. So this is nearly 1 cycle per input byte.



Code by Wojciech Mula


