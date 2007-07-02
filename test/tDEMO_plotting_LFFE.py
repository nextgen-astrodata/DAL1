#! /usr/bin/env python

import dal			# data access library
from pylab import *	# for plotting
import numpy		# for arrays
import sys			# for arguments
from rebin import *	# local rebin.py

ds = dal.dalDataset()				# declare a dataset
ds.open(sys.argv[1])				# open it
table = ds.openTable("MAIN","/")	# open a table
lffe = dal.lffep()					# pointer to buffer of table rows

#---    variable declarations  ------

d = []		# empty data list
t = []		# empty time list
a1 = []		# empty antenna1 list
a2 = []		# empty antenna2 list
u = []		# uvw vars
v = []
w = []
nchannels = 128
total_rows = table.getNumberOfRows()
already_read = 0
bufsize = 500

#---   read in data  ------

for x in range(0,total_rows,bufsize):
	if (total_rows - already_read) < bufsize:
		bufsize = total_rows - already_read
	table.readRows(lffe,x,bufsize)			# read a buffer of rows
	buf = lffe.value()
	for row in range( bufsize ):


		a1.append( buf.get_ant1( row ) )	# add to the ant1 list
		a2.append( buf.get_ant2( row ) )	# add to the ant2 list
		u.append( buf.get_u( row ) )		# add to the u list
		v.append( buf.get_v( row ) )		# add to the v list
		w.append( buf.get_w( row ) )		# add to the w list

		# look for a certain baseline | antenna-pair		
		if (buf.get_ant1(row)==0 and buf.get_ant2(row)==1):
			t.append( buf.get_time( row ) )		# add to the time list
			
			for c in range( nchannels ):		# for each channel
				d.append(abs(buf.get_data(row,0,c).r + buf.get_data(row,0,c).i))

	already_read += bufsize

nd = numpy.array( d )
# reshape / rebin data for image plotting
nd2 =  nd.reshape( nd.shape[0] / nchannels, nchannels )
nd3 = congrid( nd2, (1000, nchannels), method='linear', \
				minusone=False, centre=False)
imshow( nd3 )
show()

nu = numpy.array(u)
nv = numpy.array(v)

plot(nu,nv,'r.',-(nu),-(nv),'b.')
show()


raw_input('Press return to quit.\n')
