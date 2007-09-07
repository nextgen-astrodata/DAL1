#! /usr/bin/env python

import libpydal as dal
from pylab import *
import numarray

# open the file
msds= dal.dalDataset(sys.argv[1], "MSCASA")

# open the table
tablename = "MAIN";
filter_string = "SELECT UVW from $1"
maintable = msds.openFilteredTable( tablename, filter_string );

# get the UVW column
uvw_column = maintable.getColumn("UVW")

# get the column data object
uvw_data_object = uvw_column.data()

# get the data from the data object
data = uvw_data_object.get()

# plot the data
title("UV coverage for" + sys.argv[1])
xlabel("U(-U)")
ylabel("V(-V)")
plot(data[0],data[1],'r.',-(data[0]),-(data[1]),'b.')
show()
