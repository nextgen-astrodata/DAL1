#! /usr/bin/env python

import dal
from pylab import *
font = {'fontname'	: 'Courier',
	'color'		: 'r',
	'fontweight'	: 'bold',
	'fontsize'	: 11}
title('A title', font, fontsize=12)
xlabel('Sample number', font)
ylabel('Amplitude', font)

ds = dal.dalDataset()

#ret = ds.open("/home/jmasters/data/h5/lopes.h5")
ret = ds.open("/home/lars/data/lopes/2007-01-31/2007.01.31.23:59:33.960.hdf5")

# stationGroup = ds.openGroup("Station")

ant0 = ds.openTable( "ANTENNA0", "Station" );
ant1 = ds.openTable( "ANTENNA1", "Station" );
ant2 = ds.openTable( "ANTENNA2", "Station" );
ant3 = ds.openTable( "ANTENNA3", "Station" );

# antennaTable.listColumns()

tabs = ( ant0, ant1, ant2, ant3 )
table = ant0
outdata = dal.shortp()
vals = []
for x in range( table.getNumberOfRows() ):
	table.readRows( outdata, x, 1 )
	vals.append( outdata.value() )
plot(vals)
show()
ds.close()
print "SUCCESS"
