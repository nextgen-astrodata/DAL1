#! /usr/bin/env python

import sys
from pydal import *
from pylab import *

ds = dalDataset()
ds.open( sys.argv[1] )

a1 = ds.readIntArray( sys.argv[2] )

plot(a1[25:50],"b+")

show()
