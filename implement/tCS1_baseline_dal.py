#! /usr/bin/env python

import dal
from pylab import *
import numpy
import sys

#import Gnuplot  # for plotting
#import Gnuplot.funcutils # for plotting
#import time # for delayed rotation of plot


import Numeric

ds= dal.dalDataset()
ds.open(sys.argv[1])
table = ds.openTable("MAIN","/")
ms = dal.msp()
row = ms.value()
d1 = []
d2 = []
d3 = []
t = []
# perhaps 128 below can be stored/replaced with a table keyword NCHANNELS
for x in range(table.getNumberOfRows()):
	table.readRows(ms,x,1)
	row = ms.value()
	if (row.get_ant1()==0 and row.get_ant2()==1):
		d1.append(abs(row.get_data(0,10).r + row.get_data(0,10).i))
	if (row.get_ant1()==0 and row.get_ant2()==2):
		d2.append(abs(row.get_data(0,10).r + row.get_data(0,10).i))
	if (row.get_ant1()==0 and row.get_ant2()==3):
		d3.append(abs(row.get_data(0,10).r + row.get_data(0,10).i))
		t.append(row.get_time())

nt = numpy.array(t)
nd1 = numpy.array(d1)
nd2 = numpy.array(d2)
nd3 = numpy.array(d3)
font = {'color'      : 'r',
        'fontweight' : 'bold'}
plot(nt,nd1,'r.')
plot(nt,nd2,'g.')
plot(nt,nd3,'b.')
xlabel('Time')
ylabel('Intensity')
title('Baseline Intensity')
show()

raw_input('Please press return to exit...\n')

