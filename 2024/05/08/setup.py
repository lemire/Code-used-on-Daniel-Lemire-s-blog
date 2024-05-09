from setuptools import setup, Extension

module1 = Extension('digit_to_str', sources = ['digit_to_str.c'], extra_compile_args=['-std=c11'])

setup (name = 'digit_to_str',
        author="Daniel Lemire",
        version = '1.0.0',
        author_email='daniel@lemire.me',
        long_description = open('README.md', 'r', encoding="utf-8").read(),
        long_description_content_type = 'text/markdown',
        description = 'Fast string  generation in Python',
        ext_modules = [module1])