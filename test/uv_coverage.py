#! /usr/bin/env python

import libpydal as dal
from pylab import *
import numarray

# check usage
if ( len(sys.argv) < 2 ) or ( len(sys.argv) > 4 ):
	print "Usage:"
	print "\tuv_coverage.py <file> [antenna1] [antenna2]"
	print "\t<> required"
	print "\t[] optional"
	print ""
	sys.exit(1)

# open the file
msds= dal.dalDataset()
if ( msds.open(sys.argv[1]) ):
	sys.exit(1)

tablename = "MAIN";

if ( 3 == len(sys.argv) ):  # if a single antenna value is given
	filter_string = "SELECT UVW from $1 where ANTENNA1 = " + sys.argv[2]
	title("UV coverage for" + sys.argv[1] + "\nAntenna(" + sys.argv[2] \
	  + ")")

elif ( 4 == len(sys.argv) ):  # if two antenna values are given
	filter_string = "SELECT UVW from $1 where ANTENNA1 = " + sys.argv[2] \
  	  + " AND ANTENNA2 = " + sys.argv[3]
	title("UV coverage for" + sys.argv[1] + "\nAntenna pair(" + sys.argv[2] \
	  + "," + sys.argv[3] + ")")
  
else:  # if no antenna values are given
	filter_string = "SELECT UVW from $1"
	title("UV coverage for" + sys.argv[1] )

maintable = msds.openFilteredTable( tablename, filter_string );

# get the UVW column
uvw_column = maintable.getColumn("UVW")

# get the column data object
uvw_data_object = uvw_column.data()

# get the data from the data object
data = uvw_data_object.get()

# plot the data
xlabel("U(-U)")
ylabel("V(-V)")
plot(data[0],data[1],'r.',-(data[0]),-(data[1]),'b.')
show()
