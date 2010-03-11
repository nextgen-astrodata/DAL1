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
  \file tdal.cc
  \ingroup DAL
  \brief Test program for basic DAL functionality.
  \author Joseph Masters
*/

#include <dal.h>
#include <cassert>

#define FILENAME "tdal.h5"

using std::complex;
using namespace DAL;

#ifdef HAVE_MYSQL

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

// --------------------------------------------- open_and_close_hdf5_dataset

uint open_and_close_hdf5_dataset()
{
  uint ret = 0;

  dalDataset ds;

  ds = dalDataset( FILENAME, "HDF5" );

  if ( DAL::FAIL == ds.close() )
    ret++;

  return ret;
}

// ------------------------------------------------  create_hdf5_integer_array
uint create_hdf5_integer_array()
{
  uint ret = 0;

  dalDataset ds;

  ds = dalDataset( FILENAME, "HDF5" );

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

// ----------------------------------------------- read_hdf5_integer_array

uint read_hdf5_integer_array()
{
  uint ret = 0;

  dalDataset ds;

  ds = dalDataset( FILENAME, "HDF5" );

  dalArray * array = ds.openArray( "int_array" );
  if ( NULL == array )
    ret++;

  std::vector<int> dims =  array->dims();
  if ( dims.size() > 0 )
    {
      std::cerr << "array dimensions = ( ";
      for ( uint idx = 0; idx < dims.size(); idx++ )
        std::cerr << dims[idx] << ',';
      std::cerr << " )\n";
    }

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

  ds = dalDataset( FILENAME, "HDF5" );

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

// ---------------------------------------------------- read_hdf5_dataset_attributes
uint read_hdf5_dataset_attributes()
{
  uint ret = 0;

  dalDataset ds;

  ds = dalDataset( FILENAME, "HDF5" );

  std::string attr_name("INT_ATTR");
  int iattr = 0;
  if ( DAL::FAIL == ds.getAttribute( attr_name, iattr ) )
    ret++;
  else
    std::cerr << attr_name << "  = " << iattr << endl;

  attr_name = "FLOAT_ATTR";
  float fattr = 0.0;
  if ( DAL::FAIL == ds.getAttribute( attr_name, fattr ) )
    ret++;
  else
    std::cerr << attr_name << "  = " << fattr << endl;

  attr_name = "DOUBLE_ATTR";
  double dattr = 0.0;
  if ( DAL::FAIL == ds.getAttribute( attr_name, dattr ) )
    ret++;
  else
    printf( "%s = %f\n", attr_name.c_str() , dattr );

  attr_name = "STRING_ATTR";
  std::string sattr = "";
  if ( DAL::FAIL == ds.getAttribute( std::string(attr_name), sattr ) )
    ret++;
  else
    printf( "%s = %s\n", attr_name.c_str() , sattr.c_str() );

  if ( DAL::FAIL == ds.close() )
    ret++;

  return ret;
}

// ------------------------------------------------------ create_hdf5_table

uint create_hdf5_table()
{
  uint ret = 0;

  dalDataset ds;

  ds = dalDataset( FILENAME, "HDF5" );

  // create two identical tables - one at root level and one in a group
  dalTable * table_in_ds = ds.createTable( "table" );
  if ( NULL == table_in_ds )
    ret++;

  dalTable * table_in_group = ds.createTable( "table", "group" );
  if ( NULL == table_in_group )
    ret++;

  std::string colname = "TIME";

  table_in_ds->addColumn( colname, dal_FLOAT );
  table_in_group->addColumn( colname, dal_FLOAT );

  colname = "UVW";

  table_in_ds->addColumn( colname, dal_FLOAT, 3 );
  table_in_group->addColumn( colname, dal_FLOAT, 3 );

  colname = "ANTENNA1";
  table_in_ds->addColumn( colname, dal_FLOAT );
  table_in_group->addColumn( colname, dal_FLOAT );

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
  for (int xx=0; xx<count; xx++)
    {
      table_in_ds->appendRow(&rs);
      table_in_group->appendRow(&rs);
    }

  float md = 3;
  float mi = 5;
  count = 3;
  int colnum = 0;
  std::cerr << "Overwriting first " << count << " rows of column " <<
            colnum << " in table.\n";
  for (int xx=0; xx<count; xx++)
    {
      table_in_ds->writeDataByColNum( &md, colnum, xx );
      table_in_group->writeDataByColNum( &md, colnum, xx );
    }

  count = 3;
  colnum = 2;
  std::cerr << "Overwriting first " << count << " rows of column " <<
            colnum << " in table.\n";
  for (int xx=0; xx<count; xx++)
    {
      table_in_ds->writeDataByColNum( &mi, colnum, xx );
      table_in_group->writeDataByColNum( &mi, colnum, xx );
    }

  return ret;
}

// ---------------------------------------------------- list_groups

uint list_groups()
{
  uint ret = 0;
  dalDataset ds;

  ds = dalDataset( FILENAME, "HDF5" );

  std::cerr << "Getting list of groups in file.\n";

  std::vector<std::string> groupnames = ds.getGroupNames();
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
        ret++;
    }

  return ret;
}


// ---------------------------------------------------- main

int main()
{

  uint ret = 0;
  uint failed_tests = 0;

#ifdef HAVE_MYSQL
  std::cerr << "\n[ connect_mysql_database ]\n";
  std::cerr << "-----------------------------------------------------\n";
  connect_mysql_database();
#endif

// HDF5

// ---------- file creation, opening, closing

  std::cerr << "\n[ create_and_close_hdf5_dataset ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_dataset() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

//  std::cerr << "\n[ open_and_close_hdf5_dataset ]\n";
//  std::cerr << "-----------------------------------------------------\n";
//  if ( 0 != ( ret = open_and_close_hdf5_dataset() ) )
//    {
//      std::cerr << "FAIL\n";
//      failed_tests += ret;
//    }

// ----------- dataset attributes

  std::cerr << "\n[ set_attributes_hdf5_dataset ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = set_attributes_hdf5_dataset() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

// ----------- dataset arrays

  std::cerr << "\n[ create_hdf5_integer_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_integer_array() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ read_hdf5_integer_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = read_hdf5_integer_array() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_float_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_float_array() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_complex_float_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_complex_float_array() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ set_attributes_hdf5_array ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = set_attributes_hdf5_array() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

// ----------- dataset groups

  std::cerr << "\n[ create_hdf5_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_group() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ open_and_close_hdf5_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = open_and_close_hdf5_group() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ set_attribute_hdf5_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = set_attribute_hdf5_group() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_group_subgroup ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_group_subgroup() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_short_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_short_array_in_group() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_integer_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_integer_array_in_group() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_float_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_float_array_in_group() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_complex_float_array_in_group ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_complex_float_array_in_group() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[  read_hdf5_dataset_attributes ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = read_hdf5_dataset_attributes() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ create_hdf5_table ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = create_hdf5_table() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  std::cerr << "\n[ list_groups ]\n";
  std::cerr << "-----------------------------------------------------\n";
  if ( 0 != ( ret = list_groups() ) )
    {
      std::cerr << "FAIL\n";
      failed_tests += ret;
    }

  /*
    std::cerr << "\n[  ]\n";
    std::cerr << "-----------------------------------------------------\n";
    if ( 0 != ( ret = () ) )
    { std::cerr << "FAIL\n"; failed_tests += ret; }
  */

  std::cerr << "\nFailed tests:  " << failed_tests << endl;
  return 0;
}
