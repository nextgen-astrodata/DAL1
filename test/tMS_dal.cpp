/*-------------------------------------------------------------------------*
 | $Id:: tUseMeasures.cc 511 2007-08-05 13:14:48Z baehren                $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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
  \file tMS_dal.cpp

  \brief Test program for reading CASA measurement sets.

  \ingroup DAL

  \author Joseph Masters

  <h3>Usage</h3>

  \verbatim
  tMS_dal <input MS file> <output HDF5 file>
  \endverbatim
*/

#include <dal.h>
#include <dalDataset.h>
#include <dalData.h>

using namespace casa;

int main(int argc, char *argv[])
{
  // parameter check
  if ( argc < 3 )
    {
      cout << endl << "Too few parameters..." << endl << endl;
      cout << "The first parameter is the input casa measurement set." << endl;
      cout << "The second parameter is the output hdf5 file." << endl;
      cout << endl;
      return DAL::FAIL;
    }
  
  // create a dataset object of type MSCASA
  cout << " -- Creating a dataset object of type MSCASA ..." << endl;
  dalDataset * msds = new dalDataset( argv[1], "MSCASA" );
  
  // print out a list of tables in the dataset
  msds->listTables();
  
  // open a table in the dataset
  cout << " -- Opening table in the dataset ..." << endl;
  string tablename = "MAIN";
  string filter_cols = "UVW, TIME, ANTENNA1, ANTENNA2, DATA";
  string filter_conditions = "ANTENNA1 = 1 AND ANTENNA2 = 1";
  msds->setFilter( filter_cols, filter_conditions );
  dalTable * maintable = msds->openTable( tablename );
  
  // print the name of the table [ doesn't seem to work ]
  maintable->getName();
  
  //
  // get data from the TIME column
  //
  cout << "-------\n";
  cout << "TIME\n";
  cout << "-------\n";
  dalColumn * time_col = maintable->getColumn("TIME");
  cout << time_col->getDataType() << endl;
  if ( time_col->isScalar() ) cout << "SCALAR" << endl;
  if ( time_col->isArray() ) cout << "ARRAY" << endl;
  cout << "Number of rows: " << time_col->nrows() << endl;
  
  if ( time_col->nrows() <= 0 )
    {
      cout << "ERROR:  No rows within filtered file." << endl;
      exit(2);
    }
  
  dalData * data_object = time_col->data();
  double * value1;
  for(unsigned int xx=0; xx<13; xx++)
    {
      value1 = (double*)data_object->get(xx);  // WORKS
      cout << *value1 << endl;
    }
  delete time_col;
  
  //
  // get data from the UVW column
  //
cout << "-------\n";
cout << "UVW\n";
cout << "-------\n";
dalColumn * uvw_col = maintable->getColumn("UVW");
vector<int> shape2 = uvw_col->shape();
cout << "shape: ";
for (unsigned int uu=0; uu<shape2.size(); uu++)
  cout << shape2[uu] << ", ";
cout << endl;
cout << "ndims: " << uvw_col->ndims() << endl;
uvw_col->getDataType();
if ( uvw_col->isScalar() )  cout << "SCALAR" << endl;
if ( uvw_col->isArray() )  cout << "ARRAY" << endl;
data_object = uvw_col->data();
double * value2;
for(unsigned int xx=0; xx<5; xx++)
{
  value2 = (double*)data_object->get(xx,0);  // WORKS
  cout << *value2 << endl;
}
delete uvw_col;

//
// get data from the DATA column
//
cout << "-------\n";
cout << "DATA\n";
cout << "-------\n";
dalColumn * data_col = maintable->getColumn("DATA");
vector<int> shape3 = data_col->shape();
cout << "shape: ";
for (unsigned int uu=0; uu<shape3.size(); uu++)
  cout << shape3[uu] << ", ";
cout << endl;
unsigned int ndims3 = data_col->ndims();
cout << "ndims: " << ndims3 << endl;
data_col->getDataType();
if ( data_col->isScalar() )  cout << "SCALAR" << endl;
if ( data_col->isArray() )  cout << "ARRAY" << endl;
unsigned int nrows3 = data_col->nrows();
cout << "Number of rows: " << nrows3 << endl;
data_object = data_col->data();
complex<float> * value3;
int pol = 0;
int chan = 3;
int xx_min = 79;
int xx_max = 89;
if ( pol < shape3[0] && chan < shape3[1] && xx_max < shape3[2] )
{
 for(int xx = xx_min; xx < xx_max; xx++)
 {
  value3 = (complex<float>*)data_object->get(pol,chan,xx);
  cout << "[" << pol << "][" << chan << "][" << xx << "]: " << *value3 << endl;
 }
}


//
// create a new hdf5 dataset
//
dalDataset * ds = new dalDataset( argv[2], "HDF5" );

// define a vector for chunking dimensions
vector<int> cdims;
long indx;
complex<float> * cdata;
long mysize = 1;
for (unsigned int ll=0; ll<shape3.size(); ll++)
  mysize *= shape3[ll];
cdata = new complex<float>[ mysize ];

//#pragma omp parallel for
for(int xx=0; xx<shape3[0]; xx++)
  for(int yy=0; yy<shape3[1]; yy++)
    for(int zz=0; zz<shape3[2]; zz++)
{
    value3 = (complex<float>*)data_object->get(xx,yy,zz);
    indx = data_object->c_index(xx,yy,zz);
    cdata[ indx ] = *value3;
}
cout << "Creating complex array of data in HDF5..." << endl;
dalArray * dataarray;
dataarray = ds->createArray( "comparray", data_object );
delete data_col;

dalGroup * arraygroup;
arraygroup = ds->createGroup( "Arrays" );

// define dimensions of array
vector<int> dims;
dims.push_back(4);
dims.push_back(5);
dims.push_back(6);
int idata[4*5*6];
for (int gg=0; gg<(4*5*6); gg++)
  idata[gg] = gg;
dalArray * iarray;
cout << "Creating integer array of data in HDF5..." << endl;
iarray = arraygroup->createIntArray( "int_array", dims, idata, cdims );

ds->close();    // close the hdf5 file

cout << "SUCCESS" << endl;
return DAL::SUCCESS;
}
