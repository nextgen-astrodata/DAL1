#! /usr/bin/env python

import dal
from pylab import *
import numpy
ds= dal.dalDataset()
ds.open("/home/jmasters/sb0-1.h5")
table = ds.openTable("MAIN","/")
ms = dal.msp()
row = ms.value()
d = []
for x in range(table.getNumberOfRows()):
	table.readRows(ms,x,1)
	row = ms.value()
	if (row.get_ant1()==0 and row.get_ant2()==1):
		for i in range(256):
			d.append(abs(row.get_data(0,i).r))

nd = numpy.array(d)
nd2 =  nd.reshape(736768/256,256)

imshow(nd2[:1000])
show()
