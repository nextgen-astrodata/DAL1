#!/usr/bin/env ipython

# Program for ploting a spectrum from beam formed HDF5 files

import sys
import pydal
import scipy
import pylab

if (len(sys.argv) > 1):
  fn=sys.argv[1] #file.filename()
  file = pydal.BeamFormed(fn)
else:
  print "Please provide a beam-formed hdf5 file as input."
  sys.exit(1)

#file.summary()
beam = file.getBeam(0)
tmax=50000 #1376255
nsub=beam.nofSubbands()

spec= beam.getIntensity(0, 0, tmax)
for i in range(1, nsub):
  spec= scipy.vstack([spec, beam.getIntensity(i, 0, tmax)])

pylab.imshow(spec, aspect='auto', interpolation='nearest', vmax=1e9)
pylab.colorbar(fraction=0.07, pad=0)
pylab.xlabel('time steps')
pylab.ylabel('subband')
pylab.title(fn)
#pylab.savefig(fn+".png")
pylab.show()
