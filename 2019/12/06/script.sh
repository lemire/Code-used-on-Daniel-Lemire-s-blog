docker build -t condrng .
docker run --privileged --rm -it -v $(pwd):/project condrng
