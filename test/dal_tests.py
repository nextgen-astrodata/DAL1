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
    	self.assertEqual(type(ds),pydal.dalDataset)

    def test_dataset_createIntArray_with_list(self):
    	arr = ds.createIntArray("intarray1",[2,2],[1,2,3,4])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createIntArray_with_list_and_chunk_dimensions(self):
    	arr = ds.createIntArray("intarray1_cdims",[2,2],[1,2,3,4],[2,2])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createIntArray_with_numpy_array(self):
    	arr = ds.createIntArray("intarray1_numpy_array",[2,2],numpy.array([1,2,3,4]))
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createIntArray_with_numpy_array_and_chunk_dimensions(self):
    	arr = ds.createIntArray("intarray1_numpy_array_cdims",[2,2],numpy.array([1,2,3,4]),[2,2])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createFloatArray_with_list(self):
    	arr = ds.createFloatArray("floatarray1",[2,2],[1.0,2.0,3.0,4.0],[2,2])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createFloatArray_with_numpy_array(self):
    	arr = ds.createFloatArray("floatarray1_numpy_array",[2,2],numpy.array([1.0,2.0,3.0,4.0]),[2,2])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createGroup(self):
    	group = ds.createGroup("newgroup")
    	self.assertEqual(type(group),pydal.dalGroup)

    def test_dataset_createSubGroup(self):
    	subgroup = ds.createGroup("newgroup/subgroup")
    	self.assertEqual(type(subgroup),pydal.dalGroup)

    def test_dataset_createTable(self):
    	tab = ds.createTable("table1")
    	tab = ds.createTable("table2")
    	tab = ds.createTable("table3")
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_createTable_in_group(self):  # depends on test_createGroup
    	tab = ds.createTable("table1","newgroup")
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_listTables(self):  # depends on test_createTable
    	tabs = ds.listTables()
    	self.assertEqual(tabs,[1])

    def test_dataset_setAttribute_char(self):	#char
        self.assertTrue(ds.setAttribute_char("char_attribute",'a'))
        
    def test_dataset_setAttribute_char_list(self):		# int
        self.assertTrue(ds.setAttribute_char("char_list_attribute",['a','b','c']))

    def test_dataset_setAttribute_short(self):	# short
        self.assertTrue(ds.setAttribute_short("short_attribute",-3872))
        
    def test_dataset_setAttribute_short_list(self):		# int
        self.assertTrue(ds.setAttribute_short("short_list_attribute",[-1,0,1,2,3]))

    def test_dataset_setAttribute_int(self):		# int
        self.assertTrue(ds.setAttribute_int("int_attribute",-23637))
        
    def test_dataset_setAttribute_int_list(self):		# int
        self.assertTrue(ds.setAttribute_int("int_list_attribute",[-1,0,1,2,3]))

    def test_dataset_setAttribute_uint(self):	# uint
        self.assertTrue(ds.setAttribute_uint("uint_attribute",92834))
        
    def test_dataset_setAttribute_uint_list(self):		# int
        self.assertTrue(ds.setAttribute_uint("uint_list_attribute",[1,2,3]))

    def test_dataset_setAttribute_long(self):	# long
        self.assertTrue(ds.setAttribute_long("long_attribute",239822873648376436))
        
    def test_dataset_setAttribute_long_list(self):		# int
        self.assertTrue(ds.setAttribute_long("long_list_attribute",[932432432432732,823789,987987]))

    def test_dataset_setAttribute_float(self):	# float
        self.assertTrue(ds.setAttribute_float("float_attribute",87323.234))
        
    def test_dataset_setAttribute_float_list(self):	# float
        self.assertTrue(ds.setAttribute_float("float_list_attribute",[1.111,2.222,3.333]))
        
    def test_dataset_getAttribute_float_list(self):	# float
    	a = numpy.array([1.111,2.222,3.333])
    	b = ds.getAttribute_float("float_list_attribute")
    	# should be updated to compare expected with returned vals,
    	#  but needs the same precision.  Otherwise assertEqual fails.
    	self.assertTrue(type(b)==numpy.ndarray)
        
    def test_dataset_setAttribute_double(self):	# double
        self.assertTrue(ds.setAttribute_double("double_attribute",324.9287364))
        
    def test_dataset_setAttribute_double_list(self):	# double
        self.assertTrue(ds.setAttribute_double("double_list_attribute",[324.9287364,2342343.2343,890897.2433]))
        
    def test_dataset_setAttribute_string(self):	# string
        self.assertTrue(ds.setAttribute_string("string_attribute","hello world"))

    def test_dataset_setAttribute_string_list(self):	# string
        self.assertTrue(ds.setAttribute_string("string_list_attribute",["list","of","strings"]))

    def test_dataset_close(self):
    	self.assertTrue(ds.close())
    	
    def test_dataset_open(self):
    	self.assertTrue(ds.open("testfile.h5"))
    	
    def test_dataset_getType(self):
    	self.assertEqual(ds.getType(),"HDF5")

    def test_dataset_openTable(self):
    	tab = ds.openTable("table1")
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_openTable_in_group(self):
    	tab = ds.openTable("table1","newgroup")
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_openGroup(self):
    	group = ds.openGroup("newgroup")
    	self.assertEqual(type(group),pydal.dalGroup)

    def test_dataset_readIntArray(self):
    	a = ds.readIntArray("intarray1")
    	b = numpy.array([[1,2],[3,4]])
    	self.assertTrue((a==b).all())
    	
    def test_dataset_readFloatArray(self):
    	a = ds.readFloatArray("floatarray1")
    	b = numpy.array([[1.0,2.0],[3.0,4.0]])
        self.assertTrue((a==b).all())

    def test_dataset_setFilter(self):
		self.assertEqual(type(ds.setFilter()),pydal.dalFilter)

