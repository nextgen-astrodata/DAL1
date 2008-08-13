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

  \date 12-04-06
*/

#include <dal.h>
#include <cassert>

#define FILENAME "tdal_test.h5"

using namespace DAL;

typedef struct rowStruct
  {
    float time;
    float uvw[3];
    float antenna1;
  } rowStruct;


int main()
{

// create a dataset
#ifdef WITH_MYSQL
  std::cerr << "Creating a database object... ";
  string server("pc-swinbank");
  string name("lofar");
  string passwd("cs1");
  string database("pipeline");
  Database db( server, name, passwd, database );
  db.query("show tables;");
  std::cerr << "done\n\n";
#endif

  dalDataset * ds = NULL;
  std::cerr << "Creating a new HDF5 dataset called " << FILENAME << "... ";
  ds = new dalDataset( FILENAME, "HDF5" );
  std::cerr << "done." << endl;

//    ds->setFilter("A,B,C","where B=1");

  /*****************************************************
   *  Create groups
   *
   ****************************************************/
  std::cerr << "\nCreating groupA... ";
  dalGroup * groupA = ds->createGroup( "groupA" );
  assert( groupA>0 );
  std::cerr << "done." << endl;

  std::cerr << "Creating groupB... ";
  dalGroup * groupB = ds->createGroup( "groupB" );
  assert( groupB>0 );
  std::cerr << "done." << endl;

  std::cerr << "Creating groupC... ";
  dalGroup * groupC = ds->createGroup( "groupC" );
  assert( groupC>0 );
  std::cerr << "done." << endl;

  // define dimensions of array
  vector<int> dims;
  dims.push_back(4);
  dims.push_back(5);
  dims.push_back(6);

  int idata[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    idata[gg] = gg;

  vector<int> cdims;


  /*****************************************************
   *  Create arrays
   *
   ****************************************************/
  std::cerr << "\nCreating an integer array with dimensions 4x5x6 (in groupA)... ";
  dalArray * iarray = groupA->createIntArray( "int_array", dims, idata, cdims );
  std::cerr << "done." << endl;

  /*****************************************************
   *  Create attributes
   *
   ****************************************************/
  std::cerr << "Creating attributes for integer array... ";
  float sid[] = { (float)1 };
  double sf[] = { (double)2 };
  iarray->setAttribute_float( "STATION_ID", sid );
  iarray->setAttribute_double( "SAMPLE_FREQ", sf, 1 );
  iarray->setAttribute_string( "FEED", "None" );
  std::cerr << "done." << endl;

  std::cerr << "Creating an floating point array with dimensions 4x5x6... ";
  float fdata[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    fdata[gg] = rand();
  dalArray * farray = ds->createFloatArray( "float_array", dims, fdata, cdims );
  std::cerr << "done." << endl;

  std::cerr << "Creating a complex floating point array with dimensions 4x5x6... ";
  complex<float> * cdata = new complex<float>[ 4*5*6 ];
  for (int gg=0; gg<(4*5*6); gg++)
    {
      cdata[ gg ] = 0;
    }
  dalArray * carray = ds->createComplexFloatArray( "complex_array", dims, cdata, cdims );
  delete [] cdata;
  cdata = NULL;
  std::cerr << "done." << endl;

  /*****************************************************
   *  Create tables
   *
   ****************************************************/
  std::cerr << "\nCreating table 1 in groupA... ";
  dalTable * table1 = ds->createTable( "table1", "groupA" );
  std::cerr << "done." << endl;

  string colname = "TIME";

  std::cerr << "Adding float column " << colname << " to table1... ";
  table1->addColumn( colname, dal_FLOAT );
  std::cerr << "done." << endl;

  colname = "UVW";
  std::cerr << "Adding float array column " << colname << " to table1... ";
  table1->addColumn( colname, dal_FLOAT, 3 );
  std::cerr << "done." << endl;

  colname = "ANTENNA1";
  std::cerr << "Adding float column " << colname << " to table1... ";
  table1->addColumn( colname, dal_FLOAT );
  std::cerr << "done." << endl;

  rowStruct rs;

  rs.time = 1;
  rs.uvw[0] = 1;
  rs.uvw[1] = 1;
  rs.uvw[2] = 1;
  rs.antenna1 = 1;

  int count = 5;
  std::cerr << "Appending " << count << " rows to table1... ";
  for (int xx=0; xx<count; xx++)
    table1->appendRow(&rs);
  std::cerr << "done." << endl;

  float md = 3;
  float mi = 5;
  count = 3;
  int colnum = 0;
  std::cerr << "Overwriting first " << count << " rows of column " <<
            colnum << " in table1... ";
  for (int xx=0; xx<count; xx++)
    table1->writeDataByColNum( &md, colnum, xx );
  std::cerr << "done." << endl;

  count = 3;
  colnum = 2;
  std::cerr << "Overwriting first " << count << " rows of column " <<
            colnum << " in table1... ";
  for (int xx=0; xx<count; xx++)
    table1->writeDataByColNum( &mi, colnum, xx );
  std::cerr << "done." << endl;
  delete table1;

  /*****************************************************
   *  Close arrays
   *
   ****************************************************/
  std::cerr << "\nClosing integer array... ";
  if ( DAL::SUCCESS == iarray->close() )
    std::cerr << "done." << endl;
  else  std::cerr << "FAILED.";
  std::cerr << "Closing float array... ";
  if ( DAL::SUCCESS == farray->close() )
    std::cerr << "done." << endl;
  else  std::cerr << "FAILED.";
  std::cerr << "Closing complex array... ";
  if ( DAL::SUCCESS == carray->close() )
    std::cerr << "done." << endl;
  else  std::cerr << "FAILED.";


  /*****************************************************
   *  Open arrays to get attribute data
   *
   ****************************************************/
  std::cerr << "\nOpening integer array...";
  dalArray * iarray2 = ds->openArray("int_array","groupA");
  if (iarray2) std::cerr << "done." << endl;
  else  std::cerr << "FAILED.";

  std::cerr << "Calling getAttributes...\n";
  iarray2->getAttributes();

  std::cerr << "Getting STATION_ID attribute from array...";
  float * station_id;
  station_id = reinterpret_cast<float*>( iarray2->getAttribute("STATION_ID") );
  if (station_id) std::cerr << *(float*)station_id << " ...done.";
  else   std::cerr << "FAILED.";
  delete [] station_id;
  station_id = NULL;
  std::cerr << "\nClosing integer array... ";
  if ( DAL::SUCCESS == iarray2->close() ) std::cerr << "done." << endl;
  else  std::cerr << "FAILED.";
  delete iarray2;

  std::cerr << "Getting list of groups in file...\n";
  vector<string> groupnames = ds->getGroupNames();
  for (unsigned int jj=0; jj<groupnames.size(); jj++)
    std::cerr << groupnames[jj] << endl;

  if ( groupnames.size() > 0 )
    {
      std::cerr << "\nOpening group " << groupnames[0] << "... ";
      dalGroup * mygroup = ds->openGroup( groupnames[0] );
      if (NULL != mygroup)
        {
          std::cerr << "done." << endl;
          std::cerr << "Getting group member names...\n";
          vector<string> memnames = mygroup->getMemberNames();
          for (unsigned int jj=0; jj<memnames.size(); jj++)
            std::cerr << memnames[jj] << endl;
          delete mygroup;
        }
      else
        std::cerr << "FAILED.";
    }

  std::cerr << "\nClosing dataset... ";
  if ( DAL::SUCCESS == ds->close() )
    {
      std::cerr << "done." << endl;
    }
  else
    {
      std::cerr << "ERROR: Problem closing dataset." << endl;
    }

  delete iarray;
  delete farray;
  delete carray;
  delete groupA;
  delete groupB;
  delete groupC;
  delete ds;

  std::cerr << "\nSUCCESS" << endl;
  return DAL::SUCCESS;
}

