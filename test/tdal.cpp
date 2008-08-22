/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*!
  \file tdal.cpp
  \ingroup DAL
  \brief Test program for basic DAL functionality.
  \author Joseph Masters
*/

#include <dal.h>
#include <cassert>

#define FILENAME "tdal.h5"

using namespace DAL;

#ifdef WITH_MYSQL

// ---------------------------------------------------- connect_mysql_database

uint connect_mysql_database()
{
  uint ret = 0;

  string server("pc-swinbank");
  string name("lofar");
  string passwd("cs1");
  string database("pipeline");
  Database db( server, name, passwd, database );
  if ( DAL::FAIL == db.query("show tables;") )
    ret++;

  return ret;
}
#endif

// ---------------------------------------------------- create_hdf5_dataset

uint create_hdf5_dataset()
{
  uint ret = 0;

  dalDataset * ds = NULL;
  ds = new dalDataset( FILENAME, "HDF5" );
  if ( NULL == ds )
    ret++;

  if ( DAL::FAIL == ds->close() )
   ret++;

  delete ds;

  return ret;
}

// ------------------------------------------------  create_hdf5_integer_array
uint create_hdf5_integer_array()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);
  vector<int> cdims;

  int data[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    data[gg] = gg;

  dalArray * array = ds.createIntArray( "int_array", dims, data, cdims );
  if ( NULL == array )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete array;

  return ret;
}

// ---------------------------------------------  set_attributes_hdf5_dataset
uint set_attributes_hdf5_dataset()
{
  dalDataset ds;

  uint ret = 0;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret = false;

  std::string sval = "string test value";
  if ( DAL::FAIL == ds.setAttribute( "STRING_ATTR", sval ) )
    ret = false;

  std::vector<std::string> svals;
  svals.push_back("string");
  svals.push_back("vector");
  svals.push_back("test");
  if ( DAL::FAIL == ds.setAttribute_string( "STRING_ATTRS", svals ) )
    ret++;

  int ival = 1;
  if ( DAL::FAIL == ds.setAttribute( "INT_ATTR", &ival ) )
    ret++;

  int ivals[] = { 1, 2, 3 };
  if ( DAL::FAIL == ds.setAttribute( "INT_ATTRS", ivals, 3 ) )
    ret++;

  uint uival = 2;
  if ( DAL::FAIL == ds.setAttribute( "UINT_ATTR", &uival ) )
    ret++;

  uint uivals[] = { 1, 2, 3};
  if ( DAL::FAIL == ds.setAttribute( "UINT_ATTRS", uivals, 3 ) )
    ret++;

  float fval = 3.0;
  if ( DAL::FAIL == ds.setAttribute( "FLOAT_ATTR", &fval ) )
    ret++;

  float fvals[] = { 1.0, 2.0, 3.0 };
  if ( DAL::FAIL == ds.setAttribute( "FLOAT_ATTRS", fvals, 3 ) )
    ret++;

  double dval = 3.0;
  if ( DAL::FAIL == ds.setAttribute( "DOUBLE_ATTR", &dval ) )
    ret++;

  double dvals[] = { 1.0, 2.0, 3.0 };
  if ( DAL::FAIL == ds.setAttribute( "DOUBLE_ATTRS", dvals, 3 ) )
    ret++;

  return ret;
}

// ------------------------------------------------  create_hdf5_float_array
uint create_hdf5_float_array()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);
  vector<int> cdims;

  float data[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    data[gg] = rand();

  dalArray * array = ds.createFloatArray( "float_array", dims, data, cdims );
  if ( NULL == array )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete array;

  return ret;
}

// ---------------------------------------  create_hdf5_complex_float_array
uint create_hdf5_complex_float_array()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);
  vector<int> cdims;

  complex<float> * data = new complex<float>[ 4*5*6 ];
  for (int gg=0; gg<(4*5*6); gg++)
    data[ gg ] = gg;

  dalArray * array = ds.createComplexFloatArray( "complex_float_array",
                                                 dims, data, cdims );

  if ( NULL == array )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete [] data;
  data = NULL;
  delete array;

  return ret;
}

