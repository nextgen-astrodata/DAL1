#! /usr/bin/env python

from pydal import *
from scipy import *
import sys
from pylab import *

def downsample(vector, factor):
    """
    downsample(vector, factor):
        Downsample (i.e. co-add consecutive numbers) a short section
            of a vector by an integer factor.
    """
    if (len(vector) % factor):
        print "Lenght of 'vector' is not divisible by 'factor'=%d!" %  factor
        return 0
    newvector = reshape(vector, (len(vector)/factor, factor))
    return add.reduce(newvector, 1)

# The BeamFormed object represents the file.
#  The parameter is the name of the beam-formed file.
if (len(sys.argv) > 1):
  file = BeamFormed(sys.argv[1])
else:
  print "Please provide a beam-formed hdf5 file as input."
  sys.exit(1)

# get beam 0
beam = file.getBeam(0)

tot_samples = 512 * 128 * 2200
avg_samples = 512 * 128

for subband in range(1):
	start = 0
	xx = beam.getSubbandData_X( subband, start, tot_samples )
	yy = beam.getSubbandData_Y( subband, start, tot_samples )
	downsampled_data = array([],'f')
	for count in range(2200):
		end = start+avg_samples
		xx_intensity = abs(xx[start:end])
		yy_intensity = abs(yy[start:end])
		tot_intensity = sqrt(xx_intensity*xx_intensity + yy_intensity*yy_intensity)
		
		data = downsample( tot_intensity, 128 )
		start += avg_samples
	
		downsampled_data = concatenate( (downsampled_data, float32(data)) )

	tmpfile = "B0329+54_080313_subband%02d" % subband
	fileobj = open(tmpfile, mode='wb')
	fileobj.write(downsampled_data)
	fileobj.close()
	print "Wrote ",tmpfile

	#plot(downsampled_data)
	#show()
	#print len(downsampled_data)
	#print type(downsampled_data[0])
