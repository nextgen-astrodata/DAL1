#! /usr/bin/env python

import sys
import string
import numpy

## Check Python version ____________________________________

def check_pyversion():
        version = string.split(string.split(sys.version)[0], ".")
        if map(int, version) < [2, 5, 0]:
                print "\nSorry: Python version must be at least 2.5\n"
                sys.exit(-1)

import pydal as dal

## Run the tests ___________________________________________

shape = [1024,1024,100]
ignoreShape = True

print ignoreShape

## Construction

print "\n[1] Testing HDF5Hyperslab() ...\n"

slab1 = dal.HDF5Hyperslab()
slab1.summary()

#print "\n[2] Testing HDF5Hyperslab(std::vector<hsize_t> const &) ...\n"
#
#slab2 = dal.HDF5Hyperslab(shape)
#slab2.summary()
