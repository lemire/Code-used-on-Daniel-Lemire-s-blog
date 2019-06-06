# sudo yum install docker
# sudo systemctl enable docker
# sudo systemctl start docker
# docker pull gcc
# docker build -t my-gcc-app .
# docker run --privileged my-gcc-app

FROM gcc:8.3
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN make clean
RUN make
CMD ["./shuffle"]

