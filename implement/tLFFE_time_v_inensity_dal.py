#! /usr/bin/env python

import dal
from pylab import *
import numpy
import sys

ds= dal.dalDataset()
ds.open(sys.argv[1])
table = ds.openTable("MAIN","/")
lffe = dal.lffep()
row = lffe.value()
d = []
t = []

# perhaps 128 below can be stored/replaced with a table keyword NCHANNELS
for x in range(table.getNumberOfRows()):
	table.readRows(lffe,x,1)
	row = lffe.value()
	if (row.get_ant1()==0 and row.get_ant2()==1):
		t.append(row.get_time())
		d.append(abs(row.get_data(0,2).r + row.get_data(0,2).i))

nt = numpy.array(t)
nd = numpy.array(d)

plot(nt,log(nd),'g.-')
show()
