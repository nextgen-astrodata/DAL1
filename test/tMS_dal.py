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
#maintable = msds.openFilteredTable( tablename, filter_string );
maintable = msds.openTable( tablename );
#maintable.listColumns()
#maintable.getName()

time_col = maintable.getColumn("TIME")

uvw_col = maintable.getColumn("UVW")
print "UVW shape: "
uvw_col.shape()
print '\n'
print "UVW number of dims: "
uvw_col.ndims()
print '\n'

data_col = maintable.getColumn("DATA")
print "DATA shape: "
data_col.shape()
print '\n'
print "DATA number of dims: "
data_col.ndims()
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
