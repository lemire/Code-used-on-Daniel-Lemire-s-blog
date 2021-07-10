if ! type zstd > /dev/null; then
  sudo apt-get install zstd
fi
if ! type lz4 > /dev/null; then
  sudo apt-get install lz4
fi
gzip -k -f twitter.json
zstd -q -f twitter.json
lz4 -q -k -f --fast twitter.json
ls -al twitter*

git clone https://github.com/ebiggers/libdeflate.git && cd libdeflate/ &&  make && cd ..

time for i in {1..1000} ; do  libdeflate/gunzip -k -f twitter.json.gz; done
time for i in {1..1000} ; do  lz4 -q -f -d  twitter.json.lz4 ; done
time for i in {1..1000} ; do  zstd -q -f -d  twitter.json.zst ; done
time for i in {1..1000} ; do  gunzip -k -f twitter.json.gz; done
time for i in {1..1000} ; do  cp twitter.json  newtwitter.json ; done
