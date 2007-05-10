#! /usr/bin/env python

import dal
from pylab import *
import numpy
from rebin import *
import RO.DS9
import progress
#import numarray

# -----   open the dataset  --------

print "Opening dataset..."
ds = dal.dalDataset()
ds.open("/data/h5/L2007_01576_SB4.h5")
table = ds.openTable("MAIN","/")
cs1 = dal.cs1p()

# -----   declare a few variables  --------

print "Declaring variables..."
d = []
##### s
ii2 = []
ii3 = []
##### S
t = []
a1 = []
a2 = []
u = []
v = []
#w = []
nchannels = 256
total_rows = table.getNumberOfRows()
already_read = 0
bufsize = 50

# -----   read data into memory (python arrays)  --------

pb = progress.progressBar(0,total_rows)
print "Reading and filtering data for multiple baselines..."
for x in range(0,total_rows,bufsize):
	if (total_rows - already_read) < bufsize:
		bufsize = total_rows - already_read
	table.readRows(cs1,x,bufsize)
	buf = cs1.value()
	for row in range( bufsize ):
		a1.append( buf.get_ant1( row ) )
		a2.append( buf.get_ant2( row ) )
		u.append( buf.get_u( row ) )
		v.append( buf.get_v( row ) )
#		w.append( buf.get_w( row ) )

		if (buf.get_ant1(row)==0 and buf.get_ant2(row)==1):
			t.append( buf.get_time( row ) )

			for c in range( nchannels ):
				d.append(abs(buf.get_data(row,0,c).r + buf.get_data(row,0,c).i))
	pb(x)
	already_read += bufsize
print "\n"

#sys.exit()
# -----   create plots  --------

nd = numpy.array( d )
nd2 =  nd.reshape( nd.shape[0] / nchannels, nchannels )
ndb =  nd2.transpose()

# --- baseline intensity ---
figure(1)
title("Baseline 0-1, channels 10, 50 and 100")
xlabel("time")
ylabel("intensity")
plot(t,ndb[10],'r.-')
plot(t,ndb[50],'g.-')
plot(t,ndb[100],'b.-')
show()
raw_input('Press return to continue.\n')

# --- visibility ---
figure(2)
title("CS-1 Visibility")
xlabel("U")
ylabel("V")
nu = numpy.array(u)
nv = numpy.array(v)
plot(nu,nv,'r.',-(nu),-(nv),'b.')
show()
raw_input('Press return to continue.\n')

# --- dynamic spectrum ---

figure(3)
title("Dynamic Spectrum")
xlabel("Freqency channels")
ylabel("Time")
imshow( nd2 )
show()
raw_input('Press return to continue.\n')

# --- dynamic spectrum in ds9 ---

ds9Win = RO.DS9.DS9Win()
ds9Win.showArray(nd2)
raw_input('Press return to continue.\n')

