make clean && docker build -t mygcc8 . -f Dockerfile.gcc8 && docker run --privileged mygcc8 && docker run -t mygcc8 cat /usr/src/myapp/bitmapdecoding.s > bitmapdecoding_gcc8.s


