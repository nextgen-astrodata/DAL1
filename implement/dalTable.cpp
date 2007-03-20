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
/**
 * \file dalTable.cpp
 * \author Joseph Masters
 * \date 12-04-06
 */
 
#ifndef DALTABLE_H
#include "dalTable.h"
#endif

// for high-level table interface (written in C)
#include "H5TA.h"

using namespace H5;

dalTable::dalTable()
{
 	columns.clear();  // clear the coulumns vector
 	firstrecord = true;
}

void dalTable::openTable( void * voidfile, string tablename, string groupname )
{
	name = groupname + '/' + tablename;
	H5::H5File * lclfile = (H5::H5File*)voidfile; // H5File object
	file = lclfile;
	file_id = lclfile->getLocId();  // get the file handle
    table_id = H5Dopen ( file_id, name.c_str() );
}

dalTable::~dalTable()
{
	/* broken in HDF5 library itself! */
	//  	if ( nrecords > 1 )
	//		H5TBdelete_record (file_id, name.c_str(), 0, 1);
}

void dalTable::printColumns()
{
	// Print the column list by iterating through the global column vector
	cout << "Columns vector list:" << endl;
	for (unsigned int ii=0; ii < columns.size(); ii++)
		cout << columns[ii].getName() << " " << endl;
}

void dalTable::createTable( void * voidfile, string tablename, string groupname )
{
	/* It is necessary to have at least one column for table creation
	 * so we create a dummy column that will be deleted when real
	 * columns are added
	 */
	
	int			dummy_col;
	name = groupname + '/' + tablename;// set the private class variable: name
	
	// cast the voidfile to an hdf5 file
	H5::H5File * lclfile = (H5::H5File*)voidfile; // H5File object
	file = lclfile;
	file_id = lclfile->getLocId();  // get the file handle

	const unsigned int	NFIELDS = 1;
	const unsigned long	NRECORDS = 1;
	size_t			dst_size =  sizeof(dummy_col);
	const char		*lclfield_names[NFIELDS] = { "000dummy000" };
	size_t			dst_offset[NFIELDS] = { 0 };
		
	hid_t			field_type[NFIELDS] = { H5T_NATIVE_INT };	
	hsize_t			chunk_size = CHUNK_SIZE;
	int			fill_data[NFIELDS] = { -1 };
	int			compress  = 0;  // 0=off 1=on

	typedef struct Particle {
		int		dummy;
	} Particle;
	Particle data[NFIELDS];

	dst_size = sizeof( Particle );
	dst_offset[0] = HOFFSET( Particle, dummy );

	tablename = groupname + '/' + tablename;
	//cout << tablename << endl;
	status = H5TBmake_table( "My Table Title", file_id, tablename.c_str(),
				 NFIELDS, NRECORDS, dst_size, lclfield_names,
				 dst_offset, field_type, chunk_size,
				 fill_data, compress, data );
}



