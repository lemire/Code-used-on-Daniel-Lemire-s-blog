You may need to install python and pip.

If a binary wheel is available, you can install pyfusefilter like so:
```
pip3 install --only-binary=:all: pyfusefilter
````

There is a bug in the source distribution of pyfusefilter-1.1.2 which prevents successful installations.
Until this is fixed, you can install it from source:
```
git clone --recurse-submodules https://github.com/glitzflitz/pyfusefilter
cd pyfusefilter
python setup.py build_ext
python setup.py install
```