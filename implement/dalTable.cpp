/*-------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2007 by Joseph Masters                                  *
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

/****************************************************************
 *  Default constructor
 *
 *****************************************************************/
dalTable::dalTable()
{
    filter = new dalFilter;
}

/****************************************************************
 *  (experimental)  Return the column data
 *
 *****************************************************************/
dalColumn * dalTable::getColumn( string colname )
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
   // using the dalColumn class
	dalColumn * lclcol;
	lclcol = new dalColumn( *casa_table_handle, colname );
    #ifdef DEBUGGING_MESSAGES
	lclcol->getType();
	if ( lclcol->isScalar() )
	  cout << colname << " is SCALAR" << endl;
	if ( lclcol->isArray() )
	  cout << colname << " is ARRAY" << endl;
    #endif
	return lclcol;
#endif
   }
   else if ( type == H5TYPE )
   {
      cerr << "ERROR: hdf5 not yet supported for this function."
           << " Try getColumn_Float32, etc." << endl;
   }
   return NULL;
}

/****************************************************************
 *  (experimental)  Return the column data
 *
 *****************************************************************/
dalColumn * dalTable::getColumn_Float32( string colname )
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
   // using the dalColumn class
	dalColumn * lclcol;
	lclcol = new dalColumn( *casa_table_handle, colname );
    #ifdef DEBUGGING_MESSAGES
	lclcol->getType();
	if ( lclcol->isScalar() )
	  cout << colname << " is SCALAR" << endl;
	if ( lclcol->isArray() )
	  cout << colname << " is ARRAY" << endl;
    #endif
	return lclcol;
#endif
   }
   else if ( type == H5TYPE )
   {
      dalColumn * lclcol;
      lclcol = new dalColumn( file_id, table_id, H5TYPE, name, colname,
                              dal_FLOAT );

//	H5TBread_fields_name(file_id, name.c_str(), colname.c_str(), start, hsize_t nrecords, size_t type_size,  const size_t *field_offset, const size_t *field_sizes, void  *data);

	return lclcol;
   }
   return NULL;
}

/****************************************************************
 *  (experimental)  Return the column data
 *
 *****************************************************************/
dalColumn * dalTable::getColumn_complexFloat32( string colname )
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
   // using the dalColumn class
	dalColumn * lclcol;
	lclcol = new dalColumn( *casa_table_handle, colname );
    #ifdef DEBUGGING_MESSAGES
	lclcol->getType();
	if ( lclcol->isScalar() )
	  cout << colname << " is SCALAR" << endl;
	if ( lclcol->isArray() )
	  cout << colname << " is ARRAY" << endl;
    #endif
	return lclcol;
#endif
   }
   else if ( type == H5TYPE )
   {
      dalColumn * lclcol;
      lclcol = new dalColumn( file_id, table_id, H5TYPE, name, colname,
                              dal_COMPLEX );

//	H5TBread_fields_name(file_id, name.c_str(), colname.c_str(), start, hsize_t nrecords, size_t type_size,  const size_t *field_offset, const size_t *field_sizes, void  *data);

	return lclcol;
   }
   return NULL;
}

void dalTable::setFilter( string columns )
{
    filter = new dalFilter;
    filter->setFiletype( type );
    filter->set(columns);
}

void dalTable::setFilter( string columns, string conditions )
{
    filter->setFiletype( type );
    filter->set(columns,conditions);
}

/****************************************************************
 *  (experimental)  Return the column data
 *
 *****************************************************************/
