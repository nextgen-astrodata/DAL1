#! /usr/bin/env python

from pydal import *
import numpy
import sys

# The BeamFormed object represents the file.
#  The parameter is the name of the beam-formed file.
if (len(sys.argv) > 1):
 file = BeamFormed(sys.argv[1])
else:
 print "Please provide a beam-formed hdf5 file as input."
 sys.exit(1)

# get beam 0
beam = file.getBeam(0)
for subband in range( beam.n_subbands() ):

       data = beam.getSubbandData_XY( subband, 0, -1 )

       c = numpy.column_stack( ( numpy.byte(data[0].real), \
       numpy.byte(data[0].imag), numpy.byte(data[1].real), \
       numpy.byte(data[1].imag) ) )

       tmpfile = "B0329+54_080313_subband%02d" % (subband)
       fileobj = open(tmpfile, mode='wb')
       fileobj.write(c.flatten())
       fileobj.close()
       print "Wrote ",tmpfile
