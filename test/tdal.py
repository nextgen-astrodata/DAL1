#! /usr/bin/env python2.5

import libpydal as dal
import numarray
import sys
#from pylab import *

def create_dataset( filename ):
	ds = dal.dalDataset( filename )
	return ds

#----------------------
# Groups
#----------------------

def create_arrays_group( ds ):
	groupname = "Arrays"
	print "\nCreating group: " + groupname
	array_group = ds.createGroup( groupname )
	return array_group

def create_table_group( ds ):
	groupname = "Tables"
	print "\nCreating group: " + groupname
	table_group = ds.createGroup( groupname )
	return table_group

#----------------------
# INT Arrays
#----------------------

def create_int_array_from_list( array_group ):
	print "\nCreating an INT array from a list..."
	arrayname = "intarray_list"
	dims = [2,2] # array dimensions (row-major)
	data = [1,1,2,2] # array data
	cdims = [] # no chunk dims, array size will be fixed
	int_array_list = array_group.createIntArray(arrayname,dims,data,cdims)
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

def create_int_array_from_numarray( array_group ):
	print "\nCreating an INT array from a numarray array..."
	arrayname = "intarray_numarray"
	data = numarray.array([[1,1],[2,2]]) # array data
	dims = [2,2] # array dimensions (row-major)
	# create the array with initial dimensions
	int_array_numarray = array_group.createIntArray(arrayname,dims,data)
	#int_array_numarray.extend( newdims )
	print array_group.readIntArray(arrayname)

def create_int_array_from_numarray_including_dims( array_group ):
	print "\nCreating an INT array from a numarray array " \
	  + "(including chunk dims)..."
	arrayname = "intarray_numarray_cdims"
	data = numarray.array([[1,1],[2,2]]) # array data
	dims = [2,2] # array dimensions (row-major)
	cdims = [5,5] # rank can not be greater than data rank
	# create the array with initial dimensions
	int_array_numarray = \
	  array_group.createIntArray(arrayname,dims,data,cdims)
	newdims = [4,2]
	print "Extending the " + arrayname + " array."
	int_array_numarray.extend( newdims )
	print "Writing data to the new " + arrayname + " dimensions."
	offset = [2,0] # define the offset (row-major)
	write_dims = [2,2] # define the write dimensions (row-major)
	data = numarray.array([[1,1],[2,2]]) # array data
	#int_array_numarray.write( offset, write_dims, data )
	print array_group.readIntArray(arrayname)


#----------------------
# FLOAT Arrays
#----------------------

def create_float_array_from_list( array_group ):
	print "\nCreating a FLOAT array from a list..."
	arrayname = "floatarray_list"
	dims = [2,2] # array dimensions
	data = [1,1,2,2] # array data
	cdims = [] # no chunk dims, array size will be fixed
	int_array_list = array_group.createFloatArray(arrayname,dims,data,cdims)

def create_float_array_from_numarray( array_group ):
	print "\nCreating a FLOAT array from a numarray array..."
	arrayname = "floatarray_numarray"
	dims = [2,2] # array dimensions
	cdims = [] # no chunk dims, array size will be fixed
	data = numarray.array([[1,1],[2,2]]) # array data
	float_array_numarray = \
	 array_group.createFloatArray(arrayname,dims,data,cdims)

def close_dataset( ds ):
	print "\nClosing dataset..."
	ds.close()

#----------------------
# MAIN
#----------------------

# create the file
filename = "daltest.h5"
ds = create_dataset( filename )

# groups
array_group = create_arrays_group( ds )
table_group = create_table_group( ds )

# int arrays
create_int_array_from_list( array_group )
create_int_array_from_numarray( array_group )
create_int_array_from_numarray_including_dims( array_group )

# float arrays
create_float_array_from_list( array_group )
create_float_array_from_numarray( array_group )

close_dataset( ds )




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
