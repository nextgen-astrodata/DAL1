#! /usr/bin/env python

import dal
from pylab import *

ds = dal.dalDataset()

ret = ds.open("/home/jmasters/data/h5/lopes.h5")

#stationGroup = ds.openGroup("Station")

antennaTable = ds.openTable( "ANTENNA0", "Station" );
#antennaTable.listColumns()

outdata = dal.shortp()
vals = []

for x in range(antennaTable.getNumberOfRows()):
	antennaTable.readRows(outdata,x,1)
	vals.append( outdata.value() )

plot(vals[40000:50000])
show()

ds.close()