void * dalTable::getColumnData( string colname )
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
   // using the dalColumn class
	dalColumn lclcol;
	lclcol = dalColumn( *casa_table_handle, colname );
	lclcol.getType();
    #ifdef DEBUGGING_MESSAGES
	if ( lclcol.isScalar() )
	  cout << colname << " is SCALAR" << endl;
	if ( lclcol.isArray() )
	  cout << colname << " is ARRAY" << endl;
    #endif
	casa::uInt nrow;
	nrow = casa_table_handle->nrow();

	// create a column object
	casa::ROTableColumn column( *casa_table_handle, colname );

	// get the column descriptor
	casa::ColumnDesc col_desc = column.columnDesc();

	cout << "-------\n" << col_desc.name() << "\n-------" << endl;
	cout << "Is the column an array? ";
	casa::Bool isarray = col_desc.isArray();
	if ( casa::True == isarray )
	{
	  #ifdef DEBUGGING_MESSAGES
	  cout << "YES" << endl;
      #endif
	  casa::uInt ndimcol;
	  ndimcol = column.ndimColumn();
	  casa::IPosition shape = column.shape(1);
      #ifdef DEBUGGING_MESSAGES
	  cout << "  Number of global dims: " << ndimcol << endl;
	  cout << "  Shape: " << shape << endl;
      #endif
	  switch ( col_desc.dataType() )
	  {
	    case casa::TpComplex:
	    {
          #ifdef DEBUGGING_MESSAGES
	      cout << "Data type is Complex." << endl;
          #endif
	      casa::ROArrayColumn<casa::Complex>
	        arcolumn( *casa_table_handle, colname );
	      int polarization = 0;
	      casa::IPosition start (2,polarization,0)/*, length (1,1)*/;
	      casa::Slicer slicer (start/*, length*/);
	      array_vals_comp = arcolumn.getColumn( slicer );
          #ifdef DEBUGGING_MESSAGES
	      cout << "number of dims: " << array_vals_comp.ndim() << endl;
	      cout << "shape: " << array_vals_comp.shape() << endl;
	      cout << "size: " << array_vals_comp.size() << endl;
          #endif
	      vector< complex< float > > valvec;
	      array_vals_comp.tovector( valvec );
          #ifdef DEBUGGING_MESSAGES
	      cout << valvec[0] << valvec[1] << valvec[2] << endl;
	      cout << "vector size: " << valvec.size() << endl;
	      cout << "Polarization number: " << polarization << endl;
          #endif
	      return array_vals_comp.data();
	    }
	    break;
	    case casa::TpDComplex:
	      cout << "Data type is DComplex." << endl;
	    break;
	    case casa::TpBool:
	      cout << "Data type is Boolean." << endl;
	    break;
	    case casa::TpChar:
	      cout << "Data type is Char." << endl;
	    break;
	    case casa::TpShort:
	      cout << "Data type is Short." << endl;
	    break;
	    case casa::TpFloat:
	      cout << "Data type is Float." << endl;
	    break;
	    case casa::TpDouble:
	    {
	      cout << "Data type is Double." << endl;
	      casa::ROArrayColumn<casa::Double>
	        arcolumn( *casa_table_handle, colname );
	      int cell = 0;
	      casa::IPosition start (1,cell)/*, length (1,1)*/;
	      casa::Slicer slicer (start/*, length*/);
	      array_vals_dbl = arcolumn.getColumn( slicer );
          #ifdef DEBUGGING_MESSAGES
	      cout << "number of dims: " << array_vals_dbl.ndim() << endl;
	      cout << "shape: " << array_vals_dbl.shape() << endl;
	      cout << "size: " << array_vals_dbl.size() << endl;
          #endif
	      vector<double> valvec;
	      array_vals_dbl.tovector( valvec );
          #ifdef DEBUGGING_MESSAGES
	      cout << "vector size: " << valvec.size() << endl;
	      cout << "Data from cell number: " << cell << endl;
          #endif
	      return array_vals_dbl.data();
	    }
	    break;
	    case casa::TpString:
	      cout << "Data type is String." << endl;
	    break;
	    case casa::TpInt:
	    {
          #ifdef DEBUGGING_MESSAGES
	      cout << "Data type is Int." << endl;
          #endif
	      int * value = new int[100];
	      for (int ii=0; ii<100; ii++)
	      {
			column.getScalar(ii,value[ii]);
		  }
		  return value;
	    }
	    break;
	    default:
	      cout << "Datatype not recognized." << endl;
	  }
	}
	else
	  cout << "NO" << endl;

	if (col_desc.isScalar())
	{
	  cout << "isScalar" << endl;
	  switch ( col_desc.dataType() )
	  {
	    case casa::TpComplex:
	      cout << "Data type is Complex." << endl;
	    break;
	    case casa::TpBool:
	      cout << "Data type is Boolean." << endl;
	    break;
	    case casa::TpChar:
	      cout << "Data type is Char." << endl;
	    break;
	    case casa::TpShort:
	      cout << "Data type is Short." << endl;
	    break;
	    case casa::TpFloat:
	      cout << "Data type is Float." << endl;
	    break;
	    case casa::TpDouble:
	    {
	      cout << "Data type is Double." << endl;
	      double * value = new double[100];
	      for (int ii=0; ii<100; ii++)
		  {
		    column.getScalar(ii,value[ii]);
		  }
          return value;
	    }
	    break;
	    case casa::TpString:
	      cout << "Data type is String." << endl;
	    break;
	    case casa::TpInt:
	    {
	      cout << "Data type is Int." << endl;
	      int * value = new int[100];
	      for (int ii=0; ii<100; ii++)
		  {
		    column.getScalar(ii,value[ii]);
		  }
		  return value;
	    }
	    break;
/*	  case casa::TpArrayShort:
	  cout << "Data type is ArrayShort." << endl;
	  break;
	  case casa::TpArrayDouble:
	  cout << "Data type is ArrayDouble." << endl;
	  break;
	  case casa::TpArrayComplex:
	  cout << "Data type is ArrayComplex." << endl;
	  break;
	  case casa::TpTable:
	  cout << "Data type is Table." << endl;
	  break;*/
	    default:
	      cout << "Datatype not recognized." << endl;
	  }
	}
	else if (col_desc.isTable())
	{
		cout << "isTable" << endl;
	}

	casa::TableRecord table_rec = column.keywordSet();
    #ifdef DEBUGGING_MESSAGES
 	cout << "Number of fields: " << table_rec.nfields() << endl;
 	cout << "Datatype: " << col_desc.dataType() << endl;
    #endif
	return NULL;