// ----------------------------------------------  set_attributes_hdf5_array

uint set_attributes_hdf5_array()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalArray * array = NULL;
  array = ds.openArray( "int_array" );
  if ( NULL == array )
   ret++;

  std::string sval = "string test value";
  if ( DAL::FAIL == array->setAttribute( "STRING_ATTR", sval ) )
    ret = false;

  std::string svals[]= {"string","vector","test"};
  if ( DAL::FAIL == array->setAttribute( "STRING_ATTRS", svals, 3 ) )
    ret++;

  int ival = 1;
  if ( DAL::FAIL == array->setAttribute( "INT_ATTR", &ival ) )
    ret++;

  int ivals[] = { 1, 2, 3 };
  if ( DAL::FAIL == array->setAttribute( "INT_ATTRS", ivals, 3 ) )
    ret++;

  uint uival = 2;
  if ( DAL::FAIL == array->setAttribute( "UINT_ATTR", &uival ) )
    ret++;

  uint uivals[] = { 1, 2, 3};
  if ( DAL::FAIL == array->setAttribute( "UINT_ATTRS", uivals, 3 ) )
    ret++;

  float fval = 3.0;
  if ( DAL::FAIL == array->setAttribute( "FLOAT_ATTR", &fval ) )
    ret++;

  float fvals[] = { 1.0, 2.0, 3.0 };
  if ( DAL::FAIL == array->setAttribute( "FLOAT_ATTRS", fvals, 3 ) )
    ret++;

  double dval = 3.0;
  if ( DAL::FAIL == array->setAttribute( "DOUBLE_ATTR", &dval ) )
    ret++;

  double dvals[] = { 1.0, 2.0, 3.0 };
  if ( DAL::FAIL == array->setAttribute( "DOUBLE_ATTRS", dvals, 3 ) )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete array;

  return ret;
}

// ---------------------------------------------------- create_hdf5_group

uint create_hdf5_group()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = ds.createGroup( "group" );
  if ( NULL == group )
   ret++;

  if ( DAL::FAIL == group->close() )
   ret++;

  delete group;

  return ret;
}

// ---------------------------------------------- open_and_close_hdf5_group

uint open_and_close_hdf5_group()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = NULL;
  group = ds.openGroup( "group" );
  if ( NULL == group )
   ret++;

  if ( DAL::FAIL == group->close() )
   ret++;

  delete group;

  return ret;
}

// ---------------------------------------------- set_attribute_hdf5_group

uint set_attribute_hdf5_group()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = NULL;
  group = ds.openGroup( "group" );
  if ( NULL == group )
   ret++;

  std::string sval = "string test value";
  if ( DAL::FAIL == group->setAttribute( "STRING_ATTR", sval ) )
    ret = false;

  std::string svals[]= {"string","array","test"};
  if ( DAL::FAIL == group->setAttribute( "STRING_ATTRS", svals, 3 ) )
    ret++;

  int ival = 1;
  if ( DAL::FAIL == group->setAttribute( "INT_ATTR", &ival ) )
    ret++;

  int ivals[] = { 1, 2, 3 };
  if ( DAL::FAIL == group->setAttribute( "INT_ATTRS", ivals, 3 ) )
    ret++;

  uint uival = 2;
  if ( DAL::FAIL == group->setAttribute( "UINT_ATTR", &uival ) )
    ret++;

  uint uivals[] = { 1, 2, 3};
  if ( DAL::FAIL == group->setAttribute( "UINT_ATTRS", uivals, 3 ) )
    ret++;

  float fval = 3.0;
  if ( DAL::FAIL == group->setAttribute( "FLOAT_ATTR", &fval ) )
    ret++;

  float fvals[] = { 1.0, 2.0, 3.0 };
  if ( DAL::FAIL == group->setAttribute( "FLOAT_ATTRS", fvals, 3 ) )
    ret++;

  double dval = 3.0;
  if ( DAL::FAIL == group->setAttribute( "DOUBLE_ATTR", &dval ) )
    ret++;

  double dvals[] = { 1.0, 2.0, 3.0 };
  if ( DAL::FAIL == group->setAttribute( "DOUBLE_ATTRS", dvals, 3 ) )
    ret++;

  if ( DAL::FAIL == group->close() )
   ret++;

  delete group;
  return ret;
}

