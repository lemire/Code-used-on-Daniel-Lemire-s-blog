make clean && docker build -t deb -f Dockerfile.llvm . && docker run --privileged -t deb && docker run -t deb cat  /usr/src/myapp/bitmapdecoding.s > bitmapdecoding_clang8.s
