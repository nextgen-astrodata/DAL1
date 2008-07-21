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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALGROUP_H
#include <dalGroup.h>
#endif

#ifdef WITH_MYSQL
#ifndef DATABASE_H
#include <Database.h>
#endif
#endif

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
  cout << "Creating a database object... ";
  string server("pc-swinbank");
  string name("lofar");
  string passwd("cs1");
  string database("pipeline");
  Database db( server, name, passwd, database );
  db.query("show tables;");
  cout << "done\n\n";
#endif

  dalDataset * ds;
  cout << "Creating a new HDF5 dataset called " << FILENAME << "... ";
  ds = new dalDataset( FILENAME, "HDF5" );
  cout << "done." << endl;

//    ds->setFilter("A,B,C","where B=1");

  /*****************************************************
   *  Create groups
   *
   ****************************************************/
  cout << "\nCreating groupA... ";
  dalGroup * groupA = ds->createGroup( "groupA" );
  cout << "done." << endl;

  cout << "Creating groupB... ";
  dalGroup * groupB = ds->createGroup( "groupB" );
  cout << "done." << endl;

  cout << "Creating groupC... ";
  dalGroup * groupC = ds->createGroup( "groupC" );
  cout << "done." << endl;

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
  cout << "\nCreating an integer array with dimensions 4x5x6 (in groupA)... ";
  dalArray * iarray = groupA->createIntArray( "int_array", dims, idata, cdims );
  cout << "done." << endl;

  /*****************************************************
   *  Create attributes
   *
   ****************************************************/
  cout << "Creating attributes for integer array... ";
  float sid[] = { (float)1 };
  double sf[] = { (double)2 };
  string feed = "NONE";
  iarray->setAttribute_float("STATION_ID", sid );
  iarray->setAttribute_double("SAMPLE_FREQ", sf, 1 );
  iarray->setAttribute_string("FEED", feed );
  cout << "done." << endl;

  cout << "Creating an floating point array with dimensions 4x5x6... ";
  float fdata[4*5*6];
  for (int gg=0; gg<(4*5*6); gg++)
    fdata[gg] = rand();
  dalArray * farray = ds->createFloatArray( "float_array", dims, fdata, cdims );
  cout << "done." << endl;

  cout << "Creating a complex floating point array with dimensions 4x5x6... ";
  complex<float> * cdata = new complex<float>[ 4*5*6 ];
  for (int gg=0; gg<(4*5*6); gg++)
    {
      cdata[ gg ] = 0;
    }
  dalArray * carray = ds->createComplexFloatArray( "complex_array", dims, cdata, cdims );
  delete [] cdata;
  cdata = NULL;
  cout << "done." << endl;

  /*****************************************************
   *  Create tables
   *
   ****************************************************/
  cout << "\nCreating table 1 in groupA... ";
  dalTable * table1 = ds->createTable( "table1", "groupA" );
  cout << "done." << endl;

  string colname = "TIME";

  cout << "Adding float column " << colname << " to table1... ";
  table1->addColumn( colname, dal_FLOAT );
  cout << "done." << endl;

  colname = "UVW";
  cout << "Adding float array column " << colname << " to table1... ";
  table1->addColumn( colname, dal_FLOAT, 3 );
  cout << "done." << endl;

  colname = "ANTENNA1";
  cout << "Adding float column " << colname << " to table1... ";
  table1->addColumn( colname, dal_FLOAT );
  cout << "done." << endl;

  rowStruct rs;

  rs.time = 1;
  rs.uvw[0] = 1;
  rs.uvw[1] = 1;
  rs.uvw[2] = 1;
  rs.antenna1 = 1;

  int count = 5;
  cout << "Appending " << count << " rows to table1... ";
  for (int xx=0; xx<count; xx++)
    table1->appendRow(&rs);
  cout << "done." << endl;

  float md = 3;
  float mi = 5;
  count = 3;
  int colnum = 0;
  cout << "Overwriting first " << count << " rows of column " <<
       colnum << " in table1... ";
  for (int xx=0; xx<count; xx++)
    table1->writeDataByColNum( &md, colnum, xx );
  cout << "done." << endl;

  count = 3;
  colnum = 2;
  cout << "Overwriting first " << count << " rows of column " <<
       colnum << " in table1... ";
  for (int xx=0; xx<count; xx++)
    table1->writeDataByColNum( &mi, colnum, xx );
  cout << "done." << endl;
  delete table1;

  /*****************************************************
   *  Close arrays
   *
   ****************************************************/
  cout << "\nClosing integer array... ";
  if ( 0==iarray->close() )
    cout << "done." << endl;
  else  cout << "FAILED.";
  cout << "Closing float array... ";
  if ( 0==farray->close() )
    cout << "done." << endl;
  else  cout << "FAILED.";
  cout << "Closing complex array... ";
  if ( 0==carray->close() )
    cout << "done." << endl;
  else  cout << "FAILED.";


  /*****************************************************
   *  Open arrays to get attribute data
   *
   ****************************************************/
  cout << "\nOpening integer array...";
  dalArray * iarray2 = ds->openArray("int_array","groupA");
  if (iarray2) cout << "done." << endl;
  else  cout << "FAILED.";

  cout << "Calling getAttributes...";
  iarray2->getAttributes();

  cout << "Getting STATION_ID attribute from array...";
  float * station_id;
  station_id = reinterpret_cast<float*>( iarray2->getAttribute("STATION_ID") );
  if (station_id) cout << *(float*)station_id << " ...done.";
  else   cout << "FAILED.";
  delete [] station_id;
  station_id = NULL;
  cout << "\nClosing integer array... ";
  if ( 0==iarray2->close() ) cout << "done." << endl;
  else  cout << "FAILED.";
  delete iarray2;

  cout << "Getting list of groups in file...\n";
  vector<string> groupnames = ds->getGroupNames();
  for (unsigned int jj=0; jj<groupnames.size(); jj++)
    cout << groupnames[jj] << endl;

  if ( groupnames.size() > 0 )
    {
      cout << "\nOpening group " << groupnames[0] << "... ";
      dalGroup * mygroup = ds->openGroup( groupnames[0] );
      if (NULL != mygroup)
        {
          cout << "done." << endl;
          cout << "Getting group member names...\n";
          vector<string> memnames = mygroup->getMemberNames();
          for (unsigned int jj=0; jj<memnames.size(); jj++)
            cout << memnames[jj] << endl;
          delete mygroup;
        }
      else
        cout << "FAILED.";
    }

  cout << "\nClosing dataset... ";
  if ( 0 == ds->close() )
    {
      cout << "done." << endl;
    }
  else
    {
      cout << "ERROR: Problem closing dataset." << endl;
    }

  delete iarray;
  delete farray;
  delete carray;
  delete groupA;
  delete groupB;
  delete groupC;
  delete ds;

  cout << "\nSUCCESS" << endl;
  return DAL::SUCCESS;
}