// -------------------------------------------  create_hdf5_group_subgroup

uint create_hdf5_group_subgroup()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = ds.openGroup("group");
  if ( NULL == group )
   ret++;

  dalGroup * subgroup = group->createGroup( "subgroup" );
  if ( NULL == subgroup )
   ret++;

  if ( DAL::FAIL == subgroup->close() )
   ret++;

  if ( DAL::FAIL == group->close() )
   ret++;

  delete group;
  delete subgroup;

  return ret;

}

// ---------------------------------------  create_hdf5_integer_array_in_group
uint create_hdf5_integer_array_in_group()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = ds.openGroup("group");
  if ( NULL == group )
   ret++;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);
  vector<int> cdims;

  int data[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    data[gg] = gg;

  dalArray * array = group->createIntArray( "int_array", dims, data, cdims );
  if ( NULL == array )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete array;
  delete group;

  return ret;
}

// ---------------------------------------  create_hdf5_float_array_in_group
uint create_hdf5_float_array_in_group()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = ds.openGroup("group");
  if ( NULL == group )
   ret++;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);
  vector<int> cdims;

  float data[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    data[gg] = rand();

  dalArray * array = group->createFloatArray( "float_array", dims, data, cdims );
  if ( NULL == array )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete array;
  delete group;

  return ret;
}

// --------------------------------  create_hdf5_complex_float_array_in_group
uint create_hdf5_complex_float_array_in_group()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = ds.openGroup("group");
  if ( NULL == group )
   ret++;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);
  vector<int> cdims;

  complex<float> * data = new complex<float>[ 4*5*6 ];
  for (int gg=0; gg<(4*5*6); gg++)
    data[ gg ] = gg;

  dalArray * array = group->createComplexFloatArray( "complex_float_array",
                                                 dims, data, cdims );

  if ( NULL == array )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete [] data;
  data = NULL;
  delete array;
  delete group;

  return ret;
}

// --------------------------------  create_hdf5_short_array_in_group

uint create_hdf5_short_array_in_group()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  dalGroup * group = ds.openGroup("group");
  if ( NULL == group )
   ret++;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);
  vector<int> cdims;

  short data[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    data[gg] = rand();

  dalArray * array = group->createShortArray( "short_array", dims, data,
                                              cdims );
  if ( NULL == array )
    ret++;

  if ( DAL::FAIL == array->close() )
   ret++;

  delete array;
  delete group;

  return ret;
}







































// ---------------------------------------------------- create_tables

bool create_tables( dalDataset ds )
{
  std::cerr << "Creating table 1 in groupA.\n";
  dalTable * table1 = ds.createTable( "table1", "groupA" );

  string colname = "TIME";

  std::cerr << "Adding float column " << colname << " to table1.\n";
  table1->addColumn( colname, dal_FLOAT );

  colname = "UVW";
  std::cerr << "Adding float array column " << colname << " to table1.\n";
  table1->addColumn( colname, dal_FLOAT, 3 );

  colname = "ANTENNA1";
  std::cerr << "Adding float column " << colname << " to table1.\n";
  table1->addColumn( colname, dal_FLOAT );

  struct rowStruct
    {
      float time;
      float uvw[3];
      float antenna1;
    } rs;

  rs.time = 1;
  rs.uvw[0] = 1;
  rs.uvw[1] = 1;
  rs.uvw[2] = 1;
  rs.antenna1 = 1;

  int count = 5;
  std::cerr << "Appending " << count << " rows to table1.\n";
  for (int xx=0; xx<count; xx++)
    table1->appendRow(&rs);

  float md = 3;
  float mi = 5;
  count = 3;
  int colnum = 0;
  std::cerr << "Overwriting first " << count << " rows of column " <<
            colnum << " in table1.\n";
  for (int xx=0; xx<count; xx++)
    table1->writeDataByColNum( &md, colnum, xx );

  count = 3;
  colnum = 2;
  std::cerr << "Overwriting first " << count << " rows of column " <<
            colnum << " in table1.\n";
  for (int xx=0; xx<count; xx++)
    table1->writeDataByColNum( &mi, colnum, xx );

  return true;
}


