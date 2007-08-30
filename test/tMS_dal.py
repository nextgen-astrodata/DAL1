#! /usr/bin/env python

import libpydal as dal
from pylab import *
import numarray
#import numpy
import sys

ds= dal.dalDataset()
ds.open(sys.argv[1], "MSCASA")
table = ds.openTable("MAIN","/")
ms = dal.msp()
row = ms.value()
d = []
t = []

# perhaps 256 below can be stored/replaced with a table keyword NCHANNELS
for x in range(table.getNumberOfRows()):
	table.readRows(ms,x,1)
	row = ms.value()
	if (row.get_ant1()==0 and row.get_ant2()==1):
		t.append(row.get_time())
		for i in range(256):
			d.append(abs(row.get_data(0,i).r))

nd = numpy.array(d)
#nd2 =  nd.reshape(736768/256,256)
nd2 =  nd.reshape( nd.shape[0]/256, 256 )

#imshow(nd2[:1000])
imshow(nd2)
show()
