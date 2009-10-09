#!/usr/bin/env ipython

"""Program for ploting a spectrum from beam formed HDF5 files."""

import pydal, pylab, numpy, time, sys, warnings

#if (len(sys.argv) < 2): sys.exit("Usage: spectrum.py path")

dt=2     #time in seconds, 0 uses full range
imax=14    #imax=None #1e10 #limit intensity maximum, None is disabled
fmin=30; fmax=216; pixels=800;
filenames=['obs108_CS302LBA.h5','obs108_RS307HBA.h5','obs108_RS503HBA.h5']
#os.listdir('.') #sys.argv[1]

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
    
pylab.ion()
pylab.rcParams['font.size']=15
pylab.hold(False)
warnings.simplefilter("ignore",DeprecationWarning)

Spectrum=numpy.empty((fmax-fmin,pixels))
pylab.imshow(Spectrum,extent=(0,dt,fmax,fmin), \
             aspect='auto', interpolation='nearest', vmax=imax)
pylab.draw()
offset=0
for filename in filenames: 
    DDS=pydal.dalDataset(filename,"HDF5")
    samples=DDS.openTable("beam000/SB000").getNumberOfRows()
    duration=samples*DDS.getAttribute_int("DOWNSAMPLE_RATE") \
              /DDS.getAttribute_double("SAMPLE_RATE")
    if(dt):
        ns=int(samples*dt/duration)
    else:
        ns=samples

    ds=ns/pixels+1
    if ds*pixels>samples: ds=ds-1
    ns=ds*pixels
    dt=duration*ns/samples
    print "dt=",dt, ", ds=",ds, ", ns=",ns, ", ds*pixels=", ds*pixels
    File = pydal.BeamFormed(filename)
    Beam = File.getBeam(0)
    for s in range(Beam.nofSubbands()):
        tstart=time.time()
        subband=downsample(Beam.getIntensity(s,0,ns), ds)
        Spectrum[offset+s,]=numpy.sqrt(subband)
        if(s % 5):
            continue
        else:
            print 'duration:' , time.time()-tstart
            sp=pylab.imshow(numpy.log(Spectrum),extent=(0,dt,fmax,fmin), \
                            aspect='auto', interpolation='nearest', vmax=imax)
            canvas=sp.figure.canvas
            canvas.blit(sp.figure.bbox)
    offset=offset+Beam.nofSubbands()

pylab.imshow(numpy.log(Spectrum), extent=(0,dt,fmax,fmin), \
             aspect='auto', vmax=imax, interpolation='nearest')
pylab.colorbar(fraction=0.07, pad=0)
pylab.xlabel('time [s]')
pylab.ylabel('frequency [MHz]')
pylab.title(filename)
numpy.savez("preview.npz",Spectrum=Spectrum, dt=dt)
#Spectrum=numpy.load("Spectrum.npz")
#pylab.savefig(filename+".png")
print time.time()-tstart, "seconds"
pylab.show()
