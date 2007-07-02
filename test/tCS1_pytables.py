#! /usr/bin/env python

## \file tCS1_pytables.py
## \brief Use pyTables to read and plot antenna data from CS-1 data set
## \ingroup DAL

import tables
from pylab import *
h5 = tables.openFile("/home/jmasters/sb0-1.h5")
table = h5.root.MAIN
g = [ abs(x['DATA'][0]) for x in table.iterrows() if x['ANTENNA1']==0 and x['ANTENNA2']==1 ]

imshow(g[0:1000])
show()
