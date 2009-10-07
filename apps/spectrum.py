#!/usr/bin/env ipython

"""Program for ploting a spectrum from beam formed HDF5 files."""

import pydal, pylab, numpy, time, sys, warnings

if (len(sys.argv) < 2): sys.exit("Name to beam-formed.h5 file needed")

dt=100     #time in seconds, 0 uses full range
imax=None #imax=None #1e10 #limit intensity maximum, None is disabled
subbands=248; pixels=800; spectrum=numpy.empty((subbands,pixels))

def config():
  pylab.ion()
  pylab.rcParams['font.size'] = 18
  pylab.hold(False)
  pylab.draw()
  warnings.simplefilter("ignore",DeprecationWarning)
config()

def downsample(vector, n):
    """
    Sum up n neighboring elements and return a new vector of length/n.
    Prerequisite: the length of vector/n has to be integer.
    """
    if (len(vector) % n):
        print "Length of 'vector' has to be an integer multiple of 'n'=%d!" % n
        return 0
    newvector = numpy.reshape(vector, (len(vector)/n, n))
    return numpy.add.reduce(newvector, 1)

filename=sys.argv[1]
file = pydal.BeamFormed(filename)
Beam = file.getBeam(0)
DDS=pydal.dalDataset(filename,"HDF5")
BG=DDS.openGroup("beam000")
fmin=BG.getAttribute_int("CENTER_FREQUENCY_SB000")[0]/1e6
fmax=240 #BG.getAttribute_int("CENTER_FREQUENCY_SB"+
         #                str(Beam.nofSubbands()-1).zfill(3))/1e6
samples=DDS.openTable("beam000/SB000").getNumberOfRows()
duration=samples*DDS.getAttribute_int("DOWNSAMPLE_RATE") \
         /DDS.getAttribute_double("SAMPLE_RATE")
if(dt): ns=int(samples*dt/duration)
else: ns=samples;

ds=ns/pixels+1
if ds*pixels>samples: ds=ds-1
ns=ds*pixels
dt=duration*ns/samples
print "fmin=", fmin, fmax, dt, ds, ns, ns*pixels

for s in range(Beam.nofSubbands()):
  tstart=time.time()
  sb=downsample(Beam.getIntensity(s,0,ns), ds)
  spectrum[s,]=numpy.sqrt(sb)
  if(s % 5): continue
  print 'duration:' , time.time()-tstart
  sp=pylab.imshow(numpy.log(spectrum),extent=(0,dt,fmax,fmin), aspect='auto', \
                  interpolation='nearest', vmax=imax)
  canvas=sp.figure.canvas
  canvas.blit(sp.figure.bbox)


pylab.imshow(numpy.log(spectrum), extent=(0,dt,fmax,fmin), aspect='auto', \
             vmax=imax, interpolation='nearest')
#aspect=.7*ns/nsub
pylab.colorbar(fraction=0.07, pad=0)
pylab.xlabel('time [s]')
pylab.ylabel('frequency [MHz]')
pylab.title(filename)
#pylab.savefig(filename+".png")
print time.time()-tstart, "seconds"
pylab.show()
