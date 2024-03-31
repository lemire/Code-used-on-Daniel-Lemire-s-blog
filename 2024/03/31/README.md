You may need to install python and pip.

If a binary wheel is available, you can install pyxorfilter like so:
```
pip3 install --only-binary=:all: pyxorfilter
````

There is a bug in the source distribution of pyxorfilter-1.1.2 which prevents successful installations.
Until this is fixed, you can install it from source:
```
git clone --recurse-submodules https://github.com/glitzflitz/pyxorfilter
cd pyxorfilter
python setup.py build_ext
python setup.py install
```