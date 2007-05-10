#! /usr/bin/env python

print "-- Importing modules..."

import sys,dal,ROOT
from pylab import *
from ROOT import gRandom,TCanvas,TH1F,TGraph,TVirtualFFT,TH2F

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

nofAntennas = 4;
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

ts   = TH1F ('ts',"Time series",blocksize,0,blocksize);
hist = TH1F ('hist','TimeSeries',100,minVal,maxVal);

print "-- Filling data into histogram..."

for sample in range(len(vals)):
	ts.SetBinContent(sample,vals[sample])
	hist.Fill(vals[sample])

c1 = TCanvas ('c1','Example',100,10,1000,500)
c1.Divide(2,1)
c1.cd(1)
ts.Draw()
c1.cd(2)
hist.SetFillColor(32)
hist.Fit("gaus")
hist.Draw()
c1.Update()

## Prevent Python from exiting before we are done

raw_input('Please press return to exit...\n')
