
## \file tDEMO_paste_CS1.py
## \brief Read and plot data from a CS-1 data set
## \ingroup DAL

import dal
from pylab import *
import numpy
from rebin import *

# -----   open the dataset  --------

ds = dal.dalDataset()
ds.open("/data/h5/L2007_01576_SB4.h5")
table = ds.openTable("MAIN","/")
cs1 = dal.cs1p()

# -----   declare a few variables  --------

d = []
t = []
a1 = []
a2 = []
u = []
v = []
w = []
nchannels = 256
total_rows = table.getNumberOfRows()
already_read = 0
bufsize = 500

# -----   read data into memory (python arrays)  --------

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
w.append( buf.get_w( row ) )

if (buf.get_ant1(row)==0 and buf.get_ant2(row)==1):
t.append( buf.get_time( row ) )

for c in range( nchannels ):
d.append(abs(buf.get_data(row,0,c).r + buf.get_data(row,0,c).i))

already_read += bufsize

# -----   create plots  --------

nd = numpy.array( d )
nd2 =  nd.reshape( nd.shape[0] / nchannels, nchannels )
ndb =  nd2.transpose()
figure(1)
plot(t,ndb[10],'r.-')
plot(t,ndb[50],'g.-')
plot(t,ndb[100],'b.-')
nd3 = congrid( nd2, (1000, nchannels), method='linear', minusone=False, centre=False)
figure(2)
imshow( nd3 )
nu = numpy.array(u)
nv = numpy.array(v)
figure(3)
plot(nu,nv,'r.',-(nu),-(nv),'b.')
show()
