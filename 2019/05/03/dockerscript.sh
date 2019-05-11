make clean && docker build -t my-gcc-app . && docker run --privileged my-gcc-app && docker run -t my-gcc-app cat /usr/src/myapp/bitmapdecoding.s > bitmapdecoding.s


