```
page count: 32, volume:0.125 MB
std::fill(p, p + n, 0)      : 1.7157 GB/s
std::fill(p, p + n, '\0')   : 62.1539 GB/s
memset(p,0,n)               : 62.0907 GB/s

std::fill(p, p + n, 0)      : 1.71727 GB/s
std::fill(p, p + n, '\0')   : 64.6559 GB/s
memset(p,0,n)               : 64.8276 GB/s

std::fill(p, p + n, 0)      : 1.58685 GB/s
std::fill(p, p + n, '\0')   : 61.0963 GB/s
memset(p,0,n)               : 61.4654 GB/s




page count: 64, volume:0.25 MB
std::fill(p, p + n, 0)      : 1.71815 GB/s
std::fill(p, p + n, '\0')   : 61.7765 GB/s
memset(p,0,n)               : 61.8078 GB/s

std::fill(p, p + n, 0)      : 1.71842 GB/s
std::fill(p, p + n, '\0')   : 62.7127 GB/s
memset(p,0,n)               : 62.3603 GB/s

std::fill(p, p + n, 0)      : 1.71836 GB/s
std::fill(p, p + n, '\0')   : 62.472 GB/s
memset(p,0,n)               : 62.2173 GB/s




page count: 128, volume:0.5 MB
std::fill(p, p + n, 0)      : 1.7184 GB/s
std::fill(p, p + n, '\0')   : 56.2666 GB/s
memset(p,0,n)               : 56.2601 GB/s

std::fill(p, p + n, 0)      : 1.71833 GB/s
std::fill(p, p + n, '\0')   : 56.6978 GB/s
memset(p,0,n)               : 56.6649 GB/s

std::fill(p, p + n, 0)      : 1.71836 GB/s
std::fill(p, p + n, '\0')   : 56.573 GB/s
memset(p,0,n)               : 56.364 GB/s




page count: 256, volume:1 MB
std::fill(p, p + n, 0)      : 1.71868 GB/s
std::fill(p, p + n, '\0')   : 52.8329 GB/s
memset(p,0,n)               : 52.7615 GB/s

std::fill(p, p + n, 0)      : 1.71873 GB/s
std::fill(p, p + n, '\0')   : 53.0107 GB/s
memset(p,0,n)               : 52.9273 GB/s

std::fill(p, p + n, 0)      : 1.7188 GB/s
std::fill(p, p + n, '\0')   : 52.9647 GB/s
memset(p,0,n)               : 52.7986 GB/s




page count: 512, volume:2 MB
std::fill(p, p + n, 0)      : 1.71876 GB/s
std::fill(p, p + n, '\0')   : 42.8373 GB/s
memset(p,0,n)               : 51.153 GB/s

std::fill(p, p + n, 0)      : 1.71887 GB/s
std::fill(p, p + n, '\0')   : 42.8891 GB/s
memset(p,0,n)               : 51.3076 GB/s

std::fill(p, p + n, 0)      : 1.71472 GB/s
std::fill(p, p + n, '\0')   : 41.648 GB/s
memset(p,0,n)               : 50.9874 GB/s




page count: 1024, volume:4 MB
std::fill(p, p + n, 0)      : 1.71864 GB/s
std::fill(p, p + n, '\0')   : 46.0203 GB/s
memset(p,0,n)               : 50.4429 GB/s

std::fill(p, p + n, 0)      : 1.71674 GB/s
std::fill(p, p + n, '\0')   : 46.1187 GB/s
memset(p,0,n)               : 50.4638 GB/s

std::fill(p, p + n, 0)      : 1.71371 GB/s
std::fill(p, p + n, '\0')   : 46.0849 GB/s
memset(p,0,n)               : 50.4442 GB/s




page count: 2048, volume:8 MB
std::fill(p, p + n, 0)      : 1.71548 GB/s
std::fill(p, p + n, '\0')   : 39.2408 GB/s
memset(p,0,n)               : 37.3318 GB/s

std::fill(p, p + n, 0)      : 1.71395 GB/s
std::fill(p, p + n, '\0')   : 39.5648 GB/s
memset(p,0,n)               : 35.4999 GB/s

std::fill(p, p + n, 0)      : 1.71443 GB/s
std::fill(p, p + n, '\0')   : 39.2916 GB/s
memset(p,0,n)               : 35.6441 GB/s




page count: 4096, volume:16 MB
std::fill(p, p + n, 0)      : 1.71407 GB/s
std::fill(p, p + n, '\0')   : 27.5775 GB/s
memset(p,0,n)               : 26.8996 GB/s

std::fill(p, p + n, 0)      : 1.71565 GB/s
std::fill(p, p + n, '\0')   : 26.858 GB/s
memset(p,0,n)               : 26.7652 GB/s

std::fill(p, p + n, 0)      : 1.71532 GB/s
std::fill(p, p + n, '\0')   : 27.0259 GB/s
memset(p,0,n)               : 27.0103 GB/s




page count: 8192, volume:32 MB
std::fill(p, p + n, 0)      : 1.71676 GB/s
std::fill(p, p + n, '\0')   : 28.292 GB/s
memset(p,0,n)               : 27.2677 GB/s

std::fill(p, p + n, 0)      : 1.7164 GB/s
std::fill(p, p + n, '\0')   : 27.8432 GB/s
memset(p,0,n)               : 27.9316 GB/s

std::fill(p, p + n, 0)      : 1.71567 GB/s
std::fill(p, p + n, '\0')   : 27.1935 GB/s
memset(p,0,n)               : 27.9651 GB/s




page count: 16384, volume:64 MB
std::fill(p, p + n, 0)      : 1.71666 GB/s
std::fill(p, p + n, '\0')   : 28.349 GB/s
memset(p,0,n)               : 28.8438 GB/s

std::fill(p, p + n, 0)      : 1.71669 GB/s
std::fill(p, p + n, '\0')   : 28.2943 GB/s
memset(p,0,n)               : 28.8646 GB/s

std::fill(p, p + n, 0)      : 1.71645 GB/s
std::fill(p, p + n, '\0')   : 28.8852 GB/s
memset(p,0,n)               : 28.4143 GB/s




page count: 32768, volume:128 MB
std::fill(p, p + n, 0)      : 1.71689 GB/s
std::fill(p, p + n, '\0')   : 28.8706 GB/s
memset(p,0,n)               : 28.9951 GB/s

std::fill(p, p + n, 0)      : 1.71699 GB/s
std::fill(p, p + n, '\0')   : 29.0425 GB/s
memset(p,0,n)               : 29.0657 GB/s

std::fill(p, p + n, 0)      : 1.71679 GB/s
std::fill(p, p + n, '\0')   : 28.936 GB/s
memset(p,0,n)               : 28.983 GB/s




page count: 65536, volume:256 MB
std::fill(p, p + n, 0)      : 1.71709 GB/s
std::fill(p, p + n, '\0')   : 29.2009 GB/s
memset(p,0,n)               : 29.171 GB/s

std::fill(p, p + n, 0)      : 1.71698 GB/s
std::fill(p, p + n, '\0')   : 29.1321 GB/s
memset(p,0,n)               : 29.1438 GB/s

std::fill(p, p + n, 0)      : 1.71703 GB/s
std::fill(p, p + n, '\0')   : 29.0658 GB/s
memset(p,0,n)               : 29.0583 GB/s




page count: 131072, volume:512 MB
std::fill(p, p + n, 0)      : 1.71721 GB/s
std::fill(p, p + n, '\0')   : 29.2061 GB/s
memset(p,0,n)               : 29.2037 GB/s

std::fill(p, p + n, 0)      : 1.71715 GB/s
std::fill(p, p + n, '\0')   : 29.222 GB/s
memset(p,0,n)               : 29.2408 GB/s

std::fill(p, p + n, 0)      : 1.7171 GB/s
std::fill(p, p + n, '\0')   : 29.2392 GB/s
memset(p,0,n)               : 29.2468 GB/s




page count: 262144, volume:1024 MB
std::fill(p, p + n, 0)      : 1.71722 GB/s
std::fill(p, p + n, '\0')   : 29.2941 GB/s
memset(p,0,n)               : 29.2648 GB/s

std::fill(p, p + n, 0)      : 1.71729 GB/s
std::fill(p, p + n, '\0')   : 29.2909 GB/s
memset(p,0,n)               : 29.2794 GB/s

std::fill(p, p + n, 0)      : 1.71732 GB/s
std::fill(p, p + n, '\0')   : 29.2682 GB/s
memset(p,0,n)               : 29.3086 GB/s




page count: 524288, volume:2048 MB
std::fill(p, p + n, 0)      : 1.71732 GB/s
std::fill(p, p + n, '\0')   : 30.3459 GB/s
memset(p,0,n)               : 30.419 GB/s

std::fill(p, p + n, 0)      : 1.71733 GB/s
std::fill(p, p + n, '\0')   : 30.3444 GB/s
memset(p,0,n)               : 30.4145 GB/s

std::fill(p, p + n, 0)      : 1.71733 GB/s
std::fill(p, p + n, '\0')   : 30.3646 GB/s
memset(p,0,n)               : 30.4114 GB/s



```
