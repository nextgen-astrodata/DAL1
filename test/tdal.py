#! /usr/bin/env python

import dal
import numarray
#from pylab import *

ds = dal.dalDataset("foo.h5")

g1 = ds.createGroup("GroupA")
g2 = ds.createGroup("GroupB")

t1 = ds.createTable("table1","GroupA")
t2 = ds.createTable("table2","GroupB")

#----------------------

print "Creating an INT array from a list..."

arrayname = "intarray_list"
dims = [2,2] # array dimensions
data = [1,1,2,2] # array data
cdims = [] # no chunk dims, array size will be fixed
int_array_list = ds.createIntArray(arrayname,dims,data,cdims)

print ds.readIntArray(arrayname)

#----------------------

print "Creating an INT array from a numarray array..."

arrayname = "intarray_numarray"
data = numarray.array([[1,1],[2,2]]) # array data
int_array_numarray = ds.createIntArray(arrayname,dims,data,cdims)

print ds.readIntArray(arrayname)

#----------------------

print "Creating a FLOAT array from a list..."

arrayname = "floatarray_list"
dims = [2,2] # array dimensions
data = [1,1,2,2] # array data
cdims = [] # no chunk dims, array size will be fixed
int_array_list = ds.createFloatArray(arrayname,dims,data,cdims)

#----------------------

print "Creating a FLOAT array from a numarray array..."

arrayname = "floatarray_numarray"
data = numarray.array([[1,1],[2,2]]) # array data
float_array_numarray = ds.createFloatArray(arrayname,dims,data,cdims)

#----------------------

#print "Creating a COMPLEX array from a list..."

#arrayname = "complexarray_list"
#dims = [2,2] # array dimensions
#data = [1,1,2,2] # array data
#cdims = [] # no chunk dims, array size will be fixed
#int_array_list = ds.createComplexArray(arrayname,dims,data,cdims)

#----------------------

#print "Creating a COMPLEX array from a numarray array..."

#arrayname = "complexarray_numarray"
#data = numarray.array([[1,1],[2,2]]) # array data
#float_array_numarray = ds.createComplexArray(arrayname,dims,data,cdims)






#--------------------------------------
#--------------------------------------

#t1.addColumn("col1", dal.dal_INT )
#t1.addColumn("col2", dal.dal_FLOAT )
#t1.addColumn("col3", dal.dal_FLOAT )
##t1.listColumns()

#t2.addColumn("data", dal.dal_INT )
#t2.listColumns()

#pdata = dal.intp() # pointer to data

#for x in range(100):
	#pdata.assign(x) # assign poniter a value
	#t2.appendRow(pdata) # write to table

#outdata = dal.intp() # pointer to data

## read some values
#vals = []
#for x in range(10,30,2): # 10 to 30, step 2
	#t2.readRows(outdata,x,1)
	##print outdata.value()
	#vals.append( outdata.value() )

#g1.setAttribute_int("intAttr",5)
#g1.setAttribute_string("strAttr","teststring")
#g1.setAttribute_uint("uintAttr",6)
#g1.setAttribute_double("doubleAttr",4.321)

#plot([1,2,3,4,5,6,7,8,9,10],vals)
#show()

ds.close()
