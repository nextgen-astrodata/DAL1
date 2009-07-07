#! /usr/bin/env python

import pydal
import unittest
import os
import numpy

#A testcase is created by subclassing unittest.TestCase. The three individual tests are defined with methods whose names start with the letters test. This naming convention informs the test runner about which methods represent tests.
#
#The crux of each test is a call to assertEqual() to check for an expected result; assert_() to verify a condition; or assertRaises() to verify that an expected exception gets raised. These methods are used instead of the assert statement so the test runner can accumulate all test results and produce a report.

class dal_tests(unittest.TestCase):

#    def setUp(self):

# ------------------------------------------------------------------ dalDataset

    def test_dataset_create(self):
    	self.assertEqual(type(ds),pydal.dalDataset)

    def test_dataset_createIntArray_with_chunk_dimensions(self):
    	arr = ds.createIntArray("intarray1_cdims",[2,2],[1,2,3,4],[2,2])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createIntArray(self):
    	arr = ds.createIntArray("intarray1",[2,2],[1,2,3,4])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createFloatArray(self):
    	arr = ds.createFloatArray("floatarray1",[2,2],[1.0,2.0,3.0,4.0],[2,2])
    	self.assertEqual(type(arr),pydal.dalArray)

    def test_dataset_createGroup(self):
    	group = ds.createGroup("newgroup")
    	self.assertEqual(type(group),pydal.dalGroup)

    def test_dataset_createSubGroup(self):
    	subgroup = ds.createGroup("newgroup/subgroup")
    	self.assertEqual(type(subgroup),pydal.dalGroup)

    def test_dataset_createTable(self):
    	tab = ds.createTable("table1")
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_createTable_in_group(self):  # depends on test_createGroup
    	tab = ds.createTable("table1","newgroup")
    	self.assertEqual(type(tab),pydal.dalTable)

    def test_dataset_listTables(self):  # depends on test_createTable
    	tabs = ds.listTables()
    	self.assertEqual(tabs,[1])

    def test_dataset_setAttribute_char(self):	#char
        self.assertTrue(ds.setAttribute_char("char_attribute",'a'))
        
    def test_dataset_setAttribute_short(self):	# short
        self.assertTrue(ds.setAttribute_short("short_attribute",-3872))
        
    def test_dataset_setAttribute_int(self):		# int
        self.assertTrue(ds.setAttribute_int("int_attribute",-23637))
        
    def test_dataset_setAttribute_uint(self):	# uint
        self.assertTrue(ds.setAttribute_uint("uint_attribute",92834))
        
    def test_dataset_setAttribute_long(self):	# long
        self.assertTrue(ds.setAttribute_long("long_attribute",2398226))
        
    def test_dataset_setAttribute_float(self):	# float
        self.assertTrue(ds.setAttribute_float("float_attribute",87323.234))
        
    def test_dataset_setAttribute_double(self):	# double
        self.assertTrue(ds.setAttribute_double("double_attribute",324.9287364))
        
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
    	table.addColumn("col2", "dalINT", 1 )
    	table.addColumn("col3", "dalINT", 1 )
    	cols = table.listColumns()
    	self.assertEqual(cols,['col1','col2','col3'])
    	
    def test_table_appendRows(self):
    	table = ds.openTable("table1")
    	ret = table.appendRow(numpy.array([1,2,3]))
    	ret = table.appendRow(numpy.array([4,5,6]))
    	ret = table.appendRow(numpy.array([7,8,9]))
    	self.assertTrue(ret)

    def test_table_getNumberOfRows(self): # depends on test_table_appendRows
    	table = ds.openTable("table1")
    	self.assertTrue(table.getNumberOfRows(),3)

    def test_table_readRows(self):
    	table = ds.openTable("table1")
    	aa = table.readRows(0,1)
    	print aa
    	self.assertTrue(True)

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

	suite = unittest.TestSuite()
	suite.addTest(dal_tests("test_dataset_create"))
	suite.addTest(dal_tests("test_dataset_createIntArray"))
	suite.addTest(dal_tests("test_dataset_createIntArray_with_chunk_dimensions"))
	suite.addTest(dal_tests("test_dataset_createFloatArray"))
	suite.addTest(dal_tests("test_dataset_createGroup"))
	suite.addTest(dal_tests("test_dataset_createSubGroup"))
	suite.addTest(dal_tests("test_dataset_createTable"))
	suite.addTest(dal_tests("test_dataset_createTable_in_group"))
#	suite.addTest(dal_tests("test_listTables"))
	suite.addTest(dal_tests("test_dataset_setAttribute_char"))
	suite.addTest(dal_tests("test_dataset_setAttribute_short"))
	suite.addTest(dal_tests("test_dataset_setAttribute_int"))
	suite.addTest(dal_tests("test_dataset_setAttribute_uint"))
	suite.addTest(dal_tests("test_dataset_setAttribute_long"))
	suite.addTest(dal_tests("test_dataset_setAttribute_float"))
	suite.addTest(dal_tests("test_dataset_setAttribute_double"))
	suite.addTest(dal_tests("test_dataset_setAttribute_string"))
	suite.addTest(dal_tests("test_dataset_setAttribute_string_list"))
	suite.addTest(dal_tests("test_dataset_openTable"))
	suite.addTest(dal_tests("test_dataset_openGroup"))
	suite.addTest(dal_tests("test_dataset_getType"))
	suite.addTest(dal_tests("test_dataset_readIntArray"))
	suite.addTest(dal_tests("test_dataset_readFloatArray"))
#	suite.addTest(dal_tests("test_dataset_setFilter"))

	suite.addTest(dal_tests("test_group_getName"))
	suite.addTest(dal_tests("test_group_createIntArray"))
	suite.addTest(dal_tests("test_group_setAttribute_char"))
	suite.addTest(dal_tests("test_group_setAttribute_short"))
	suite.addTest(dal_tests("test_group_setAttribute_int"))
	suite.addTest(dal_tests("test_group_setAttribute_uint"))
	suite.addTest(dal_tests("test_group_setAttribute_long"))
	suite.addTest(dal_tests("test_group_setAttribute_float"))
	suite.addTest(dal_tests("test_group_setAttribute_double"))
	suite.addTest(dal_tests("test_group_setAttribute_string"))
	
	suite.addTest(dal_tests("test_table_setAttribute_char"))
	suite.addTest(dal_tests("test_table_setAttribute_short"))
	suite.addTest(dal_tests("test_table_setAttribute_int"))
	suite.addTest(dal_tests("test_table_setAttribute_uint"))
	suite.addTest(dal_tests("test_table_setAttribute_long"))
	suite.addTest(dal_tests("test_table_setAttribute_float"))
	suite.addTest(dal_tests("test_table_setAttribute_double"))
	suite.addTest(dal_tests("test_table_setAttribute_string"))
	suite.addTest(dal_tests("test_table_addColumns"))
	suite.addTest(dal_tests("test_table_appendRows"))
	suite.addTest(dal_tests("test_table_getNumberOfRows"))
#	suite.addTest(dal_tests("test_table_readRows"))
	
	suite.addTest(dal_tests("test_dataset_close"))
	
	unittest.TextTestRunner(verbosity=3).run(suite)
