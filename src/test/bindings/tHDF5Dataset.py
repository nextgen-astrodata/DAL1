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