# ------------------------------------------------------------------ dalGroup
class dalGroup_tests(unittest.TestCase):
		
    def test_group_createShortArray(self):  # depends on createGroup
        group = ds.openGroup("newgroup")
        arr = group.createShortArray("shortarray1",[2,2],[1,2,3,4])
        self.assertEqual(type(group),pydal.dalGroup)

    def test_group_createIntArray(self):  # depends on createGroup
        group = ds.openGroup("newgroup")
        arr = group.createIntArray("intarray1",[2,2],[1,2,3,4])
        self.assertEqual(type(group),pydal.dalGroup)

    def test_group_getName(self):
		group = ds.openGroup("newgroup")
		self.assertEqual(group.getName(),"newgroup")

    def test_group_setAttribute_char(self):	#char
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_char("char_attribute",'a'))
        
    def test_group_setAttribute_short(self):	# short
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_short("short_attribute",-3872))
        
    def test_group_setAttribute_int(self):		# int
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_int("int_attribute",-23637))
        
    def test_group_setAttribute_uint(self):	# uint
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_uint("uint_attribute",92834))
        
    def test_group_setAttribute_long(self):	# long
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_long("long_attribute",2398226))
        
    def test_group_setAttribute_float(self):	# float
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_float("float_attribute",87323.234))
        
    def test_group_setAttribute_double(self):	# double
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_double("double_attribute",324.9287364))
        
    def test_group_setAttribute_string(self):	# string
    	group = ds.openGroup("newgroup")
    	self.assertTrue(group.setAttribute_string("string_attribute","hello world"))

    # getAttribute
    def test_group_getAttribute_string(self):	# string
    	group = ds.openGroup("newgroup")
    	self.assertEqual(group.getAttribute("string_attribute"),"hello world")

class dalTable_tests(unittest.TestCase):

#    def setUp(self):

# ------------------------------------------------------------------ dalTable

    def test_table_setAttribute_char(self):	#char
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_char("char_attribute",'a'))
        
    def test_table_setAttribute_short(self):	# short
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_short("short_attribute",-3872))
        
    def test_table_setAttribute_int(self):		# int
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_int("int_attribute",-23637))
        
    def test_table_setAttribute_uint(self):	# uint
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_uint("uint_attribute",92834))
        
    def test_table_setAttribute_long(self):	# long
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_long("long_attribute",2398226))
        
    def test_table_setAttribute_float(self):	# float
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_float("float_attribute",87323.234))
        
    def test_table_setAttribute_double(self):	# double
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_double("double_attribute",324.9287364))
        
    def test_table_setAttribute_string(self):	# string
    	table = ds.openTable("table1")
    	self.assertTrue(table.setAttribute_string("string_attribute","hello world"))

    # getAttribute
    def test_table_getAttribute_string(self):	# string
    	table = ds.openTable("table1")
    	self.assertEqual(table.getAttribute("string_attribute"),"hello world")
    	
    def test_table_addColumns(self):
    	table = ds.openTable("table1")
    	table.addColumn("col1", "dalINT", 1 )
    	table.addColumn("col2", "dalFLOAT", 1 )
    	table.addColumn("col3", "dalINT", 1 )
    	t2 = ds.openTable("table2")
    	t2.addColumn("col1", "dalINT", 1 )
    	t2.addColumn("col2", "dalFLOAT", 1 )
    	t2.addColumn("col3", "dalSHORT", 1 )
    	t3 = ds.openTable("table3")
    	t3.addColumn("col1", "dalSHORT", 1 )
    	t3.addColumn("col2", "dalSHORT", 1 )
    	t3.addColumn("col3", "dalSHORT", 1 )
    	cols = table.listColumns()
    	self.assertEqual(cols,['col1','col2','col3'])
    	
    def test_table_appendRow(self):
    	table = ds.openTable("table1")
    	for x in range(BIGNUM):
    		ret = table.appendRow([0,1.,2])
    	self.assertTrue(ret)

    def test_table_appendRows(self):
    	table = ds.openTable("table2")
    	ret = table.appendRows([0 for x in range(BIGNUM*3)],BIGNUM)
    	t3 = ds.openTable("table3")
    	ret = t3.appendRows([0 for x in range(BIGNUM*3)],BIGNUM)
    	self.assertTrue(ret)

    def test_table_write_col_data_by_index(self):
    	table = ds.openTable("table1")
    	table.write_col_by_index_boost(numpy.random.randint(0,100,BIGNUM),0,0,BIGNUM)
    	table.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.float32),1,0,BIGNUM)
    	table.write_col_by_index_boost(numpy.ones(BIGNUM,int),2,0,BIGNUM)
    	t2 = ds.openTable("table2")
    	t2.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.int32),0,0,BIGNUM)
    	t2.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.float32),1,0,BIGNUM)
    	t2.write_col_by_index_boost(numpy.array([0 for x in range(BIGNUM)],dtype=numpy.int16),2,0,BIGNUM)
    	t3 = ds.openTable("table3")
    	t3.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.int16),0,0,BIGNUM)
    	t3.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.int16),1,0,BIGNUM)
    	t3.write_col_by_index_boost(numpy.ones(BIGNUM,numpy.int16),2,0,BIGNUM)
    	self.assertTrue(table)

    def test_table_getNumberOfRows(self): # depends on test_table_appendRows
    	table = ds.openTable("table1")
    	self.assertEqual(table.getNumberOfRows(),BIGNUM)

    def test_table_readRows(self):
    	table = ds.openTable("table1")
    	aa = table.readRows(0,1)
    	print aa
    	self.assertTrue()

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

	# create the file
	ds = pydal.dalDataset("testfile.h5","HDF5")

	suite1 = unittest.TestSuite()
	suite1.addTest(dalDataset_tests("test_dataset_create"))
