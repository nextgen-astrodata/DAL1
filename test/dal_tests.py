#! /usr/bin/env python

import pydal
import unittest
import os
import numpy

BIGNUM = 200

#A testcase is created by subclassing unittest.TestCase. The three individual tests are defined with methods whose names start with the letters test. This naming convention informs the test runner about which methods represent tests.
#
#The crux of each test is a call to assertEqual() to check for an expected result; assert_() to verify a condition; or assertRaises() to verify that an expected exception gets raised. These methods are used instead of the assert statement so the test runner can accumulate all test results and produce a report.

# ------------------------------------------------------------------ dalDataset
class dalDataset_tests(unittest.TestCase):

    def test_dataset_create(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
	aa = ds
	ds.close()
    	self.assertEqual(type(aa),pydal.dalDataset)

    def test_dataset_open(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
	aa = ds
	ds.close()
    	self.assertEqual(type(aa),pydal.dalDataset)
    
    def test_dataset_createIntArray_with_list(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	arr = ds.createIntArray("intarray1",[2,2],[1,2,3,4])
	ds.close()
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createIntArray_with_list_and_chunk_dimensions(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	arr = ds.createIntArray("intarray1_cdims",[2,2],[1,2,3,4],[2,2])
	ds.close()
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createIntArray_with_numpy_array(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	arr = ds.createIntArray("intarray1_numpy_array",[2,2],numpy.array([1,2,3,4]))
	ds.close()
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createIntArray_with_numpy_array_and_chunk_dimensions(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	arr = ds.createIntArray("intarray1_numpy_array_cdims",[2,2],numpy.array([1,2,3,4]),[2,2])
	ds.close()
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createFloatArray_with_list(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	arr = ds.createFloatArray("floatarray1",[2,2],[1.0,2.0,3.0,4.0],[2,2])
	ds.close()
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createFloatArray_with_numpy_array(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	arr = ds.createFloatArray("floatarray1_numpy_array",[2,2],numpy.array([1.0,2.0,3.0,4.0]),[2,2])
	ds.close()
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createGroup(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.createGroup("newgroup")
	ds.close()
    	self.assertEqual(type(group),pydal.dalGroup)

    def test_dataset_createSubGroup(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	subgroup = ds.createGroup("newgroup/subgroup")
	ds.close()
    	self.assertEqual(type(subgroup),pydal.dalGroup)

    def test_dataset_createTable(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	tab = ds.createTable("table1")
    	tab = ds.createTable("table2")
    	tab = ds.createTable("table3")
	ds.close()
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_createTable_in_group(self):  # depends on test_createGroup
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	tab = ds.createTable("table1","newgroup")
	ds.close()
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_listTables(self):  # depends on test_createTable
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	tabs = ds.listTables()
	ds.close()
    	self.assertEqual(tabs,[1])

    def test_dataset_setAttribute_char(self):	#char
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_char("char_attribute",'a')
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_char_list(self):		# char
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_char("char_list_attribute",['a','b','c'])
	ds.close()
        self.assertTrue(ret)

    def test_dataset_setAttribute_short(self):	# short
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_short("short_attribute",-3872)
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_short_list(self):		# short
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_short("short_list_attribute",[-1,0,1,2,3])
	ds.close()
        self.assertTrue(ret)

    def test_dataset_setAttribute_int(self):		# int
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_int("int_attribute",-23637)
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_int_list(self):		# int
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_int("int_list_attribute",[-1,0,1,2,3])
	ds.close()
        self.assertTrue(ret)

    def test_dataset_setAttribute_uint(self):	# uint
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_uint("uint_attribute",92834)
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_uint_list(self):		# uint
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_uint("uint_list_attribute",[1,2,3])
	ds.close()
        self.assertTrue(ret)

    def test_dataset_setAttribute_long(self):	# long
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_long("long_attribute",239822873648376436)
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_long_list(self):		# long
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_long("long_list_attribute",[932432432432732,823789,987987])
	ds.close()
        self.assertTrue(ret)

    def test_dataset_setAttribute_float(self):	# float
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_float("float_attribute",87323.234)
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_float_list(self):	# float
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_float("float_list_attribute",[1.111,2.222,3.333])
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_getAttribute_float_list(self):	# float
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	a = numpy.array([1.111,2.222,3.333])
    	b = ds.getAttribute_float("float_list_attribute")
    	# should be updated to compare expected with returned vals,
    	#  but needs the same precision.  Otherwise assertEqual fails.
	ds.close()
    	self.assertTrue(type(b)==numpy.ndarray)
        
    def test_dataset_setAttribute_double(self):	# double
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_double("double_attribute",324.9287364)
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_double_list(self):	# double
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_double("double_list_attribute",[324.9287364,2342343.2343,890897.2433])
	ds.close()
        self.assertTrue(ret)
        
    def test_dataset_setAttribute_string(self):	# string
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_string("string_attribute","hello world")
	ds.close()
        self.assertTrue(ret)

    def test_dataset_setAttribute_string_list(self):	# string
	ds = pydal.dalDataset("testfile.h5","HDF5")
	ret = ds.setAttribute_string("string_list_attribute",["list","of","strings"])
	ds.close()
        self.assertTrue(ret)

    def test_dataset_getType(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
	tt = ds.getType()
	ds.close()
    	self.assertEqual(tt,"HDF5")

    def test_dataset_openTable(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	tab = ds.openTable("table1")
	ds.close()
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_openTable_in_group(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	tab = ds.openTable("table1","newgroup")
	ds.close()
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_openGroup(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
	ds.close()
    	self.assertEqual(type(group),pydal.dalGroup)

    def test_dataset_openArray(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	array = ds.openArray("intarray1")
	ds.close()
    	self.assertEqual(type(array),pydal.dalArray)

    def test_dataset_readIntArray(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	a = ds.readIntArray("intarray1")
    	b = numpy.array([[1,2],[3,4]])
	ds.close()
    	self.assertTrue((a==b).all())
    	
    def test_dataset_readFloatArray(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	a = ds.readFloatArray("floatarray1")
    	b = numpy.array([[1.0,2.0],[3.0,4.0]])
	ds.close()
        self.assertTrue((a==b).all())

    def test_dataset_setFilter(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
	filt = ds.setFilter()
	ds.close()
	self.assertEqual(type(filt),pydal.dalFilter)

    def test_dataset_close(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	self.assertTrue(ds.close())
    	
# ------------------------------------------------------------------ dalGroup
class dalGroup_tests(unittest.TestCase):
		
    def test_group_createShortArray(self):  # depends on createGroup
	ds = pydal.dalDataset("testfile.h5","HDF5")
        group = ds.openGroup("newgroup")
        arr = group.createShortArray("shortarray1",[2,2],[1,2,3,4])
        self.assertEqual(type(group),pydal.dalGroup)

    def test_group_createIntArray(self):  # depends on createGroup
	ds = pydal.dalDataset("testfile.h5","HDF5")
        group = ds.openGroup("newgroup")
        arr = group.createIntArray("intarray1",[2,2],[1,2,3,4])
        self.assertEqual(type(group),pydal.dalGroup)

    def test_group_getName(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
	group = ds.openGroup("newgroup")
	self.assertEqual(group.getName(),"newgroup")

    def test_group_setAttribute_char(self):	#char
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_char("char_attribute",'a'))
        
    def test_group_setAttribute_char_list(self):		# char
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_char("char_list_attribute",['a','b','c']))

    def test_group_setAttribute_short(self):	# short
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_short("short_attribute",-3872))
        
    def test_group_setAttribute_short_list(self):		# short
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_short("short_list_attribute",[-3872, 8, 3472]))

    def test_group_setAttribute_int(self):		# int
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_int("int_attribute",-23637))
        
    def test_group_setAttribute_int_list(self):		# int
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_int("int_list_attribute",[-23637, 888, 21638]))

    def test_group_setAttribute_uint(self):	# uint
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_uint("uint_attribute",92834))
        
    def test_group_setAttribute_uint_list(self):		# uint
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_uint("uint_list_attribute",[92834, 888, 21638]))

    def test_group_setAttribute_long(self):	# long
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_long("long_attribute",2398226))
        
    def test_group_setAttribute_long_list(self):		# long
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_long("long_list_attribute",[2398226, 29398226, 76767676]))

    def test_group_setAttribute_float(self):	# float
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_float("float_attribute",87323.234))
        
    def test_group_setAttribute_float_list(self):		# float
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_float("float_list_attribute",[87323.234, -65656.99999, 777777.666]))

    def test_group_setAttribute_double(self):	# double
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_double("double_attribute",324.9287364))
        
    def test_group_setAttribute_double_list(self):		# double
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_double("double_list_attribute",[324.9287364, -65656.9999999992, 777777.66666661]))

    def test_group_setAttribute_string(self):	# string
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_string("string_attribute","hello world"))

    def test_group_setAttribute_string_list(self):		# string
    	group = ds.openGroup("newgroup")
        self.assertTrue(group.setAttribute_string("string_list_attribute",["hello world", "this is string two", "good bye"]))

    # getAttribute
    def test_group_getAttribute_string(self):	# string
    	group = ds.openGroup("newgroup")
    	self.assertEqual(group.getAttribute("string_attribute"),"hello world")

class dalTable_tests(unittest.TestCase):

#    def setUp(self):

# ------------------------------------------------------------------ dalTable

    def test_table_setAttribute_char(self):	#char
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_char("char_attribute",'a')
	ds.close()
    	self.assertTrue(ret)

    def test_table_setAttribute_char_list(self):	# list of chars
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_char("char_attribute_list",['a, b, c, d'])
	ds.close()
    	self.assertTrue(ret)

    def test_table_setAttribute_short(self):	# short
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_short("short_attribute",-3872)
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_short_list(self):	# list of shorts
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_short("short_attribute_list",[-3872, -330, 3840])
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_int(self):		# int
	ds = pydal.dalDataset("testfile.h5","HDF5")
	table = ds.openTable("table1")
	ret = table.setAttribute_int("int_attribute",123456)
	ds.close()
    	self.assertTrue(ret)

    def test_table_setAttribute_int_list(self):		# list of ints
	ds = pydal.dalDataset("testfile.h5","HDF5")
	table = ds.openTable("table1")
	ret = table.setAttribute_int("int_attribute_list",[-23637, 0, 24898, 665557])
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_uint(self):	# uint
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_uint("uint_attribute",92834)
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_uint_list(self):	# list of uints
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_uint("uint_attribute_list",[92834, 6646, 333399])
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_long(self):	# long
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_long("long_attribute",2398226)
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_long_list(self):	# list of longs
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_long("long_attribute_list",[2398226, 49895458, 5895869586958989])
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_float(self):	# float
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_float( "float_attribute", 87323.234 )
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_float_list(self):	# list of floats
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_float( "float_attribute_list", [87323.234, 998989.77777, -929292.521] )
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_double(self):	# double
	ds = pydal.dalDataset("testfile.h5","HDF5")
	table = ds.openTable("table1")
	ret = table.setAttribute_double( "double_attribute", 324.9287364 )
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_double_list(self):	# list of doubles
	ds = pydal.dalDataset("testfile.h5","HDF5")
	table = ds.openTable("table1")
	ret = table.setAttribute_double( "double_attribute_list", [324.9287364, -44994.498594895844, 88.77e208] )
	ds.close()
    	self.assertTrue(ret)
        
    def test_table_setAttribute_string(self):	# string
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_string("string_attribute","hello world")
	ds.close()
    	self.assertTrue(ret)

    def test_table_setAttribute_string_list(self):	# list of strings
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_string("string_attribute_list",["hello world", "this is the second string", "and another"])
	ds.close()
    	self.assertTrue(ret)

    # getAttribute
    def test_table_getAttribute_string(self):	# string
	ds = pydal.dalDataset("testfile.h5","HDF5")
	table = ds.openTable("table1")
	attrval = table.getAttribute("string_attribute")
	ds.close()
    	self.assertEqual(attrval,"hello world")
    	
    def test_table_addColumns(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
    	table.addColumn("col1", "dalINT", 1 )
    	table.addColumn("col2", "dalFLOAT", 1 )
    	table.addColumn("col3", "dalSHORT", 1 )
    	t2 = ds.openTable("table2")
    	t2.addColumn("col1", "dalFLOAT", 1 )
    	t2.addColumn("col2", "dalFLOAT", 1 )
    	t2.addColumn("col3", "dalFLOAT", 1 )
    	t3 = ds.openTable("table3")
    	t3.addColumn("col1", "dalSHORT", 1 )
    	t3.addColumn("col2", "dalFLOAT", 1 )
    	t3.addColumn("col3", "dalINT", 1 )
    	cols = table.listColumns()
	ds.close()
    	self.assertEqual(cols,['col1','col2','col3'])
    	
    def test_table_appendRow(self):  # modifies table1
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
    	for x in range(BIGNUM):
    	  ret = table.appendRow([6,2.6,36])
	ds.close()
    	self.assertTrue(ret)

    def test_table_appendRows(self):  # modifies table2
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table2")
    	ret = table.appendRows([ 1.0 for x in range(BIGNUM*3)],BIGNUM)
	ds.close()
    	self.assertTrue(ret)

    def test_table_write_col_data_by_index(self):  # modifies table3
	ds = pydal.dalDataset("testfile.h5","HDF5")
        t3 = ds.openTable("table3")
	# first create the rows in the table with appendRows
	ret = t3.appendRows([0 for x in range(BIGNUM*3)],BIGNUM)
        # then fill them with data using write_col_by_index
    	t3.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.int16),0,0,BIGNUM)
    	t3.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.float32),1,0,BIGNUM)
    	t3.write_col_by_index_boost(numpy.random.randint(0,100,BIGNUM),2,0,BIGNUM)
	ds.close()
    	self.assertTrue(t3)

    def test_table_getNumberOfRows(self): # depends on test_table_appendRows
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	num_rows = table.getNumberOfRows()
	ds.close()
    	self.assertEqual(num_rows,BIGNUM)

    def test_table_readRows(self):
	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
    	aa = table.readRows(0,1)
    	print aa
	ds.close()
    	self.assertTrue()


class dalArray_tests(unittest.TestCase):

#    def setUp(self):

# ------------------------------------------------------------------ dalArray

    def test_array_setAttribute_char(self):	#char
        #arr = ds.createIntArray("intarray1",[2,2],[1,2,3,4])
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_char("char_attribute",'a'))
        
    def test_array_setAttribute_char_list(self):		# char
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_char("char_list_attribute",['a','b','c']))

    def test_array_setAttribute_short(self):	# short
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_short("short_attribute",-3872))
        
    def test_array_setAttribute_short_list(self):		# short
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_short("short_list_attribute",[-3872, 8, 3472]))

    def test_array_setAttribute_int(self):		# int
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_int("int_attribute",-23637))
        
    def test_array_setAttribute_int_list(self):		# int
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_int("int_list_attribute",[-23637, 888, 21638]))

    def test_array_setAttribute_uint(self):	# uint
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_uint("uint_attribute",92834))
        
    def test_array_setAttribute_uint_list(self):		# uint
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_uint("uint_list_attribute",[92834, 888, 21638]))

    def test_array_setAttribute_long(self):	# long
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_long("long_attribute",9999999999999999992398226))
        
    def test_array_setAttribute_long_list(self):		# long
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_long("long_list_attribute",[9999999999999999992398226, 29398226, 76767676]))

    def test_array_setAttribute_float(self):	# float
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_float("float_attribute",87323.234))
        
    def test_array_setAttribute_float_list(self):		# float
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_float("float_list_attribute",[87323.234, -65656.99999, 777777.666]))

    def test_array_setAttribute_double(self):	# double
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_double("double_attribute",324.9287364))
        
    def test_array_setAttribute_double_list(self):		# double
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_double("double_list_attribute",[324.9287364, -65656.9999999992, 777777.66666661]))

    def test_array_setAttribute_string(self):	# string
	arr = ds.openArray( "intarray1" )
    	self.assertTrue(arr.setAttribute_string("string_attribute","hello world"))

    def test_array_setAttribute_string_list(self):		# string
	arr = ds.openArray( "intarray1" )
        self.assertTrue(arr.setAttribute_string("string_list_attribute",["hello world", "this is string two", "good bye"]))

    def test_array_close(self):
        arr = ds.openArray("intarray1")
        self.assertTrue(arr.close());