#else
	cout << "CASA support not enabled." << endl;
	return NULL;
#endif
    }
   else
   {
	cout << "dalTable::getColumnData operation not supported for type "
	  << type << endl;
	return NULL;
   }
}

/****************************************************************
 *  (experimental) Print the name of the table
 *
 *****************************************************************/
void dalTable::getName()
{
   if ( type == MSCASATYPE )
   {
	// print the name of the table
#ifdef WITH_CASA
	cout << "CASA table name: " << casa_table_handle->tableName() << endl;
#else
	cout << "CASA support not enabled." << endl;
#endif
   }
   else
   {
	cout << "dalTable::getName operation not supported for type "
	  << type << endl;
   }
}

/**************************************************************************//*!
 *
 *  Create a new table object
 *
 *  @param filetype The type to work with.
 *         Typically "MSCASA" for this function.
 *
 *****************************************************************************/
dalTable::dalTable( string filetype )
{
    filter = new dalFilter;

	type = filetype;
 	columns.clear();  // clear the coulumns vector
 	firstrecord = true;

   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
	casa_table_handle = new casa::Table();
#else
	cout << "CASA support not enabled." << endl;
#endif
   }
}

#ifdef WITH_CASA
/****************************************************************
 *  Open a CASA table (not in a MS)
 *
 *****************************************************************/
void dalTable::openTable( string tablename )
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
   	if ( filter->isSet() )
	{
	  *casa_table_handle = casa::Table( tablename );
	  *casa_table_handle = casa::tableCommand( filter->get(),
            *casa_table_handle );
	}
	else
	{
	  *casa_table_handle = casa::Table( tablename );
	}
#else
	cout << "CASA support not enabled." << endl;
	exit(-1);
#endif
   }
   else
   {
	cout << "dalTable::openTable operation not supported for type "
	  << type << endl;
   }
}

/****************************************************************
 *  Open a CASA table
 *
 *****************************************************************/
