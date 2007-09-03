#! /usr/bin/env python

import libpydal as dal
from pylab import *
import numarray
#import numpy
import sys

msds= dal.dalDataset(sys.argv[1], "MSCASA")
#msds.listTables()
tablename = "MAIN";
#msds = dal.dalTable(tablename)
filter_string = "Select UVW, TIME, ANTENNA1, ANTENNA2, DATA from $1 where ANTENNA1 = 1 AND ANTENNA2 = 1"
maintable = msds.openFilteredTable( tablename, filter_string );
#maintable = msds.openTable( tablename );
#maintable.listColumns()
#maintable.getName()

print "-"*5 + "\nTIME\n" + "-"*5
time_col = maintable.getColumn("TIME")
if ( time_col.isScalar() ):
	print "SCALAR"
if ( time_col.isArray() ):
	print "ARRAY"
print "shape: " + str(time_col.shape())
print "number of dims: " + str(time_col.ndims())
print "datatype: " + str(time_col.getDataType())
time_data = time_col.data()
print time_data.shape
for i in range(5):
	print time_data[i]
print '\n'

print "-"*5 + "\nUVW\n" + "-"*5
uvw_col = maintable.getColumn("UVW")
if ( uvw_col.isScalar() ):
	print "SCALAR"
if ( uvw_col.isArray() ):
	print "ARRAY"
print "shape: " + str(uvw_col.shape())
print "number of dims: " + str(uvw_col.ndims())
print "datatype: " + str(uvw_col.getDataType())
#uvw_col.data()
print '\n'

print "-"*5 + "\nDATA\n" + "-"*5
data_col = maintable.getColumn("DATA")
if ( data_col.isScalar() ):
	print "SCALAR"
if ( data_col.isArray() ):
	print "ARRAY"
print "shape: " + str(data_col.shape())
print "number of dims: " + str(data_col.ndims())
print "datatype: " + str(data_col.getDataType())
#data_col.data()
print '\n'

#ms = dal.msp()
#row = ms.value()
#d = []
#t = []

## perhaps 256 below can be stored/replaced with a table keyword NCHANNELS
#for x in range(table.getNumberOfRows()):
	#table.readRows(ms,x,1)
	#row = ms.value()
	#if (row.get_ant1()==0 and row.get_ant2()==1):
		#t.append(row.get_time())
		#for i in range(256):
			#d.append(abs(row.get_data(0,i).r))

#nd = numpy.array(d)
##nd2 =  nd.reshape(736768/256,256)
#nd2 =  nd.reshape( nd.shape[0]/256, 256 )

##imshow(nd2[:1000])
#imshow(nd2)
#show()
