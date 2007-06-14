#! /usr/bin/env python

## \file tCS1_visibility_dal.py
## \brief Read and plot visibilities of a CS-1 data set
## \ingroup DAL

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
u = []
v = []
t = []

# perhaps 128 below can be stored/replaced with a table keyword NCHANNELS
for x in range(table.getNumberOfRows()):
	table.readRows(ms,x,1)
	row = ms.value()
	u.append(row.get_u())
	v.append(row.get_v())
	t.append(row.get_time())
nu = numpy.array(u)
nv = numpy.array(v)

plot(nu,nv,'r.')
plot(-(nu),-(nv),'b.')
xlabel('U')
ylabel('V')
title('Visibility')
font = {'color'      : 'r',
        'fontweight' : 'bold'}
show()

#g = Gnuplot.Gnuplot()

#d = Numeric.transpose((u,v,t))
#g.title("UV coverage over time")
#g.xlabel("U (m)")
#g.ylabel("V (m)")
#g.splot(d)

#for view in range(35,70,5):
        #g('set view 60, %d' % view)
        #g.replot()
        #time.sleep(0.7)
#for view in range(35,70,5):
        #g('set view 80, %d' % view)
        #g.replot()
        #time.sleep(0.7)
raw_input('Please press return to exit...\n')

