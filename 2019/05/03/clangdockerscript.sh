make clean && docker build -t deb -f Dockerfile.llvm . && docker run --privileged -t deb