void dalTable::addColumn( string colname, string coltype, unsigned int size )
{
	
	// make sure the column name isn't blank
	if ( 0 == colname.length() ) {
		cout << "WARNING: Trying to add column without a name.  Skipping."
		     << endl;
		return;
	}

	if ( size > 1 ) {
		addArrayColumn( colname, coltype, size );
		return;
	}
	
	if ( ( dal_COMPLEX == coltype ) || ( dal_DCOMPLEX == coltype ) ) 
	{
		vector<dalColumn> cv;
		string component_type;
		
		component_type = dal_DOUBLE;
			
		dalColumn col_a( "r", component_type );  // real component
		dalColumn col_b( "i", component_type );  // imaginary component

		cv.push_back( col_a );
		cv.push_back( col_b );

		addComplexColumn( colname, cv, 2 );
		return;
	}
	
	// retrieve table information
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	

	// allocate space for the column/field names and retrieve them from
	// the table
	field_names = (char**)malloc( nfields * sizeof(char*) );
	for (unsigned int ii=0; ii<nfields; ii++) {
		field_names[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
	}
	status = H5TBget_field_info( file_id, name.c_str(), field_names, NULL,
				     NULL, NULL );

	// check to make sure column doesn't already exist
	bool removedummy = false;
	for (unsigned int ii=0; ii<nfields; ii++) {
		if (0 == strcmp(colname.c_str(),field_names[ii])) {
			cout << "WARNING: Cannot create column \'"
			     << colname.c_str()
			     <<	"\'. Column already exists." << endl;
			return;
		}
		else if (0 == strcmp("000dummy000",field_names[ii])) {
			removedummy = true;
		}
	}

	for (unsigned int ii=0; ii<nfields; ii++) {
		free(field_names[ii]);
	}
	
	// set the column type
	hid_t	field_type_new;	
	if ( dal_INT == coltype )
		field_type_new = H5T_NATIVE_INT;
		
	else if ( dal_UINT == coltype )
		field_type_new = H5T_NATIVE_UINT;

	else if ( dal_SHORT == coltype )
		field_type_new = H5T_NATIVE_SHORT;

	else if ( dal_FLOAT == coltype )
		field_type_new = H5T_NATIVE_FLOAT;

	else if ( dal_DOUBLE == coltype )
		field_type_new = H5T_NATIVE_DOUBLE;
		
	else if ( dal_STRING == coltype ) {
		field_type_new = H5Tcopy( H5T_C_S1 );
		H5Tset_size( field_type_new, 16 );
	}
	
	else {
		cout << "ERROR: column type " << coltype << " is not supported."
		     << endl;
		exit(99);
	}
	
	// set additional required fields for new column call
	hsize_t	position = nfields;
	void * data; 

	// create the new column
	status = H5TBinsert_field( file_id, name.c_str(), colname.c_str(),
				   field_type_new, position, NULL, data );

	if ( removedummy )
		removeColumn("000dummy000");
	
	return;
	
	// if successful, add corresponding column object to list
	if ( 0 == status ) {
	  	dalColumn * lc = new dalColumn( colname, dal_INT /*coltype*/ );
		
		// add new dalColumn to the 'columns' vector
	  	columns.push_back( *lc );
	
	 	lc->addMember( "a_member", coltype );
	}

}
herr_t attr_info(hid_t loc_id, const char *name, void *opdata);
void dalTable::getAttributes() {

   //status = H5Aget_num_attrs(group_id);
   //printf ("H5Aget_num_attrs returns: %i\n", status);
   status = H5Aiterate( table_id, NULL, attr_info, NULL );
   //printf ("\nH5Aiterate returns: %i\n", status);

}

void dalTable::addArrayColumn( string colname, string coltype, unsigned int indims )
{
	// make sure the column name isn't blank
	if ( 0 == colname.length() ) {
		cout << "WARNING: Trying to add column without a name.  Skipping."
		     << endl;
		return;
	}


	// retrieve table information
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	

	// allocate space for the column/field names and retrieve them from
	// the table
	field_names = (char**)malloc( nfields * sizeof(char*) );
	for (unsigned int ii=0; ii<nfields; ii++) {
		field_names[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
	}
	status = H5TBget_field_info( file_id, name.c_str(), field_names, NULL,
				     NULL, NULL );

	// check to make sure column doesn't already exist
	for (unsigned int ii=0; ii<nfields; ii++) {
		if (0 == strcmp(colname.c_str(),field_names[ii])) {
			cout << "WARNING: Cannot create column \'"
			     << colname.c_str()
			     << "\'. Column already exists." << endl;
			return;
		}
	}
	for (unsigned int ii=0; ii<nfields; ii++) {
		free(field_names[ii]);
	}
		
	// set the column type
	hsize_t dd = indims;
	hsize_t * dims = &dd;
	hid_t	field_type;	
	hid_t   h5type;
	
	if ( dal_INT == coltype )
		h5type =  H5T_NATIVE_INT;
	
	else if ( dal_UINT == coltype )
		h5type = H5T_NATIVE_UINT;
	
	else if ( dal_SHORT == coltype )
		h5type = H5T_NATIVE_SHORT;
	
	else if ( dal_FLOAT == coltype )
		h5type = H5T_NATIVE_FLOAT;
	
	else if ( dal_DOUBLE == coltype )
		h5type = H5T_NATIVE_DOUBLE;
	
	else if ( dal_STRING == coltype )
		h5type = H5T_NATIVE_CHAR;
	
	else {
		cout << "ERROR: column type " << coltype << " is not supported." 
		     << endl;
		exit(99);
	}
	field_type = H5Tarray_create( h5type, 1, dims, NULL);

	// set additional required fields for new column call
	hsize_t	position = nfields;
	int		fill_data[indims];
	int		data[indims]; 
	
	// create the new column
	status = H5TBinsert_field( file_id, name.c_str(), colname.c_str(),
				   field_type, position, fill_data, data );

	// if successful, add corresponding column object to list
	if ( 0 == status ) {
	  	dalColumn * lc = new dalColumn( colname, "foo" );
		
		// add new dalColumn to the 'columns' vector
	  	columns.push_back( *lc );
	
	 	lc->addMember( "a_member", dal_INT );
	}

}

void dalTable::addComplexColumn( string compname, vector<dalColumn> foo,
									int subfields )
{
	size_t sz = 0;
	
	// compute the size of the compound column
	for ( unsigned int ii=0; ii<foo.size(); ii++) {
		//cout << "subcolumn name is " << foo[ii].getName() 
		//	 << ". Type is " << foo[ii].getType() << endl;
		sz += foo[ii].getSize();
	}

	// create a compound type that can hold each field
	hid_t fieldtype = H5Tcreate( H5T_COMPOUND, sz );

	size_t offset = 0;
	for ( unsigned int ii=0; ii<foo.size(); ii++) {
		
		if (0==ii)
			offset=0;
		else
			offset += foo[ii-1].getSize();
			

		if ( dal_INT == foo[ii].getType() ) {
			H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
				   H5T_NATIVE_INT);
		}
		else if ( dal_FLOAT == foo[ii].getType() ) {
			H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
				   H5T_NATIVE_FLOAT);
		}
		else if ( dal_DOUBLE == foo[ii].getType() ) {
			H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
				   H5T_NATIVE_DOUBLE);
		}
	}

	// retrieve table information
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );

	// set additional required fields for new column call
	hsize_t	position = nfields;
	int		fill_data[subfields];
	int		data[subfields]; 

	// create the new column
	status = H5TBinsert_field(	file_id,
								name.c_str(),
								compname.c_str(),
								fieldtype,
								position,
								fill_data,
								data );	

	return;
	// if successful, add corresponding column object to list
	if ( 0 == status ) {
	  	dalColumn * lc = new dalColumn( compname, "foo" /*H5_COMPOUND*/ );
		
		// add new dalColumn to the 'columns' vector
	  	columns.push_back( *lc );

	 	lc->addMember( "a_member", dal_INT );
	}

}

