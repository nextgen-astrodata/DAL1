#! /usr/bin/env python

## \file tLopes.py
## \brief Read in and plot LOPES raw time-series data
## \ingroup DAL

print "-- Importing modules..."

import dal
from pylab import *

print "-- Setting up layout for data display..."

font = {'fontname'	: 'Courier',
	'color'		: 'r',
	'fontweight'	: 'bold',
	'fontsize'	: 11}
title('A title', font, fontsize=12)
xlabel('Sample number', font)
ylabel('Amplitude', font)

print "-- Opening data set..."

ds = dal.dalDataset()

#ret = ds.open("/home/jmasters/data/h5/lopes.h5")
ret = ds.open("/home/lars/data/lopes/2007-01-31/2007.01.31.23:59:33.960.hdf5")

# stationGroup = ds.openGroup("Station")

print "-- Reading antenna data..."

ant0 = ds.openTable( "ANTENNA0", "Station" );
ant1 = ds.openTable( "ANTENNA1", "Station" );
ant2 = ds.openTable( "ANTENNA2", "Station" );
ant3 = ds.openTable( "ANTENNA3", "Station" );

# antennaTable.listColumns()

print "-- Filling data into plot..."

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
