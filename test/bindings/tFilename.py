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

observationID = "123456789"
description = "TBB_LBA"
filetype = dal.FileType.tbb
extension = dal.FileExtension.h5

## Test construction

print "\n[1] Testing Filename() ...\n"

file1 = dal.Filename()
file1.summary()

print "\n[2] Testing Filename(string,string,FileType,FileExtension) ...\n"

file2 = dal.Filename (observationID,description,filetype,extension)
file2.summary()

## Parameter access

file = dal.Filename()
file.summary

print "\n[3] Testing Filename::setObservationID(string) ...\n"

file.setObservationID (observationID)
file.summary()

print "\n[4] Testing Filename::setOptionalDescription(string) ...\n"

file.setOptionalDescription(description)
file.summary()

print "\n[5] Testing Filename::setFileType(FileType) ...\n"

filetype = dal.FileType.dynspec

file.setFiletype(filetype)
file.summary()

print "\n[6] Testing Filename::setExtension(FileExtension) ...\n"

extension = dal.FileExtension.fits

file.setExtension(extension)
file.summary()
