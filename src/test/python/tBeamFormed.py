#! /usr/bin/env python

from pydal import *
import sys

# The BeamFormed object represents the file.
#  The parameter is the name of the beam-formed file.
if (len(sys.argv) > 1):
  file = BeamFormed(sys.argv[1])
else:
  print "Please provide a beam-formed hdf5 file as input."
  sys.exit(1)

# Here's a print to screen summary of the file.
print "[1] ---------------------------------"
file.summary()

print "[2] ---------------------------------"
print file.beams()

print "[3] ---------------------------------"
print file.source()

# get beam 0
beam = file.getBeam(0)

# show the beam pointing direction
print "[4] ---------------------------------"
print beam.ra()
print beam.dec()
beam.summary()

# get X complex data for subband 0, start 0, length 10
# returns a numpy array
sb0x = beam.getSubbandData_X( 0, 0, 10 )
print "[5] ---------------------------------"
print sb0x

# get Y complex data for subband 0, start 0, length 10
# returns a numpy array
sb0y = beam.getSubbandData_Y( 0, 0, 10 )
print "[6] ---------------------------------"
print sb0y

# get X and Y complex data for subband 0, start 0, length 10
# returns a numpy array
sb0xy = beam.getSubbandData_XY( 0, 0, 10 )
print "[7] ---------------------------------"
print sb0xy