void dalTable::removeColumn(string colname)
{	
	status = H5TBget_table_info( file_id, name.c_str(), &nfields, &nrecords );
	if ( nfields < 2 ) {
			cout << "WARNING: Cannot delete last column." << endl;
			return;
	}
	
	field_names = (char**)malloc( nfields * sizeof(char*) );
	for (unsigned int ii=0; ii<nfields; ii++)
	{
		field_names[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
	}
	
	
	status = H5TBget_field_info( file_id, name.c_str(), field_names, NULL,
				     NULL, NULL );

	//if ( 0 == status )
		//{
			//columns.pop_back();
	//	}

	bool columnpresent = false;					
	for (unsigned int ii=0; ii < nfields; ii++) {
		
		if (0 == strcmp(colname.c_str(),field_names[ii])) {
			status = H5TBdelete_field( file_id, name.c_str(),
						   field_names[ii]);
			status = H5TBget_table_info ( file_id, name.c_str(),
						      &nfields, &nrecords );
			columnpresent = true;
			break;
		}
	}
	for (unsigned int ii=0; ii<nfields; ii++) {
		free(field_names[ii]);
	}
	
	if ( !columnpresent )
		cout << "WARNING: Column \'" << colname.c_str() <<
				"\' not present.  Cannot delete." << endl;
}

void dalTable::writeDataByColNum( void * data, int index, int rownum )
{
	
	/*
	 * Cleanup to make more efficient.  Check the last three fields for
	 * H5TBget_field_info();
	 */	
	int num_fields 		= 1;	  // number of fields to overwrite
	const int inum		= index;  // column number to overwrite
	const int * index_num	= &inum;  // pointer to column number to overwrite
	hsize_t start		= hsize_t(rownum); // record index to start at
	hsize_t numrecords	= 1;	  // number of records to write

	
	const size_t size = sizeof( data );
	const size_t field_offsets[1] = { 0 };
	const size_t dst_sizes[1] = { sizeof( int ) }; //(update)
	
  	status = H5TBwrite_fields_index(file_id, name.c_str(), num_fields,
					index_num, start, numrecords, size,
					field_offsets, dst_sizes, data);
}

void dalTable::appendRow( void * data )
{
	hsize_t recs2write = 1; // number of records to append
	size_t * field_sizes;
	size_t * field_offsets;
	size_t * size_out;
	
	// retrieve the input fields needed for the append_records call
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	
	field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
	field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
	size_out = (size_t*)malloc( sizeof(size_t) );

	status = H5TBget_field_info( file_id, name.c_str(), NULL, field_sizes,
				     field_offsets, size_out );
	
	if ( firstrecord ) {
		hsize_t start = 0;
		hsize_t numrows = 1;
		status = H5TBwrite_records( file_id, name.c_str(), start, numrows,
					    *size_out, field_offsets, field_sizes,
					    data);
		firstrecord = false;
	}
	else {
		status = H5TBappend_records ( file_id, name.c_str(), recs2write,
						*size_out, field_offsets,
						field_sizes, data );
	}
	free( field_sizes );
	free( field_offsets );
	free( size_out );
}

void dalTable::appendRows( void * data, long row_count )
{
	size_t * field_sizes;
	size_t * field_offsets;
	size_t size_out;

	// retrieve the input fields needed for the append_records call
	H5TBget_table_info( file_id, name.c_str(), &nfields, &nrecords );
	
	field_sizes  = (size_t *)malloc((size_t)nfields * sizeof(size_t));
	field_offsets = (size_t *)malloc((size_t)nfields * sizeof(size_t));

	status = H5TBget_field_info( file_id, name.c_str(), NULL, field_sizes,
					field_offsets, &size_out );
	
	if ( firstrecord ) {
		hsize_t start = 0;
		status = H5TBappend_records ( file_id, name.c_str(),
					      (hsize_t)row_count, size_out,
					      field_offsets, field_sizes, data );
		status = H5TBwrite_records( file_id, name.c_str(), start,
					    (hsize_t)row_count, size_out,
					    field_offsets, field_sizes, data );
		firstrecord = false;
	}
	else {
		status = H5TBappend_records ( file_id, name.c_str(),
					      (hsize_t)row_count, size_out,
					      field_offsets, field_sizes, data );
	}
	free( field_sizes );
	free( field_offsets );
}

void dalTable::setAttribute_string( string attrname, string data ) {
	if ( H5LTset_attribute_string( file_id, name.c_str(),
					attrname.c_str(), data.c_str() ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalTable::setAttribute_int( string attrname, int * data, int size ) {
	if ( H5LTset_attribute_int( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalTable::setAttribute_uint( string attrname, unsigned int * data, int size ) {
	if ( H5LTset_attribute_uint( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalTable::setAttribute_double( string attrname, double * data, int size ) {
	if ( H5LTset_attribute_double( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
}

void dalTable::listColumns( /*void * data_out, long nstart, long numberRecs*/ )
{
	char** field_names;
	size_t * field_sizes;
	size_t * field_offsets;
	size_t * size_out;
 	
	// retrieve the input fields needed for the append_records call
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	
	field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
	field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
	size_out = (size_t*)malloc( sizeof(size_t) );

	/* Alocate space */ 
	field_names = (char**)malloc( sizeof(char*) * (size_t)nfields );
	for ( unsigned int i = 0; i < nfields; i++) { 
	 	  field_names[i]=(char*)malloc(sizeof(char)*HLTB_MAX_FIELD_LEN );
 	 }
	status = H5TBget_field_info( file_id, name.c_str(), field_names,
				     field_sizes, field_offsets, size_out );

	for (unsigned int ii=0; ii<nfields; ii++) {
		cout << setw(11) << field_names[ii];
	}
	cout << endl;
}

long dalTable::getNumberOfRows()
{
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	return nrecords;
}

void dalTable::readRows( void * data_out, long nstart, long numberRecs )
{
	size_t * field_sizes;
	size_t * field_offsets;
	size_t * size_out;
 	
	// retrieve the input fields needed for the append_records call
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	
	field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
	field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
	size_out = (size_t*)malloc( sizeof(size_t) );

	status = H5TBget_field_info( file_id, name.c_str(), field_names,
				     field_sizes, field_offsets, size_out );
 	hsize_t start = nstart;
 	hsize_t nrecs = numberRecs;

	status = H5TBread_records( file_id, name.c_str(), start, nrecs,
				   size_out[0], field_offsets, field_sizes,
				   data_out );
	
	if (status < 0) {
		cout << "Problem reading records. Row buffer may be too big. "
		     << "Make sure the buffer is smaller than the size of the "
		     << "table." << endl;
		exit(897);
	}
}

void dalTable::getAttribute( string attrname ) {

	hsize_t * dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute( table_id, attrname.c_str() ) <= 0 ) {
		cout << "Attribute " << attrname << " not found." << endl;
		return;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims(file_id,fullname.c_str(),attrname.c_str(),&rank );

	dims = (hsize_t *)malloc(rank * sizeof(hsize_t));

	H5LTget_attribute_info( file_id, fullname.c_str(), attrname.c_str(),
				dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		double data[*dims];
		H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
			 H5T_NATIVE_DOUBLE, data);
		cout << attrname << " = ";
		for (unsigned int ii=0; ii<*dims; ii++) {
		  cout << data[ii];
		  if (ii < (*dims)-1)
		    cout << ',';
		  else
		    cout << endl;
		}
	}
	else if ( H5T_INTEGER == type_class ) {
		int data[*dims];
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), attrname.c_str(),
									H5T_NATIVE_INT, data) )
		{
			cout << attrname << " not found" << endl;
		}
		else
		{
			cout << attrname << " = ";
			for (unsigned int ii=0; ii<*dims; ii++) {
			  cout << data[ii];
			  if (ii < (*dims)-1)
				cout << ',';
			  else
				cout << endl;
			}
		}
	}
	else if ( H5T_STRING == type_class ) {
		char* data;
		string fullname = "/" + name;
		data = (char *)malloc(rank * sizeof(char));
		H5LTget_attribute_string( file_id, fullname.c_str(),
					  attrname.c_str(),data);
		cout << attrname << " = " << data << endl;
	}
	else {
		cout << "Attribute " << attrname << " type unknown." << endl;
	}
}
