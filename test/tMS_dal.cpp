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

// First open the measurement set
dalDataset * msds;
msds = new dalDataset( argv[1], "MSCASA" );
msds->listTables();
dalTable * maintable;
maintable = msds->openTable( "MAIN" );
maintable->getName();
msds->close();  // close the casa MeasurementSet
delete msds;

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