void dalTable::openTable( string tablename, casa::MSReader * reader )
{
   if ( type == MSCASATYPE )
   {
	*casa_table_handle = reader->table( tablename );
   }
   else
   {
	cout << "dalTable::openTable operation not supported for type "
	  << type << endl;
   }
}

/****************************************************************
 *  Open a filtered CASA table
 *
 *****************************************************************/
void dalTable::openTable( string tablename,
   casa::MSReader * reader, dalFilter * filter )
{
   if ( type == MSCASATYPE )
   {
	*casa_table_handle = reader->table( tablename );
	*casa_table_handle = casa::tableCommand( filter->get(),
            *casa_table_handle );
   }
   else
   {
	cout << "dalTable::openTable operation not supported for type "
	  << type << endl;
   }
}
#endif

/****************************************************************
 *  Open a table (mainly for hdf5)
 *
 *****************************************************************/
void dalTable::openTable( void * voidfile, string tablename, string groupname )
{
   if ( type == H5TYPE )
   {
	name = groupname + '/' + tablename;
	hid_t * lclfile = (hid_t*)voidfile; // H5File object
	file = lclfile;
	file_id = *lclfile;  // get the file handle

	table_id = H5Dopen ( file_id, name.c_str() );
   }
   else
   {
	cout << "dalTable::openTable operation not supported for type "
	  << type << endl;
   }
}

/****************************************************************
 *  Default destructor
 *
 *****************************************************************/
dalTable::~dalTable()
{
	/* broken in HDF5 library itself! */
	//  	if ( nrecords > 1 )
	//		H5TBdelete_record (file_id, name.c_str(), 0, 1);
}

/****************************************************************
 *  print the columns in the table
 *
 *****************************************************************/
