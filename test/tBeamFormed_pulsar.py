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
        print "Length of 'vector' is not divisible by 'factor'=%d!" %  factor
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
start = 0
tot_samples = 1128448
for subband in range(1):

	xx = beam.getSubbandData_X( subband, start, tot_samples )
	yy = beam.getSubbandData_Y( subband, start, tot_samples )
	xx_intensity = abs(xx)
	yy_intensity = abs(yy)
	tot_intensity = sqrt(xx_intensity*xx_intensity + yy_intensity*yy_intensity)

        print tot_intensity[:10]
	#tmpfile = "B0329+54_080313_subband%02d_version_2" % subband
	#fileobj = open(tmpfile, mode='wb')
	#fileobj.write(tot_intensity)
	#fileobj.close()
	#print "Wrote ",tmpfile

	#plot(tot_intensity)
	#show()
	#print len(tot_intensity)
	#print type(tot_intensity[0])
