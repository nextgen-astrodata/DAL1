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
	  return FAIL;
	}

// create a dataset object of type MSCASA
dalDataset * msds;
msds = new dalDataset( argv[1], "MSCASA" );

// print out a list of tables in the dataset
msds->listTables();

// open a table in the dataset
dalTable * maintable;
string tablename = "MAIN";
string filter_string =
  "Select UVW, TIME, ANTENNA1, ANTENNA2, DATA from $1 where ANTENNA1 = 1 AND ANTENNA2 = 1";
	
maintable = msds->openFilteredTable( tablename, filter_string );

// print the name of the table [ doesn't seem to work ]
maintable->getName();

// open a column in the table
// dalColumn * timecol;
cout << "-------\n" << "TIME" << "\n-------" << endl;
dalColumn * foo1 = new dalColumn();
foo1 = maintable->getColumn("TIME");
foo1->getDataType();
if ( foo1->isScalar() )
  cout << "SCALAR" << endl;
if ( foo1->isArray() )
  cout << "ARRAY" << endl;
cout << "Number of rows: " << foo1->nrows() << endl;
/*foo1_data = (double *)foo1->data();
for (int ii=0; ii<5; ii++)
  cout << "TIME data out: " << foo1_data[ii] << endl;*/
dalData * data_object1 = foo1->data();
double * value1;
for(unsigned int xx=0; xx<13; xx++)
{
  value1 = (double*)data_object1->get(xx);  // WORKS
  cout << *value1 << endl;
}
delete foo1;

cout << "-------\n" << "UVW" << "\n-------" << endl;
dalColumn * foo2 = new dalColumn();
foo2 = maintable->getColumn("UVW");
vector<int> shape2 = foo2->shape();
cout << "shape: ";
for (unsigned int uu=0; uu<shape2.size(); uu++)
  cout << shape2[uu] << ", ";
cout << endl;
cout << "ndims: " << foo2->ndims() << endl;
foo2->getDataType();
if ( foo2->isScalar() )
  cout << "SCALAR" << endl;
if ( foo2->isArray() )
  cout << "ARRAY" << endl;
//double * foo2_data;
/*foo2_data = (double *)foo2->data();
for (int ii=0; ii<5; ii++)
  cout << "UVW data out: " << foo2_data[ii] << endl;*/
dalData * data_object2 = foo2->data();
double * value2;
for(unsigned int xx=0; xx<5; xx++)
{
  value2 = (double*)data_object2->get(xx,0);  // WORKS
  cout << *value2 << endl;
}
delete foo2;

cout << "-------\n" << "DATA" << "\n-------" << endl;
dalColumn * foo3 = new dalColumn();
foo3 = maintable->getColumn("DATA");
vector<int> shape3 = foo3->shape();
cout << "shape: ";
for (unsigned int uu=0; uu<shape3.size(); uu++)
  cout << shape3[uu] << ", ";
cout << endl;
unsigned int ndims3 = foo3->ndims();
cout << "ndims: " << ndims3 << endl;
foo3->getDataType();
if ( foo3->isScalar() )
  cout << "SCALAR" << endl;
if ( foo3->isArray() )
  cout << "ARRAY" << endl;
unsigned int nrows3 = foo3->nrows();
cout << "Number of rows: " << nrows3 << endl;
// complex<float> * foo3_data;
// foo3_data= foo3->data();
/*unsigned int polarization = 2;
unsigned int channel = 45;
unsigned int row=345;
unsigned int index =  (polarization) + (channel*shape3[0]) + (row*(shape3[0]*shape3[1]));*/
// cout << foo3_data[ index ] << endl;
dalData * data_object3 = foo3->data();
// cout << (complex<float>*)data_object.get(2,45,345) << endl;
/*complex<float> * foo3_data = (complex<float>*)*/
complex<float> * value3;
for(unsigned int xx=300; xx<320; xx++)
{
  value3 = (complex<float>*)data_object3->get(2,45,xx);  // WORKS
  cout << "[2][45][" << xx << "]: " << *value3 << endl;
}
//   value3 = (complex<float>*)data_object3->get(1,45,300);  // WORKS
//   cout << *value3 << endl;
// exit(1);


