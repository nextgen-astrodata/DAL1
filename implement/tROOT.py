#! /usr/bin/env python

print "-- Importing modules..."

import sys,dal,ROOT
from pylab import *
from ROOT import gRandom,TCanvas,TH1F,TGraph

## ------------------------------------------------------------------------------
## Working with DAL functionality

print "-- Opening LOPES-Event data set..."

ds = dal.dalDataset()

ret = ds.open("/home/lars/data/lopes/2007-01-31/2007.01.31.23:59:33.960.hdf5")

print "-- Extracting ADC time-series for individual antennas..."

ant0 = ds.openTable( "ANTENNA0", "Station" );
ant1 = ds.openTable( "ANTENNA1", "Station" );
ant2 = ds.openTable( "ANTENNA2", "Station" );
ant3 = ds.openTable( "ANTENNA3", "Station" );

tabs = ( ant0, ant1, ant2, ant3 );
table = ant0;
outdata = dal.shortp();
blocksize = table.getNumberOfRows();

print "-- Fill antenna data into vector..."

vals = []

for x in range(blocksize):
	table.readRows( outdata, x, 1 );
	vals.append( outdata.value() );

maxVal = max(vals)
minVal = min(vals)

## ------------------------------------------------------------------------------
## Start piping data into ROOT

c1 = TCanvas ('c1','Example',200,10,700,500)
ts = TH1F ('ts','TimeSeries',100,minVal,maxVal);

print "-- Filling data into histogram..."

for x in range(len(vals)):
	ts.Fill(vals[x])

ts.Fit("gaus")
ts.Draw()
c1.Update()

raw_input('Please press return to exit...\n')