#    def tearDown(self):

if __name__ == "__main__":

# uncomment the following line to run all tests
#	unittest.main()

# uncomment the following lines to run all test in MyTests
# the order of the tests is alphabetical
#	suite = unittest.TestLoader().loadTestsFromTestCase(MyTests)
#	unittest.TextTestRunner(verbosity=2).run(suite)
	

	# delete the test file if it already exists
	cmd = 'rm -f testfile.h5'
	os.system(cmd)

	dataset_test_suite = unittest.TestSuite()
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_create"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_open"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createIntArray_with_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createIntArray_with_list_and_chunk_dimensions"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createIntArray_with_numpy_array"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createIntArray_with_numpy_array_and_chunk_dimensions"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createFloatArray_with_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createFloatArray_with_numpy_array"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createGroup"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createSubGroup"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createTable"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_createTable_in_group"))
#	dataset_test_suite.addTest(dalDataset_tests("test_listTables"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_char"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_char_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_short"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_short_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_int"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_int_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_uint"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_uint_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_long"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_long_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_float"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_float_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_getAttribute_float_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_double"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_double_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_string"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setAttribute_string_list"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_openTable"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_openTable_in_group"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_openGroup"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_openArray"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_getType"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_readIntArray"))
	dataset_test_suite.addTest(dalDataset_tests("test_dataset_readFloatArray"))
