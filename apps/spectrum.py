#!/usr/bin/env ipython

# Program for ploting a spectrum from beam formed HDF5 files

import sys
import pydal
import scipy
import pylab
import time

if (len(sys.argv) > 1):
  fn=sys.argv[1] #file.filename()
  file = pydal.BeamFormed(fn)
  ds=pydal.dalDataset(fn,"HDF5")
  #file.summary()
else:
  print "Please provide a beam-formed hdf5 file as input."
  sys.exit(1)

tmax=1  #time in seconds, 0 uses full range
vmax=None #1e10 #limit intensity maximum, None is disabled
interpolation='bilinear' #'bilinear'  'nearest'

tstart=time.time()
beam = file.getBeam(0)
nsub=beam.nofSubbands()
beamg=ds.openGroup("beam000")
fmin=beamg.getAttribute_int("CENTER_FREQUENCY_SB000")/1e6
fmax=beamg.getAttribute_int("CENTER_FREQUENCY_SB"+str(nsub-1).zfill(3))/1e6
ntot=ds.openTable("beam000/SB000").getNumberOfRows()
tend=ntot*ds.getAttribute_int("DOWNSAMPLE_RATE") \
         /ds.getAttribute_double("SAMPLE_RATE")
if (tmax): ns=int(ntot*tmax/tend)
else: ns=ntot

#UTC=ds.getAttribute_string("EPOCH_UTC")
#UTC_DATE=ds.getAttribute_string("EPOCH_DATE")

#print beam.ra()
#sys.exit()

for i in range(0, nsub):
  try: spec
  except NameError:    spec=beam.getIntensity(i, 0, ns)
  print "reading subband", i, ", center frequency="
  beamg.getAttribute_int("CENTER_FREQUENCY_SB"+str(i).zfill(3))
  spec=scipy.vstack([spec, beam.getIntensity(i, 0, ns)])

fs=20
params = {'axes.labelsize': fs,
          'axes.titlesize': 1.2*fs,
          'text.fontsize': fs,
          'legend.fontsize': fs,
          'xtick.labelsize': .8*fs,
          'ytick.labelsize': .8*fs}
pylab.rcParams.update(params)

pylab.imshow(pylab.log(spec), aspect='auto', extent=(0,tmax,fmax,fmin), interpolation=str(interpolation), vmax=vmax)
#interpolation='nearest', aspect=.7*ns/nsub
pylab.colorbar(fraction=0.07, pad=0)
pylab.xlabel('time [s]')
pylab.ylabel('frequency [MHz]')
pylab.title(fn)
#pylab.title("")
pylab.savefig(fn+".png")
print time.time()-tstart, "seconds"
pylab.show()

