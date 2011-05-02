#! /usr/bin/env python

from pydal import *
import numpy
import sys
import time

# The BeamFormed object represents the file.
#  The parameter is the name of the beam-formed file.
if (len(sys.argv) > 1):
 file = BeamFormed(sys.argv[1])
else:
 print "Please provide a beam-formed hdf5 file as input."
 sys.exit(1)

# get beam 0
beam = file.getBeam(0)

# n_chan * down = 128
n_chan = 16
down = 128/n_chan
n_samples = 144441344
number_of_reads = 32
nsamples_to_read = n_samples/number_of_reads
print "Reading ",nsamples_to_read,", ",number_of_reads," times."

#for subband in range( beam.n_subbands() ):
for subband in [11]:

    start = 0

    # read from the subband
    # becuase 8 * n_samples = total_samples for this data (144441344)
    ee = []
    for xx in range(number_of_reads):

       print "read number", xx

       data = beam.getSubbandData_XY( subband, start, nsamples_to_read )

       print "done"
       start += nsamples_to_read

       end = n_chan
       #Channelize
       while(end < nsamples_to_read):
          x = numpy.zeros(n_chan)
          y = numpy.zeros(n_chan)
          i = numpy.zeros(n_chan)
	  samp = 0
	  
	  #Downsample
          while(samp < down):
            #print "[",samp,"] FFT for ",end-n_chan," to ",end
	    x += abs(numpy.fft.fft(data[0][end-n_chan:end]))
	    y += abs(numpy.fft.fft(data[1][end-n_chan:end]))
            i += numpy.sqrt(x*x+y*y)
	    samp += 1
	    end += n_chan
	  ee.append(i)
	
       print time.ctime()

    ee = numpy.float32(numpy.array(ee).transpose())

    for chan in [8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7]:
         tmpfile = "16channels_B0329+54_080313_subband%02d_chan%02d" % (subband,chan)
         fileobj = open(tmpfile, mode='wb')
         fileobj.write(ee[chan])
         fileobj.close()
         print "Wrote ",tmpfile