#	dataset_test_suite.addTest(dalDataset_tests("test_dataset_setFilter"))

	group_test_suite = unittest.TestSuite()
	group_test_suite.addTest(dalGroup_tests("test_group_getName"))
	group_test_suite.addTest(dalGroup_tests("test_group_createShortArray"))
	group_test_suite.addTest(dalGroup_tests("test_group_createIntArray"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_char"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_char_list"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_short"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_short_list"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_int"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_int_list"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_uint"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_uint_list"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_long"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_long_list"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_float"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_float_list"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_double"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_double_list"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_string"))
	group_test_suite.addTest(dalGroup_tests("test_group_setAttribute_string_list"))
	
	table_test_suite = unittest.TestSuite()
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_char"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_short"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_int"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_uint"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_long"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_float"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_double"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_string"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_char_list"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_short_list"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_int_list"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_uint_list"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_long_list"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_float_list"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_double_list"))
	table_test_suite.addTest(dalTable_tests("test_table_setAttribute_string_list"))
	table_test_suite.addTest(dalTable_tests("test_table_addColumns"))
	table_test_suite.addTest(dalTable_tests("test_table_appendRow"))
	table_test_suite.addTest(dalTable_tests("test_table_appendRows"))
	table_test_suite.addTest(dalTable_tests("test_table_write_col_data_by_index"))
	table_test_suite.addTest(dalTable_tests("test_table_getNumberOfRows"))
#	table_test_suite.addTest(dalTable_tests("test_table_readRows"))
	
	array_test_suite = unittest.TestSuite()
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_char"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_char_list"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_short"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_short_list"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_int"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_int_list"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_uint"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_uint_list"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_long"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_long_list"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_float"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_float_list"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_double"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_double_list"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_string"))
	array_test_suite.addTest(dalArray_tests("test_array_setAttribute_string_list"))
	array_test_suite.addTest(dalArray_tests("test_array_close"))

	array_test_suite.addTest(dalDataset_tests("test_dataset_close"))
	
	alltests = unittest.TestSuite((dataset_test_suite, table_test_suite))
	#alltests = unittest.TestSuite((dataset_test_suite, group_test_suite, table_test_suite, array_test_suite))

	unittest.TextTestRunner(verbosity=5).run(alltests)

	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_char("char_attribute",'a')
	ds.close()

	ds = pydal.dalDataset("testfile.h5","HDF5")
    	table = ds.openTable("table1")
	ret = table.setAttribute_int("int_vector_attribute",[1,2,3,4])
	ds.close()
