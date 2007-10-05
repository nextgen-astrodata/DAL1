#! /usr/bin/env python

import pydal as dal
import sys
	
# check usage
if len(sys.argv) < 6 or len(sys.argv) > 7:
	print "Usage:"
	print "\tdatacube.py <infile> <outfile> <antenna1> <antenna2> " + \
	      "<sub-band> [channel]"
	print "\t<> required"
	print "\t[] optional"
	print ""
	sys.exit(1)

# open file
msds= dal.dalDataset()
if ( msds.open(sys.argv[1]) ):
	sys.exit(1)
	
h5ds = dal.dalDataset(sys.argv[2], "HDF5")

# open table
tablename = "MAIN";
msds.setFilter("TIME, DATA","ANTENNA1 = 1 AND ANTENNA2 = 2 AND DATA_DESC_ID = " + sys.argv[5])
maintable = msds.openTable( tablename );

# get data
data_col = maintable.getColumn("DATA")
data_object = data_col.data()

dataarray = h5ds.createArray( "data", data_object );

msds.close()
h5ds.close()
