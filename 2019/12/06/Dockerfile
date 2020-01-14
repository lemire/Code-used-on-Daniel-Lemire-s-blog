FROM debian:unstable-slim

RUN mkdir project
WORKDIR /project
RUN apt update && apt full-upgrade -y && apt autoremove && apt clean
RUN apt-get -y install bash
RUN apt-get -y install make
RUN apt-get -y install g++

CMD make clean && make && ls && ls condrng && ./condrng
