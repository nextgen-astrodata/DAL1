#! /usr/bin/env python

import libpydal as dal
from pylab import *
import numarray
	
if len(sys.argv) < 5:
	print "Usage:"
	print "\tbaseline_intensity.py <file> <antenna1> <antenna2> <sub-band>"
	print ""
	sys.exit(1)

msds= dal.dalDataset(sys.argv[1], "MSCASA")
tablename = "MAIN";
#filter_string = \
 #"SELECT TIME, DATA from $1 where " + \
  #"ANTENNA1 = " + sys.argv[2] + " AND ANTENNA2 = " + sys.argv[3] + " OR " + \
  #"ANTENNA2 = " + sys.argv[2] + " AND ANTENNA1 = " + sys.argv[3]
filter_string = "SELECT TIME, DATA from $1 where " \
  + "ANTENNA1 = 1 AND ANTENNA2 = 2 AND DATA_DESC_ID = " + sys.argv[4]
maintable = msds.openFilteredTable( tablename, filter_string );

time_col = maintable.getColumn("TIME")
time_data = time_col.data()
time = time_data.get()

data_col = maintable.getColumn("DATA")
data_data = data_col.data()
data = data_data.get()
nchannels = data.shape[1] # second element of the data shape i.e. (4,256,nrows)
for channel in range(data.shape[1]):
	plot( time, abs(data[0][channel]), "." )
title("Time vs. Amplitude, Baseline " + \
  sys.argv[2] + '-' + sys.argv[3] + ", Sub-band(" + sys.argv[4] + ") " + \
  str(data.shape[1]) + " channels" + '\n' + sys.argv[1] )
xlabel("time")
ylabel("intensity")
gca().set_yscale("log")
show()
