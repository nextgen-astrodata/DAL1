#! /usr/bin/env python

import sys
import pydal as dal
from pylab import *

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
	msds.setFilter("UVW","ANTENNA1 = " + sys.argv[2])
	title("UV coverage for" + sys.argv[1] + "\nAntenna(" + sys.argv[2] \
	  + ")")

elif ( 4 == len(sys.argv) ):  # if two antenna values are given
	msds.setFilter("UVW","ANTENNA1 = " + sys.argv[2] + " AND ANTENNA2 = " + sys.argv[3])
	title("UV coverage for" + sys.argv[1] + "\nAntenna pair(" + sys.argv[2] \
	  + "," + sys.argv[3] + ")")
  
else:  # if no antenna values are given
	msds.setFilter("UVW")
	title("UV coverage for\n" + sys.argv[1] )

maintable = msds.openTable( tablename )

# get the UVW column
uvw_column = maintable.getColumn("UVW")

# get the data from the column
data = uvw_column.data()

# plot the data
xlabel("U(-U)")
ylabel("V(-V)")
plot(data[0],data[1],'r,',-(data[0]),-(data[1]),'b,')
show()