// --------------------------------------------- open_and_close_hdf5_dataset

uint open_and_close_hdf5_dataset()
{
  uint ret = 0;

  dalDataset ds;

  if ( DAL::FAIL == ds.open( FILENAME ) )
    ret++;

  return ret;
}

// ---------------------------------------------------- open_integer_array

bool open_integer_array()
{
  dalDataset ds;
  if ( DAL::FAIL == ds.open( FILENAME ) )
   return false;

  dalArray * iarray2 = ds.openArray( "int_array", "groupA" );
  if ( NULL == iarray2 )
   return false;

  std::cerr << "Calling getAttributes.\n";
  iarray2->getAttributes();

  std::cerr << "Getting STATION_ID attribute from array = ";
  float * station_id;
  station_id = reinterpret_cast<float*>( iarray2->getAttribute("STATION_ID") );
  if (station_id) std::cerr << *(float*)station_id << endl;
  delete [] station_id;
  station_id = NULL;
  std::cerr << "Closing integer array.\n";

  if ( DAL::FAIL == iarray2->close() )
   return false;

  return true;
}

// ---------------------------------------------------- list_groups

bool list_groups()
{
  dalDataset ds;
  if ( DAL::FAIL == ds.open( FILENAME ) )
   return false;

  std::cerr << "Getting list of groups in file.\n";

  vector<string> groupnames = ds.getGroupNames();
  for (unsigned int jj=0; jj<groupnames.size(); jj++)
    std::cerr << groupnames[jj] << endl;

  if ( groupnames.size() > 0 )
    {
      std::cerr << "Opening group " << groupnames[0] << ".\n";
      dalGroup * mygroup = ds.openGroup( groupnames[0] );
      if ( NULL != mygroup )
        {
          std::cerr << "Getting group member names.\n";
          vector<string> memnames = mygroup->getMemberNames();
          for (unsigned int jj=0; jj<memnames.size(); jj++)
            std::cerr << memnames[jj] << endl;
          delete mygroup;
        }
      else
        return false;
    }

  return true;
}

// ---------------------------------------------------- main

int main()
{

  uint ret = 0;
  uint failed_tests = 0;

#ifdef WITH_MYSQL
  std::cerr << "\n[ connect_mysql_database ]\n";
  std::cerr << "-----------------------------------------------------\n";
  connect_mysql_database();
#endif

// HDF5

// ---------- file creation, opening, closing

  std::cerr << "\n[ create_and_close_hdf5_dataset ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_dataset() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ open_and_close_hdf5_dataset ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = open_and_close_hdf5_dataset() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

// ----------- dataset attributes

  std::cerr << "\n[ set_attributes_hdf5_dataset ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = set_attributes_hdf5_dataset() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

// ----------- dataset arrays

  std::cerr << "\n[ create_hdf5_integer_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_integer_array() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ create_hdf5_float_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_float_array() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ create_hdf5_complex_float_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_complex_float_array() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ set_attributes_hdf5_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = set_attributes_hdf5_array() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

// ----------- dataset groups

  std::cerr << "\n[ create_hdf5_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_group() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ open_and_close_hdf5_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = open_and_close_hdf5_group() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ set_attribute_hdf5_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = set_attribute_hdf5_group() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ create_hdf5_group_subgroup ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_group_subgroup() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ create_hdf5_short_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_short_array_in_group() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ create_hdf5_integer_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_integer_array_in_group() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ create_hdf5_float_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_float_array_in_group() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ create_hdf5_complex_float_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_complex_float_array_in_group() ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }





// ----------- group subgroup creation

/*
  std::cerr << "\n[  ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = () ) )
  { std::cerr << "FAIL\n"; failed_tests += ret; }

  std::cerr << "\n[ open_integer_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  open_integer_array();

  std::cerr << "\n[ list_groups ]\n";
  std::cerr << "-----------------------------------------------------\n";
  list_groups();
*/

  std::cerr << "\nFailed tests:  " << failed_tests << endl;
  return 0;
}
