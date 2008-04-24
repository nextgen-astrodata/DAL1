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

       n_chan = 16
       down = 8
       end = n_chan
       count = 0
       #Channelize
       while(end < 2048):
          x = numpy.zeros(n_chan)
          y = numpy.zeros(n_chan)
          i = numpy.zeros(n_chan)
	  samp = 0
	  #Downsample
          while(samp < down):
            x += numpy.roll(abs(numpy.fft.fft(data[0][end-n_chan:end])),n_chan/2)
	    y += numpy.roll(abs(numpy.fft.fft(data[1][end-n_chan:end])),n_chan/2)
            i += numpy.sqrt(x*x+y*y)
	    samp += 1
	    end += n_chan
	  print i, subband, count
	  count += 1

       #tmpfile = "B0329+54_080313_subband%02d_chan%03d" % (subband)
       #fileobj = open(tmpfile, mode='wb')
       #fileobj.write(data)
       #fileobj.close()
       #print "Wrote ",tmpfile
