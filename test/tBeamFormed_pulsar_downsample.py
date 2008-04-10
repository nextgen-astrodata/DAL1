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

subband = 10
start = 0
n_samples = 512 * 128

first = 1

downsampled_data = array([],'f')

for count in range(2200):
	data = beam.getSubbandData_XY( subband, start, n_samples )
	xx_intensity = abs(data[0])
	yy_intensity = abs(data[1])
	tot_intensity = sqrt(xx_intensity*xx_intensity + yy_intensity*yy_intensity)
	
	data = downsample( tot_intensity, 128 )
	start += n_samples

	downsampled_data = concatenate( (downsampled_data, float32(data)) )


print len(downsampled_data)
#plot(downsampled_data)
#show()
print type(downsampled_data[0])

tmpfile = "tmp.dat"
fileobj = open(tmpfile, mode='wb')
fileobj.write(downsampled_data)
fileobj.close()
