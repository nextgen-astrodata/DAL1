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
double * foo1_data;
foo1_data = (double *)foo1->data();
for (int ii=0; ii<5; ii++)
  cout << "TIME data out: " << foo1_data[ii] << endl;
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
double * foo2_data;
foo2_data = (double *)foo2->data();
for (int ii=0; ii<5; ii++)
  cout << "UVW data out: " << foo2_data[ii] << endl;
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

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/*complex<float> * foo3_data =
   new complex<float>[ nrows3 * shape3[0] * shape3[1] ];*/
complex<float> * foo3_data_sb_chan;
complex<float> sum;
// for (int polarization=0; polarization<shape3[0]; polarization++)
// {
//   for (int channel=0; channel<shape3[1]; channel++)
//   {
    foo3_data_sb_chan = (complex<float> *)foo3->data(/*polarization,channel*/);
// long idx = (*8670) + (*256) + (*4)
// for (unsigned int ii=0; ii<nrows3; ii++)
// {
   
// }
// cout << foo3_data_sb_chan[ idx ] << endl;

//     for (unsigned int ii=0; ii<nrows3; ii++)
//        sum += foo3_data[ii];
//     cout << "SUM[" << polarization << "][" << channel << "]: " << sum << endl;
//     sum=0;
//   }
// }
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


// for each row
unsigned int polarization = 2; // 0,1,2, or 3
unsigned int channel = 45; // up to 256 (channel*4)
unsigned int row=345; // up to 8670 (row*1024)
unsigned int index =
  (polarization) + (channel*shape3[0]/*4*/) + (row*(shape3[0]*shape3[1]/*4*256*/));
cout << foo3_data_sb_chan[ index ] << endl;
cout << "Creating new dalData object." << endl;
dalData * data_object = new dalData();
cout << "Created new dalData object." << endl;
exit(7);
for (unsigned int lclstart=polarization; lclstart<(1024*4); lclstart+=4)
{
  for (unsigned int p=lclstart; p<(nrows3*1024); p+=1024)
    {
       sum += foo3_data_sb_chan[ p ];
    }
  // skip the next 3 polarizations of 256 channels
  cout  << "SUM: " << sum << endl;
  sum=0;
}

// for (unsigned int r=0; r<20; r++)
//   cout << foo3_data_sb_chan[r] << endl;

// for (int ii=3; ii<7; ii+=1024)
// {
//   sum += foo3_data_sb_chan[ii];
// }
// cout  << "SUM: " << sum << endl;

// for (int ii=0; ii<nrows3; ii+=1024)
// {
//   sum += foo3_data_sb_chan[ii];
// }
// cout  << "SUM: " << sum << endl;
// for (int ii=0; ii<nrows3; ii+=1024)
// {
//   sum += foo3_data_sb_chan[ii];
// }
// cout  << "SUM: " << sum << endl;

//    cout << "DATA out[" << ii << "]: " << foo3_data_sb_chan[ii] << endl;
// for (int ii=8670; ii>8665; ii--)
//    cout << "DATA out[" << ii << "]: " << foo3_data_sb_chan[ii] << endl;
delete foo3;




exit(8);





double * mytime_data = new double[ 100 ];
mytime_data = (double *)maintable->getColumnData( "TIME" );
for (int ii=0; ii<5; ii++)
  cout << "TIME data out: " << mytime_data[ii] << endl;
delete mytime_data;

int * ant1_data = new int[ 100 ];
ant1_data = (int *)maintable->getColumnData( "ANTENNA1" );
for (int ii=0; ii<5; ii++)
  cout << "ANTENNA1 data out: " << ant1_data[ii] << endl;
delete ant1_data;

int * ant2_data = new int[ 100 ];
ant2_data = (int *)maintable->getColumnData( "ANTENNA2" );
for (int ii=0; ii<5; ii++)
  cout << "ANTENNA2 data out: " << ant2_data[ii] << endl;
delete ant2_data;

double * myuvw;
myuvw = (double *)maintable->getColumnData( "UVW" );
// cout << myuvw << endl;
for (int ii=0; ii<5; ii++)
   cout << "UVW data out: " << myuvw[ii] << endl;

complex<float> * data;
data = (complex<float> *)maintable->getColumnData( "DATA" );
for (int ii=0; ii<5; ii++)
   cout << "DATA out: " << data[ii] << endl;

// close the dataset
msds->close();
delete msds;
exit(10);

	// First open the measurement set
	MeasurementSet MS( argv[1] );

	// create a msreader object
        MSReader reader(MS);

	// create a vector that will hold the table names
	Vector<String> ms_tables;
	ms_tables = reader.tables();

	// list the names of the tables
	cout << ms_tables << endl;

	// create an object for the main table and open a pointer
	Table main_table = reader.table( "MAIN" );

	// print the name of the main table
	cout << main_table.tableName() << endl;

	// point to the UVW column
	ROTableColumn uvw_column = ROTableColumn( main_table, "UVW" );

	// print the shape of the UVW column
	cout << "shape of uvw column: " << uvw_column.shapeColumn() << endl;
	
