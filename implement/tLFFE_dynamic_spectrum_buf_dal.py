#! /usr/bin/env python

## \file tLFFE_dynamic_spectrum_buf_dal.py
## \brief Python script fro creating a dynamic spectrum from WSRT LFFE data
## \ingroup DAL

import dal # data access library
from pylab import * # for plotting
import numpy # for arrays
import sys # for arguments
from rebin import * # local rebin.py

ds = dal.dalDataset() # declare a dataset
ds.open(sys.argv[1]) # open it
table = ds.openTable("MAIN","/") # open a table
lffe = dal.lffep() # pointer to buffer of table rows
d = [] # empty data list
t = [] # empty time list

total_rows = table.getNumberOfRows()
already_read = 0
bufsize = 500

for x in range(0,total_rows,bufsize):
	if (total_rows - already_read) < bufsize:
		bufsize = total_rows - already_read
	table.readRows(lffe,x,bufsize) # read a buffer of rows
	buf = lffe.value()
	for row in range( bufsize ):
		# look for a certain baseline | antenna-pair
		if (buf.get_ant1(row)==0 and buf.get_ant2(row)==1):
			t.append(buf.get_time(row)) # add to the time list
			for i in range(128):  # for each channel
				d.append(abs(buf.get_data(row,0,i).r + buf.get_data(row,0,i).i))
	already_read += bufsize

nd = numpy.array(d)
nd2 =  nd.reshape( nd.shape[0]/128, 128 )
nd3 = congrid( nd2, (1000,128), method='linear', minusone=False, centre=False)
imshow(nd3)
xlabel('Channel')
ylabel('Sample No.')
title('Dynamic Spectrum')
show()