void dalTable::printColumns()
{
   if ( type == H5TYPE )
   {
	// Print the column list by iterating through the global column vector
	cout << "Columns vector list:" << endl;
	for (unsigned int ii=0; ii < columns.size(); ii++)
		cout << columns[ii].getName() << " " << endl;
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  (currently assumes HDF5) Create a table in a group
 *
 *****************************************************************/
void dalTable::createTable( void * voidfile, string tablename, string groupname )
{
   if ( type == H5TYPE )
   {
	/* It is necessary to have at least one column for table creation
	 * so we create a dummy column that will be deleted when real
	 * columns are added
	 */
	
	int			dummy_col;
	name = groupname + '/' + tablename;// set the private class variable: name
	
	// cast the voidfile to an hdf5 file
	hid_t * lclfile = (hid_t*)voidfile; // H5File object
	file = lclfile;
	file_id = *lclfile;  // get the file handle

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
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}


/****************************************************************
 *  (currently assumes HDF5) Add a column to a table
 *
 *****************************************************************/
void dalTable::addColumn( string colname, string coltype, int size )
{

   if ( type == H5TYPE )
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
	else if ( ( dal_COMPLEX_CHAR == coltype ) ) 
	{
		vector<dalColumn> cv;
		string component_type;
		
		component_type = dal_CHAR;
			
		dalColumn col_a( "r", component_type );  // real component
		dalColumn col_b( "i", component_type );  // imaginary component

		cv.push_back( col_a );
		cv.push_back( col_b );

		addComplexColumn( colname, cv, 2 );

		return;
	}
	else if ( ( dal_COMPLEX_SHORT == coltype ) ) 
	{
		vector<dalColumn> cv;
		string component_type;
		
		component_type = dal_SHORT;
			
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
	if ( -1 == size )  // -1 for variable length data
	{
		if ( dal_CHAR == coltype )
			field_type_new = H5Tvlen_create (H5T_NATIVE_CHAR);

		else if ( dal_INT == coltype )
			field_type_new = H5Tvlen_create (H5T_NATIVE_INT);
			
		else if ( dal_UINT == coltype )
			field_type_new = H5Tvlen_create (H5T_NATIVE_UINT);
	
		else if ( dal_SHORT == coltype )
			field_type_new = H5Tvlen_create (H5T_NATIVE_SHORT);
	
		else if ( dal_FLOAT == coltype )
			field_type_new = H5Tvlen_create (H5T_NATIVE_FLOAT);
	
		else if ( dal_DOUBLE == coltype )
			field_type_new = H5Tvlen_create (H5T_NATIVE_DOUBLE);
			
		else {
			cout << "ERROR: column type " << coltype <<
			  " is not supported." << endl;
			exit(98);
		}
	} else {
		if ( dal_CHAR == coltype )
			field_type_new = H5T_NATIVE_CHAR;

		else if ( dal_INT == coltype )
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
			cout << "ERROR: column type " << coltype <<
			  " is not supported." << endl;
			exit(99);
		}
	}
	
	// set additional required fields for new column call
	hsize_t	position = nfields;

	// create the new column
	status = H5TBinsert_field( file_id, name.c_str(), colname.c_str(),
				   field_type_new, position, NULL, NULL );

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

   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
herr_t attr_info(hid_t loc_id, const char *name, void *opdata);
void dalTable::getAttributes() {
   if ( type == H5TYPE )
   {

   //status = H5Aget_num_attrs(group_id);
   //printf ("H5Aget_num_attrs returns: %i\n", status);
   status = H5Aiterate( table_id, NULL, attr_info, NULL );
   //printf ("\nH5Aiterate returns: %i\n", status);

   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::addArrayColumn( string colname, string coltype, unsigned int indims )
{
   if ( type == H5TYPE )
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
	bool removedummy = false;
	for (unsigned int ii=0; ii<nfields; ii++) {
		if (0 == strcmp(colname.c_str(),field_names[ii])) {
			cout << "WARNING: Cannot create column \'"
			     << colname.c_str()
			     << "\'. Column already exists." << endl;
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
	hsize_t dd = indims;
	hsize_t * dims = &dd;
	hid_t	field_type;	
	hid_t   h5type;
	
	if ( dal_CHAR == coltype )
		h5type =  H5T_NATIVE_CHAR;

	else if ( dal_INT == coltype )
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
	
	else if ( dal_COMPLEX_CHAR == coltype )
	{
		vector<dalColumn> cv;
		string component_type;
		
		component_type = dal_CHAR;
			
		dalColumn col_a( "r", component_type );  // real component
		dalColumn col_b( "i", component_type );  // imaginary component

		cv.push_back( col_a );
		cv.push_back( col_b );

		size_t sz = 0;
		
		// compute the size of the compound column
		for ( unsigned int ii=0; ii<cv.size(); ii++) {
			//cout << "subcolumn name is " << foo[ii].getName() 
			//	 << ". Type is " << foo[ii].getType() << endl;
			sz += cv[ii].getSize();
		}
	
		// create a compound type that can hold each field
		h5type = H5Tcreate( H5T_COMPOUND, sz );
	
		size_t offset = 0;
		for ( unsigned int ii=0; ii<cv.size(); ii++) {
			
			if (0==ii)
				offset=0;
			else
				offset += cv[ii-1].getSize();
				
	
			if ( dal_CHAR == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_CHAR);
			}
			else if ( dal_INT == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_INT);
			}
			else if ( dal_FLOAT == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_FLOAT);
			}
			else if ( dal_DOUBLE == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_DOUBLE);
			}
		}
	}	

	else if ( dal_COMPLEX == coltype )
	{
		vector<dalColumn> cv;
		string component_type;
		
		component_type = dal_DOUBLE;
			
		dalColumn col_a( "r", component_type );  // real component
		dalColumn col_b( "i", component_type );  // imaginary component

		cv.push_back( col_a );
		cv.push_back( col_b );

		size_t sz = 0;
		
		// compute the size of the compound column
		for ( unsigned int ii=0; ii<cv.size(); ii++) {
			//cout << "subcolumn name is " << foo[ii].getName() 
			//	 << ". Type is " << foo[ii].getType() << endl;
			sz += cv[ii].getSize();
		}
	
		// create a compound type that can hold each field
		h5type = H5Tcreate( H5T_COMPOUND, sz );
	
		size_t offset = 0;
		for ( unsigned int ii=0; ii<cv.size(); ii++) {
			
			if (0==ii)
				offset=0;
			else
				offset += cv[ii-1].getSize();
				
	
			if ( dal_CHAR == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_CHAR);
			}
			else if ( dal_INT == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_INT);
			}
			else if ( dal_FLOAT == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_FLOAT);
			}
			else if ( dal_DOUBLE == cv[ii].getType() ) {
				H5Tinsert( h5type,
					cv[ii].getName().c_str(), offset,
					H5T_NATIVE_DOUBLE);
			}
		}
	}	
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

	if ( removedummy )
		removeColumn("000dummy000");
	
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::addComplexColumn( string compname, vector<dalColumn> foo,
				int subfields )
{
   if ( type == H5TYPE )
   {
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
		if (0 == strcmp(compname.c_str(),field_names[ii])) {
			cout << "WARNING: Cannot create column \'"
			     << compname.c_str()
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

	size_t sz = 0;
	
	// compute the size of the compound column
	for ( unsigned int ii=0; ii<foo.size(); ii++) {
// 		cout << "subcolumn name is " << foo[ii].getName() 
// 			 << ". Type is " << foo[ii].getType() << endl;
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
			
		if ( dal_CHAR == foo[ii].getType() ) {
			H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
				   H5T_NATIVE_CHAR);
		}
		else if ( dal_SHORT == foo[ii].getType() ) {
			H5Tinsert( fieldtype, foo[ii].getName().c_str(), offset,
				   H5T_NATIVE_SHORT);
		}
		else if ( dal_INT == foo[ii].getType() ) {
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
	status = H5TBinsert_field( file_id, name.c_str(), compname.c_str(),
					fieldtype, position,
					fill_data, data );	

	if ( removedummy )
		removeColumn("000dummy000");

	return;

	// if successful, add corresponding column object to list
	if ( 0 == status ) {
	  	dalColumn * lc = new dalColumn( compname, "foo" /*H5_COMPOUND*/ );
		
		// add new dalColumn to the 'columns' vector
	  	columns.push_back( *lc );

	 	lc->addMember( "a_member", dal_INT );
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }

}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::removeColumn(string colname)
{	
   if ( type == H5TYPE )
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
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::writeDataByColNum( void * data, int index, int rownum )
{
   if ( type == H5TYPE )
   {
	
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
// 	const size_t field_offsets[1] = { 0 };
// 	const size_t dst_sizes[1] = { sizeof( int ) }; //(update)
	
  	status = H5TBwrite_fields_index(file_id, name.c_str(), num_fields,
					index_num, start, numrecords, size,
					field_offsets, field_sizes, data);
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::appendRow( void * data )
{
   if ( type == H5TYPE )
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
		status = H5TBwrite_records( file_id, name.c_str(), start,
			   numrows, *size_out, field_offsets, field_sizes,
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
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::appendRows( void * data, long row_count )
{
   if ( type == H5TYPE )
   {
	size_t * field_sizes;
	size_t * field_offsets;
	size_t * size_out;

	// retrieve the input fields needed for the append_records call
	H5TBget_table_info( file_id, name.c_str(), &nfields, &nrecords );
	
	field_sizes  = (size_t *)malloc((size_t)nfields * sizeof(size_t));
	field_offsets = (size_t *)malloc((size_t)nfields * sizeof(size_t));
	size_out = (size_t*)malloc( sizeof(size_t) );

	status = H5TBget_field_info( file_id, name.c_str(), NULL, field_sizes,
					field_offsets, size_out );
	
	if ( firstrecord ) {
		hsize_t start = 0;
		if (row_count>1)
		{
		  status = H5TBappend_records ( file_id, name.c_str(),
					      (hsize_t)row_count-1, *size_out,
					      field_offsets, field_sizes, data);
		}
		status = H5TBwrite_records( file_id, name.c_str(), start,
					    (hsize_t)row_count, *size_out,
					    field_offsets, field_sizes, data );
		firstrecord = false;
	}
	else {
		status = H5TBappend_records ( file_id, name.c_str(),
					      (hsize_t)row_count, *size_out,
					      field_offsets, field_sizes, data);
	}
	free( field_sizes );
	free( field_offsets );
	free( size_out );
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::setAttribute_string( string attrname, string data ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_string( file_id, name.c_str(),
					attrname.c_str(), data.c_str() ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::setAttribute_char( string attrname, char * data, int size ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_char( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::setAttribute_int( string attrname, int * data, int size ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_int( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::setAttribute_uint(string attrname, unsigned int * data, int size)
{
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_uint( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::setAttribute_double( string attrname, double * data, int size ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_double( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cout << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::listColumns( /*void * data_out, long nstart, long numberRecs*/ )
{
   if ( type == H5TYPE )
   {
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
		cout << setw(17) << field_names[ii];
	}
	cout << endl;
   }
   else if ( type == MSCASATYPE )
   {
     casa::TableDesc td = casa_table_handle->tableDesc();
     cout << td.columnNames() << endl;
   }
   else
   {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }

}

/****************************************************************
 *  
 *
 *****************************************************************/
long dalTable::getNumberOfRows()
{
   if ( type == H5TYPE )
   {
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	return nrecords;

   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
	 return(-1);
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::readRows( void * data_out, long nstart, long numberRecs, long buffersize )
{
   if ( type == H5TYPE )
   {
        size_t * field_sizes;
        size_t * field_offsets;
        size_t * size_out;

        // retrieve the input fields needed for the append_records call
        H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );

        field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
        field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
        size_out = (size_t*)malloc( sizeof(size_t) );
        field_names = (char**)malloc( nfields * sizeof(char*) );
        for (unsigned int ii=0; ii<nfields; ii++) {
                field_names[ii] = (char*)malloc(MAX_COL_NAME_SIZE*sizeof(char));
        }

        status = H5TBget_field_info( file_id, name.c_str(), field_names,
                                     field_sizes, field_offsets, size_out );

        hsize_t start = nstart;
        hsize_t nrecs = numberRecs;

        if (buffersize > 0)
                size_out[0] = buffersize;
        status = H5TBread_records( file_id, name.c_str(), start, nrecs,
                                   size_out[0], field_offsets, field_sizes,
                                   data_out );

	free(field_sizes);
	free(field_offsets);
	free(size_out);
	for (unsigned int ii=0; ii<nfields; ii++) {
		free(field_names[ii]);
	}

        if (status < 0) {
                cout << "Problem reading records. Row buffer may be too big. "
                     << "Make sure the buffer is smaller than the size of the "
                     << "table." << endl;
                exit(897);
        }
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
bool dalTable::findAttribute( string attrname )
{
   if ( type == H5TYPE )
   {
	if ( H5LT_find_attribute( table_id, attrname.c_str() ) <= 0 ) {
		cout << "Attribute " << attrname << " not found." << endl;
		return false;
	} else {
		return true;
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
	 return false;
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::printAttribute( string attrname ) {

   if ( type == H5TYPE )
   {
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
	H5LTget_attribute_ndims( file_id, fullname.c_str(),
	  attrname.c_str(), &rank );

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
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), 
		  attrname.c_str(), H5T_NATIVE_INT, data) )
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
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  (currently assumes hdf5) return the value of the (scalar) attribute
 *
 *****************************************************************/
void * dalTable::getAttribute( string attrname ) {

   if ( type == H5TYPE )
   {
	hsize_t dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute( table_id, attrname.c_str() ) <= 0 ) {
		return NULL;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims( file_id, fullname.c_str(),
	  attrname.c_str(), &rank );

	H5LTget_attribute_info( file_id, fullname.c_str(), attrname.c_str(),
				&dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		void * data;
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), 
		  attrname.c_str(),
			 H5T_NATIVE_DOUBLE, data) )
		  return NULL;
		else
		  return reinterpret_cast<double*>(data);
	}
	else if ( H5T_INTEGER == type_class ) {
		void * data;
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(),
		  attrname.c_str(),
			H5T_NATIVE_INT, data) )
		  return NULL;
		else
		  return reinterpret_cast<int*>(data);
	}
	else if ( H5T_STRING == type_class ) {
		char* data;
		string fullname = "/" + name;
		data = (char *)malloc(rank * sizeof(char));
		if ( 0 < H5LTget_attribute_string( file_id, fullname.c_str(),
			  attrname.c_str(),data) )
		  return NULL;
		else
		  return data;
	}
	else
	{
		return NULL;
	}
   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
     return NULL;
   }
}

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON

/****************************************************************
 *  wrapper for openTable (hdf5)
 *
 *****************************************************************/
void dalTable::ot_hdf5( void * voidfile, string tablename, string groupname )
{
   openTable( voidfile, tablename, groupname );
}

/****************************************************************
 *  wrapper for appendRow (hdf5)
 *
 *****************************************************************/
void dalTable::append_row_boost( bpl::object data )
{
  appendRow(&data);
}

/****************************************************************
 *  wrapper for getColumnData
 *
 *****************************************************************/
// bpl::numeric::array dalTable::gcd_boost( string arrayname )
// {

/*double * mytime_data = new double[ 100 ];
mytime_data = (double *)maintable->getColumnData( "TIME" );
for (int ii=0; ii<10; ii++)
  cout << "TIME data out: " << mytime_data[ii] << endl;
delete mytime_data;

double * myuvw;
myuvw = (double *)maintable->getColumnData( "UVW" );
// cout << myuvw << endl;
for (int ii=0; ii<30; ii++)
   cout << "UVW data out: " << myuvw[ii] << endl;

complex<float> * data;
data = (complex<float> *)maintable->getColumnData( "DATA" );
for (int ii=0; ii<30; ii++)
   cout << "DATA out: " << data[ii] << endl;

	hid_t lclfile;
	hid_t  status;
// 	hid_t datatype, dataspace;

	// get the dataspace
	lclfile = H5Dopen(h5fh, arrayname.c_str());
	hid_t filespace = H5Dget_space(lclfile);

	// what is the rank of the array?
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
	hsize_t dims[ data_rank ];
// cout << "data rank: " << data_rank << endl;
	status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	int size = 1;
	bpl::list dims_list;
	for (int ii=0; ii<data_rank; ii++)
	{
// cout << "dims["  << ii << "]: " << dims[ii] << endl;
	  size *= dims[ii];
	  dims_list.append(dims[ii]);
	}
// cout << "size: " << size << endl;

	int * data = NULL;
	data = new int[size];

	status = H5LTread_dataset_int( h5fh, arrayname.c_str(), data );
// 	for (int ii=0; ii<size; ii++)
// 	{
// 	  cout << data[ii] << endl;
// 	}

	bpl::list data_list;
	// for each dimension
	for (int ii=0; ii<size; ii++)
	{
	    data_list.append(data[ii]);
	}
	bpl::numeric::array nadata(
//           bpl::make_tuple(
	    bpl::make_tuple(data_list)
// 	  )
	);
// 	dims_list.reverse();
	nadata.setshape(dims_list);
	delete data;
	return nadata;*/
// }

#ifdef WITH_CASA
/****************************************************************
 *  wrapper for openTable (hdf5)
 *
 *****************************************************************/
void dalTable::ot_nonMStable( string tablename )
{
   openTable( tablename );
}

/****************************************************************
 *  wrappers for openTable (casa)
 *
 *****************************************************************/
/*void dalTable::ot_ms1( string tablename, casa::MSReader * reader)
{
   openTable( tablename, reader );
}*/
// void dalTable::ot_ms2( string tablename, casa::MSReader * reader,
//   string parse_string )
// {
//    openTable( tablename, reader, parse_string );
// }
/******************************************************
 * wrappers for setFilter
 ******************************************************/
void dalTable::setFilter_boost1( string columns )
{
   setFilter( columns );
}

void dalTable::setFilter_boost2( string columns, string conditions )
{
   setFilter( columns, conditions );
}

#endif

#endif