// ROTableColumn time_column = ROTableColumn( main_table, "TIME" );
// cout << "shape of time column: " << time_column.shapeColumn() << endl;

	// print the number of samples in the MS
	int numSamples = MS.nrow();
	cout << numSamples << endl;

	// print the number of fields in the MAIN table
	MSField fields = MS.field();
	int numFields  = fields.nrow();
	cout << numFields << endl;

	// create a read-only measurement set table columns object
	ROMSMainColumns msc(MS);

	// open the TIME column
	ROScalarColumn< Double > time_column = msc.time();

	// get the TIME column data
	Vector< Double > time_column_data = time_column.getColumn();
	cout << "TIME column shape " << time_column_data.shape() << endl;
	cout << "TIME column dims " << time_column_data.ndim() << endl;

	// pipe the TIME column data into a STL vector
	std::vector< double > time_data;
	time_column_data.tovector( time_data );
	cout << "TIME vector size " << time_data.size() << endl;
	cout << time_data[0] << endl << time_data[1] << endl << time_data[2] << endl;

	// print out what the first row of the data array *should* look like
	Array<Complex> darray ( msc.data()(1) );
	cout << darray << endl;

	// get the data column
	ROArrayColumn< Complex > data_column = msc.data();
	cout << "DATA column shape " << data_column.shape(1) << endl;
	cout << "DATA column dims " << data_column.ndim(1) << endl;
	Array< Complex > data_column_row;

	Array< Complex > data_column_polarization;
	std::vector< Complex > data_data;
	std::vector< complex<float> > data_row;
	for (int row=0; row < 10/*numSamples*/; row++)
	{
	  for (int polarization=0; polarization<4; polarization++)
	  {
	    IPosition start (2,polarization,0), length (2,1,256), stride (2,1,1);
	    Slicer slicer (start, length, stride);
	    data_column.getSlice(row, slicer, data_column_polarization);
	    data_column_polarization.tovector( data_data );
	    for (unsigned int goo=0; goo<data_data.size(); goo++)
	      data_row.push_back( data_data[goo] );
/*	    for (int gg=0; gg<9; gg++)
	      cout << data_data[gg];
	    cout << endl;*/
	  }
	}

// pipe the DATA column data into a STL vector
// for (int ii=0; ii<1/*numSamples*/; ii++)
// {
//   data_column.get( ii, data_column_row );
//   data_column_row.tovector( data_data );
// }
// cout << data_column_row << endl;
// cout << data_data << endl;
// cout << "DATA vector size " << data_data.size() << endl;
// for (int ii=0; ii<data_data.size(); ii++)
//   cout << data_data[ii];
// cout << endl;

	// print shape and number of dimensions for the UVW column
	cout << "UVW column shape " << msc.uvw()(1).shape() << endl;
	cout << "UVW column dims  " << msc.uvw()(1).ndim() << endl;
	
	// put the first row of the UVW column into a vector
	//std::vector<double> uvw_data;
	//msc.uvw()(1).tovector( uvw_data );
	//cout << uvw_data[0] << endl << uvw_data[1] << endl << uvw_data[2] << endl;
	//cout << msc.uvw()(1) << endl;

// Array< Complex > data_array ( msc.data()(numSamples-1) );
// cout << data_array << endl;
// cout << "Shape of DATA array = " << data_array.shape() << endl;
// Vector<uInt> time_array ( msc.time()(1) );
// cout << "Shape of TIME array = " << msc.time()(1).shape() << endl;
// cout << "TIME column dims  " << msc.time()(1).ndim() << endl;
// cout << data_array << endl;
// std::vector< std::complex<float> > stl_vec;
// data_array.tovector(stl_vec);
// msc.data()(1).tovector( stl_vec );
// cout << stl_vec[5] << endl;
// Complex * data_array_storage = data_array.getStorage( False );

// int first_axis = 1;
// int second_axis = 0;
// int idx = (first_axis*4) + second_axis;

// cout << "Second element of data array = " << data_array_storage[idx] << endl;
//cout << msc.uvw()(1).get(0,foo) << endl;
// for ( int ii=0; ii<2; ii++) {
//   cout << msc.uvw()(ii) << endl;
//   cout << msc.data()(ii) << endl;
// }

	// create a new hdf5 dataset
	dalDataset * ds;
	ds = new dalDataset( argv[2], "HDF5" );

	// define dimensions of array
	vector<int> dims;
	dims.push_back( 4 );
	dims.push_back( 10/*numSamples*/ );
	dims.push_back( 256 );
	// define a vector for chunking dimensions
	vector<int> cdims;

	dalArray * dataarray;
	dataarray = ds->createComplexArray( "dataarray", dims, data_row, cdims );

 dalGroup * arraygroup;
 arraygroup = ds->createGroup( "Arrays" );
 dims.clear();
 dims.push_back(4);
 dims.push_back(5);
 dims.push_back(6);
 int idata[4*5*6];
 for (int gg=0; gg<(4*5*6); gg++)
   idata[gg] = gg;
 dalArray * iarray;
 iarray = arraygroup->createIntArray( "int_array", dims, idata, cdims );

	ds->close();    // close the hdf5 file

	delete dataarray;
	delete arraygroup;
	delete ds;

	cout << "SUCCESS" << endl;
	return SUCCESS;
}
