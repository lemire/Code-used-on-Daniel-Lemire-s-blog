FROM gcc:9.1
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN make clean
RUN make
CMD ["./map_neon"]
