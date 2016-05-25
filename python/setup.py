#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Build/Install tools for pyblossom
"""

from distutils.core import setup, Extension

module = Extension(
    'pyblossom',
    sources=['./src/pyblossom.c', '../src/bloom.c'],
    libraries=[],
    include_dirs=['../src'],
    define_macros=[('NDEBUG', '1')]
)

setup(
    name="pyblossom",
    version='1.0.0',
    description="A library for Bloom filters",
    ext_modules=[module],
    author="Michael T. Schmidt",
    author_email="schmidmt@gmail.com"
)