// exit(8);





// double * mytime_data = new double[ 100 ];
// mytime_data = (double *)maintable->getColumnData( "TIME" );
// for (int ii=0; ii<5; ii++)
//   cout << "TIME data out: " << mytime_data[ii] << endl;
// delete mytime_data;
// 
// int * ant1_data = new int[ 100 ];
// ant1_data = (int *)maintable->getColumnData( "ANTENNA1" );
// for (int ii=0; ii<5; ii++)
//   cout << "ANTENNA1 data out: " << ant1_data[ii] << endl;
// delete ant1_data;
// 
// int * ant2_data = new int[ 100 ];
// ant2_data = (int *)maintable->getColumnData( "ANTENNA2" );
// for (int ii=0; ii<5; ii++)
//   cout << "ANTENNA2 data out: " << ant2_data[ii] << endl;
// delete ant2_data;
// 
// double * myuvw;
// myuvw = (double *)maintable->getColumnData( "UVW" );
// // cout << myuvw << endl;
// for (int ii=0; ii<5; ii++)
//    cout << "UVW data out: " << myuvw[ii] << endl;
// 
// complex<float> * data;
// data = (complex<float> *)maintable->getColumnData( "DATA" );
// for (int ii=0; ii<5; ii++)
//    cout << "DATA out: " << data[ii] << endl;
// 
// // close the dataset
// msds->close();
// delete msds;

// exit(10);

// create a new hdf5 dataset
dalDataset * ds;
ds = new dalDataset( argv[2], "HDF5" );

// define a vector for chunking dimensions
vector<int> cdims;
long indx;
// complex<float> total=0;
dalArray * dataarray;
complex<float> * cdata;
long mysize = shape3[0] * shape3[1] * shape3[2];
cdata = new complex<float>[ mysize ];
long ccount=0;
for(int xx=0; xx<shape3[0]; xx++)
  for(int yy=0; yy<shape3[1]; yy++)
    for(int zz=0; zz<shape3[2]; zz++)
{
    ccount+=1;
    value3 = (complex<float>*)data_object3->get(xx,yy,zz);
// cout << *value3 << ',';
     indx = xx*256*8670 + yy*8670 + zz;  // YES!
     indx = xx*shape3[1]*shape3[2] + yy*shape3[2] + zz;  // YES!
     cdata[ indx ] = *value3;
//  if (8680 == ccount) exit(4);
//     cdata[ indx ] = ccount;
// total += *value3;
}
/*for(unsigned int xx=300; xx<320; xx++)
{
  value3 = (complex<float>*)data_object3->get(2,45,xx);  // WORKS
  cout << "[2][45][" << xx << "]: " << *value3 << endl;
}*/
// cout << total << endl;
// exit(9);
// cout << "Creating complex array of data in HDF5..." << endl;
// // dataarray = ds->createComplexArray( "dataarray", shape3, (complex<float>*)data_object3->data, cdims );
dataarray = ds->createComplexArray( "dataarray", shape3, cdata, cdims );

vector<int> dims;
// dims.push_back(2);
// dims.push_back(3);
// dims.push_back(4);
// complex<float> * adata;
// adata = new complex<float>[ 2*3*4 ];
// adata[ 11 ] = 11;
// // adata[ 0 + (1*3) + (1*3*4)] = 1;
// dataarray = ds->createComplexArray( "dataarray", dims/*shape3*/, adata/*cdata*/, cdims );
// free(adata);

free(cdata);
delete foo3;

dalGroup * arraygroup;
arraygroup = ds->createGroup( "Arrays" );
// define dimensions of array
dims.clear();
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

// delete dataarray;
// delete arraygroup;
// delete ds;

cout << "SUCCESS" << endl;
return SUCCESS;
}
