#! /usr/bin/env python

import dal
import numarray
import sys
#from pylab import *

ds = dal.dalDataset("daltest.h5")

groupname = "Arrays"
print "\nCreating group: " + groupname
array_group = ds.createGroup( groupname )

groupname = "Tables"
print "\nCreating group: " + groupname
table_group = ds.createGroup( groupname )


#----------------------
# INT Arrays
#----------------------

print "\nCreating an INT array from a list..."
arrayname = "intarray_list"
dims = [2,2] # array dimensions
data = [1,1,2,2] # array data
cdims = [] # no chunk dims, array size will be fixed
int_array_list = array_group.createIntArray(arrayname,dims,data,cdims)
print array_group.readIntArray(arrayname)

print "\nCreating an INT array from a numarray array..."
arrayname = "intarray_numarray"
data = numarray.array([[1,1],[2,2]]) # array data
dims = [2,2] # array dimensions
# create the array with initial dimensions
int_array_numarray = array_group.createIntArray(arrayname,dims,data)
#int_array_numarray.extend( newdims )
print array_group.readIntArray(arrayname)

print "\nCreating an INT array from a numarray array (including chunk dims)..."
arrayname = "intarray_numarray_cdims"
data = numarray.array([[1,1],[2,2]]) # array data
dims = [2,2] # array dimensions
cdims = [5,5] # rank can not be greater than data rank
# create the array with initial dimensions
int_array_numarray = array_group.createIntArray(arrayname,dims,data,cdims)
#int_array_numarray.extend( newdims )
print array_group.readIntArray(arrayname)

#----------------------
#  ATTRIBUTES
#----------------------

print "\nCreating a STRING attribute..."
attrname = "test_string_attribute"
attrval = "foo"
int_array_list.setAttribute_string(attrname,attrval)

print "\nCreating an INT attribute..."
attrname = "test_int_attribute"
attrval = 5
int_array_list.setAttribute_int(attrname,attrval)

print "\nCreating a FLOAT attribute..."
attrname = "test_float_attribute"
attrval = 1.234
int_array_list.setAttribute_float(attrname,attrval)

print "\nGetting attributes of: " + arrayname
int_array_list.getAttributes()

#----------------------
# FLOAT Arrays
#----------------------

#----------------------

print "\nCreating a FLOAT array from a list..."
arrayname = "floatarray_list"
dims = [2,2] # array dimensions
data = [1,1,2,2] # array data
cdims = [] # no chunk dims, array size will be fixed
int_array_list = array_group.createFloatArray(arrayname,dims,data,cdims)

#----------------------

print "\nCreating a FLOAT array from a numarray array..."
arrayname = "floatarray_numarray"
data = numarray.array([[1,1],[2,2]]) # array data
float_array_numarray = array_group.createFloatArray(arrayname,dims,data,cdims)

#----------------------


#----------------------
# COMPLEX Arrays
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

#t1 = ds.createTable("table1","GroupA")
#t2 = ds.createTable("table2","GroupB")

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

print "\nClosing dataset..."
ds.close()
