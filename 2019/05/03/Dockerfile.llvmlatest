# sudo yum install docker
# sudo systemctl enable docker
# sudo systemctl start docker
# docker build -t deb -f Dockerfile.llvm .
# docker run --privileged -t deb


FROM debian:unstable
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN apt-get update
RUN apt-get install -y wget gnupg
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key| apt-key add -
RUN echo "deb http://apt.llvm.org/unstable/ llvm-toolchain main" >  /etc/apt/sources.list.d/docker.list
RUN echo "deb-src http://apt.llvm.org/unstable/ llvm-toolchain main" >> /etc/apt/sources.list.d/docker.list
RUN apt-get update
RUN apt-get install -y  make clang lldb lld
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100
RUN c++ --version
RUN cc --version
RUN make clean
RUN CXX=/usr/bin/c++ make
CMD ["./bitmapdecoding", "test"]

