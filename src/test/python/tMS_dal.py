#! /usr/bin/env python

import pydal as dal
from pylab import *
import numarray
#import numpy
import sys

if len(sys.argv) < 2:
	print "Usage:"
	print "\tMS_dal.py <infile>"
	print "\t<> required"
	print "\t[] optional"
	print ""
	sys.exit(1)

#msds= dal.dalDataset(sys.argv[1], "MSCASA")
msds= dal.dalDataset()
if ( msds.open(sys.argv[1]) ):
	sys.exit(1)

#msds.listTables()
tablename = "MAIN";
#msds = dal.dalTable(tablename)
msds.setFilter("UVW,TIME,DATA,ANTENNA1,ANTENNA2","ANTENNA1 = 1 AND ANTENNA2 = 2")
maintable = msds.openTable( tablename );
#maintable = msds.openTable( tablename );
#maintable.listColumns()
#maintable.getName()


print "-"*5 + "\nUVW\n" + "-"*5
uvw_col = maintable.getColumn("UVW")
if ( uvw_col.isScalar() ):
	print "SCALAR"
if ( uvw_col.isArray() ):
	print "ARRAY"
print "shape: " + str(uvw_col.shape())
print "number of dims: " + str(uvw_col.ndims())
print "datatype: " + str(uvw_col.getDataType())
foo = uvw_col.data()
print foo.shape
#plot(foo[0],foo[1],'r.',-(foo[0]),-(foo[1]),'b.')
#show()
for p in range(5):
	print foo[0][p]
print '\n'




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
print type(time_data)
#plot(foo)
#show()
print '\n'

print "-"*5 + "\nANTENNA1\n" + "-"*5
a1_col = maintable.getColumn("ANTENNA1")
if ( a1_col.isScalar() ):
	print "SCALAR"
if ( a1_col.isArray() ):
	print "ARRAY"
print "shape: " + str(a1_col.shape())
print "number of dims: " + str(a1_col.ndims())
print "datatype: " + str(a1_col.getDataType())
a1_data = a1_col.data()
for p in range(5):
	print a1_data[p]
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
foo = data_col.data()
print foo.shape
foo[0].shape
for i in range(3):
	print foo[i][0][0]
#imshow( abs(foo[0]) )
#show()
print '\n'

print "SUCCESS"