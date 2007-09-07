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
#filter_string = "Select UVW, TIME, ANTENNA1, ANTENNA2 from $1 where ANTENNA1 = 1 AND ANTENNA2 = 2"
filter_string = "SELECT TIME, DATA from $1 where ANTENNA1 = 1 AND ANTENNA2 = 2"
maintable = msds.openFilteredTable( tablename, filter_string );
#maintable = msds.openTable( tablename );
#maintable.listColumns()
#maintable.getName()




#print "-"*5 + "\nUVW\n" + "-"*5
#uvw_col = maintable.getColumn("UVW")
#if ( uvw_col.isScalar() ):
	#print "SCALAR"
#if ( uvw_col.isArray() ):
	#print "ARRAY"
#print "shape: " + str(uvw_col.shape())
#print "number of dims: " + str(uvw_col.ndims())
#print "datatype: " + str(uvw_col.getDataType())
#uvw_data = uvw_col.data()
#foo = uvw_data.get()
#print foo.shape
#plot(foo[0],foo[1],'r.',-(foo[0]),-(foo[1]),'b.')
#show()
#for p in range(5):
	#print foo[0][p]
#print '\n'




#print "-"*5 + "\nTIME\n" + "-"*5
#time_col = maintable.getColumn("TIME")
#if ( time_col.isScalar() ):
	#print "SCALAR"
#if ( time_col.isArray() ):
	#print "ARRAY"
#print "shape: " + str(time_col.shape())
#print "number of dims: " + str(time_col.ndims())
#print "datatype: " + str(time_col.getDataType())
#time_data = time_col.data()
##print time_data.shape
##print type(time_data)
#foo = time_data.get()
##plot(foo)
##show()
##close()

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
data_data = data_col.data()
foo = data_data.get()
print foo.shape
#for p in range(340,345):
	#print foo[2][45][p]
#for p in range(340,345):
	#print abs(foo[2][45][p])
foo[0].shape

imshow( abs(foo[0]) )
show()
print '\n'

sys.exit(8)



#mydata = data_col.data()
#print mydata.shape
#import numpy
#na = numpy.array(mydata)
##for i in range(100):
##print "data[" + str(i) + "]: " + str(na[:][0][0])
##print "data[:][0][0] " + str(na[:][0][0])
#print "sum data[:][0][0] " + str(na[:][1][0].sum())
#print "sum data[:][1][1] " + str(na[:][1][1].sum())
#print "sum data[:][1][2] " + str(na[:][1][2].sum())
#print '\n'

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
