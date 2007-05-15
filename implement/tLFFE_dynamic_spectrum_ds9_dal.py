#! /usr/bin/env python

## \file tLFFE_dynamic_spectrum_ds9_dal.py
## \brief Python script for passing WSRT LFFE data dynamic spectrum to DS9
## \ingroup DAL

import dal
from pylab import *
import numpy
import sys
import RO.DS9
import numarray
ds9Win = RO.DS9.DS9Win()

# for congrid
import numpy as n
import scipy.interpolate

ds= dal.dalDataset()
ds.open(sys.argv[1])
table = ds.openTable("MAIN","/")
lffe = dal.lffep()
row = lffe.value()
d = []
t = []

def congrid(a, nud, method='neighbour', centre=False, minusone=False):
	if not a.dtype.type in [n.typeDict['Float32'], n.typeDict['Float64']]:
		print "Converting to float"
		a = a.astype('Float32')
 
	if minusone:
		m1 = 1
	else:
		m1 = 0
	if centre:
		ofs = 0.5
	else:
		ofs = 0.

	old = n.asarray( a.shape )
	ndims = len( old )
	if len( nud ) != ndims:
		print "Congrid: dimensions error. This routine currently only support rebinning to the same number of dimensions."
		return None

	nudr = n.asarray( nud ).astype('Float32')

	dimlist = []

	if method == 'neighbour':
		for i in range( ndims ):
			base = n.indices(nudr)[i]
			dimlist.append( (old[i] - m1) / (nudr[i] - m1) \
				* (base + ofs) - ofs )

		cd = n.array( dimlist )
		cdr = cd.round().astype( 'UInt16' )
		nua = a[list( cdr )]
		return nua

	elif method in ['nearest','linear','cubic','spline']:
		# calculate new dims
		for i in range( ndims ):
			base = n.arange( nudr[i] )
			dimlist.append( (old[i] - m1) / (nudr[i] - m1) \
				* (base + ofs) - ofs )

		# specify old dims
		olddims = [n.arange(i).astype('Float32') for i in list( a.shape )]

		# first interpolation - for ndims = any
		mint = scipy.interpolate.interp1d( olddims[-1], a, kind=method )
		nua = mint( dimlist[-1] )

		trorder = [ndims - 1] + range( ndims - 1 )
		for i in range( ndims - 2, -1, -1 ):
			nua = nua.transpose( trorder )

		mint = scipy.interpolate.interp1d( olddims[i], nua, kind=method )
		nua = mint( dimlist[i] )

		if ndims > 1:
			# need one more transpose to return to original dimensions
			nua = nua.transpose( trorder )

		return nua

	else:
		print "Congrid error: Unrecognized interpolation type.\n", \
              "This routine currently only supports \'nearest\',\'linear\',\'cubic\', and \'spline\'."
		return None


def rebin(a, *args):
    shape = a.shape
    lenShape = len(shape)
    factor = asarray(shape)/asarray(args)
    evList = ['a.reshape('] + \
    ['args[%d],factor[%d],'%(i,i) for i in range(lenShape)] + \
    [')'] + ['.sum(%d)'%(i+1) for i in range(lenShape)] + \
    ['/factor[%d]'%i for i in range(lenShape)]
    print ''.join(evList)
    return eval(''.join(evList))

# perhaps 128 below can be stored/replaced with a table keyword NCHANNELS
for x in range(table.getNumberOfRows()):
	table.readRows(lffe,x,1)
	row = lffe.value()
	if (row.get_ant1()==0 and row.get_ant2()==1):
		t.append(row.get_time())
		for i in range(128):
			d.append(abs(row.get_data(0,i).r + row.get_data(0,i).i))

nd = numpy.array(d)
nd2 =  nd.reshape( nd.shape[0]/128, 128 )
#nd3 = rebin(nd2, 1000, 128)
nd3 = congrid( nd2, (1000,128), method='linear', minusone=False, centre=False)
#print type(nd3)
#print nd3.shape
#ilffehow(nd2[:1000])
#imshow(nd3)
#xlabel('Channel')
#ylabel('Sample No.')
#title('Dynamic Spectrum')
#show()
ds9Win.showArray(nd3)
raw_input('Please press return to exit...\n')