#	suite1.addTest(dalDataset_tests("test_dataset_open"))
	suite1.addTest(dalDataset_tests("test_dataset_createIntArray_with_list"))
	suite1.addTest(dalDataset_tests("test_dataset_createIntArray_with_list_and_chunk_dimensions"))
	suite1.addTest(dalDataset_tests("test_dataset_createIntArray_with_numpy_array"))
	suite1.addTest(dalDataset_tests("test_dataset_createIntArray_with_numpy_array_and_chunk_dimensions"))
	suite1.addTest(dalDataset_tests("test_dataset_createFloatArray_with_list"))
	suite1.addTest(dalDataset_tests("test_dataset_createFloatArray_with_numpy_array"))
	suite1.addTest(dalDataset_tests("test_dataset_createGroup"))
	suite1.addTest(dalDataset_tests("test_dataset_createSubGroup"))
	suite1.addTest(dalDataset_tests("test_dataset_createTable"))
	suite1.addTest(dalDataset_tests("test_dataset_createTable_in_group"))
#	suite1.addTest(dalDataset_tests("test_listTables"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_char"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_char_list"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_short"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_short_list"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_int"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_int_list"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_uint"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_uint_list"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_long"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_long_list"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_float"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_float_list"))
	suite1.addTest(dalDataset_tests("test_dataset_getAttribute_float_list"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_double"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_double_list"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_string"))
	suite1.addTest(dalDataset_tests("test_dataset_setAttribute_string_list"))
	suite1.addTest(dalDataset_tests("test_dataset_openTable"))
	suite1.addTest(dalDataset_tests("test_dataset_openTable_in_group"))
	suite1.addTest(dalDataset_tests("test_dataset_openGroup"))
	suite1.addTest(dalDataset_tests("test_dataset_getType"))
	suite1.addTest(dalDataset_tests("test_dataset_readIntArray"))
	suite1.addTest(dalDataset_tests("test_dataset_readFloatArray"))
#	suite1.addTest(dalDataset_tests("test_dataset_setFilter"))

	suite2 = unittest.TestSuite()
	suite2.addTest(dalGroup_tests("test_group_getName"))
	suite2.addTest(dalGroup_tests("test_group_createShortArray"))
	suite2.addTest(dalGroup_tests("test_group_createIntArray"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_char"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_short"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_int"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_uint"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_long"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_float"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_double"))
	suite2.addTest(dalGroup_tests("test_group_setAttribute_string"))
	
	suite3 = unittest.TestSuite()
	suite3.addTest(dalTable_tests("test_table_setAttribute_char"))
	suite3.addTest(dalTable_tests("test_table_setAttribute_short"))
	suite3.addTest(dalTable_tests("test_table_setAttribute_int"))
	suite3.addTest(dalTable_tests("test_table_setAttribute_uint"))
	suite3.addTest(dalTable_tests("test_table_setAttribute_long"))
	suite3.addTest(dalTable_tests("test_table_setAttribute_float"))
	suite3.addTest(dalTable_tests("test_table_setAttribute_double"))
	suite3.addTest(dalTable_tests("test_table_setAttribute_string"))
	suite3.addTest(dalTable_tests("test_table_addColumns"))
	suite3.addTest(dalTable_tests("test_table_appendRow"))
	suite3.addTest(dalTable_tests("test_table_appendRows"))
	suite3.addTest(dalTable_tests("test_table_write_col_data_by_index"))
	suite3.addTest(dalTable_tests("test_table_getNumberOfRows"))
#	suite3.addTest(dalTable_tests("test_table_readRows"))
	
	suite3.addTest(dalDataset_tests("test_dataset_close"))
	
	alltests = unittest.TestSuite((suite1, suite2, suite3))

	unittest.TextTestRunner(verbosity=5).run(alltests)
