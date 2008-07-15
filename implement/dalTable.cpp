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

namespace DAL {

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
       dalColumn * lclcol = NULL;
       lclcol = new dalColumn( *casa_table_handle, colname );

       #ifdef DEBUGGING_MESSAGES
       lclcol->getType();
       if ( lclcol->isScalar() )
         cerr << colname << " is SCALAR" << endl;
       if ( lclcol->isArray() )
         cerr << colname << " is ARRAY" << endl;
       #endif

       return lclcol;

     #else

       cerr << "ERROR: casacore not installed" << endl;

       // prevent unsused var build warning when casacore isn't installed
       colname = colname;
       return NULL;
     #endif
   }
   else if ( type == H5TYPE )
   {
      cerr << "ERROR: hdf5 not yet supported for this function."
           << " Try getColumn_Float32, etc." << endl;
      return NULL;
   }
   else
   {
      return NULL;
   }
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
	dalColumn * lclcol = NULL;
	lclcol = new dalColumn( *casa_table_handle, colname );
    #ifdef DEBUGGING_MESSAGES
	lclcol->getType();
	if ( lclcol->isScalar() )
	  cerr << colname << " is SCALAR" << endl;
	if ( lclcol->isArray() )
	  cerr << colname << " is ARRAY" << endl;
    #endif
	return lclcol;
#endif
   }
   else if ( type == H5TYPE )
   {
      dalColumn * lclcol;
      lclcol = new dalColumn( file_id, table_id, H5TYPE, name, colname,
                              dal_FLOAT );

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
	dalColumn * lclcol = NULL;
	lclcol = new dalColumn( *casa_table_handle, colname );
    #ifdef DEBUGGING_MESSAGES
	lclcol->getType();
	if ( lclcol->isScalar() )
	  cerr << colname << " is SCALAR" << endl;
	if ( lclcol->isArray() )
	  cerr << colname << " is ARRAY" << endl;
    #endif
	return lclcol;
#endif
   }
   else if ( type == H5TYPE )
   {
      dalColumn * lclcol;
      lclcol = new dalColumn( file_id, table_id, H5TYPE, name, colname,
                              dal_COMPLEX );

	return lclcol;
   }
   return NULL;
}

dalColumn * dalTable::getColumn_complexInt16( string colname )
{
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
   // using the dalColumn class
	dalColumn * lclcol = NULL;
	lclcol = new dalColumn( *casa_table_handle, colname );
    #ifdef DEBUGGING_MESSAGES
	lclcol->getType();
	if ( lclcol->isScalar() )
	  cerr << colname << " is SCALAR" << endl;
	if ( lclcol->isArray() )
	  cerr << colname << " is ARRAY" << endl;
    #endif
	return lclcol;
#else
	cerr << "ERROR: casacore not installed" << endl;
	// prevent unused variable build warning if casacore isn't installed
	colname = colname;
	return NULL;
#endif
   }
   else if ( type == H5TYPE )
   {
      dalColumn * lclcol;
      lclcol = new dalColumn( file_id, table_id, H5TYPE, name, colname,
                              dal_COMPLEX_SHORT );
      return lclcol;

   }
   else
   {
      return NULL;
   }
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
	  cerr << colname << " is SCALAR" << endl;
	if ( lclcol.isArray() )
	  cerr << colname << " is ARRAY" << endl;
    #endif
	casa::uInt nrow;
	nrow = casa_table_handle->nrow();

	// create a column object
	casa::ROTableColumn column( *casa_table_handle, colname );

	// get the column descriptor
	casa::ColumnDesc col_desc = column.columnDesc();

	cerr << "-------\n" << col_desc.name() << "\n-------" << endl;
	cerr << "Is the column an array? ";
	casa::Bool isarray = col_desc.isArray();
	if ( casa::True == isarray )
	{
	  #ifdef DEBUGGING_MESSAGES
	  cerr << "YES" << endl;
      #endif
	  casa::uInt ndimcol;
	  ndimcol = column.ndimColumn();
	  casa::IPosition shape = column.shape(1);
      #ifdef DEBUGGING_MESSAGES
	  cerr << "  Number of global dims: " << ndimcol << endl;
	  cerr << "  Shape: " << shape << endl;
      #endif
	  switch ( col_desc.dataType() )
	  {
	    case casa::TpComplex:
	    {
          #ifdef DEBUGGING_MESSAGES
	      cerr << "Data type is Complex." << endl;
          #endif
	      casa::ROArrayColumn<casa::Complex>
	        arcolumn( *casa_table_handle, colname );
	      int polarization = 0;
	      casa::IPosition start (2,polarization,0)/*, length (1,1)*/;
	      casa::Slicer slicer (start/*, length*/);
	      array_vals_comp = arcolumn.getColumn( slicer );
          #ifdef DEBUGGING_MESSAGES
	      cerr << "number of dims: " << array_vals_comp.ndim() << endl;
	      cerr << "shape: " << array_vals_comp.shape() << endl;
	      cerr << "size: " << array_vals_comp.size() << endl;
          #endif
	      vector< complex< float > > valvec;
	      array_vals_comp.tovector( valvec );
          #ifdef DEBUGGING_MESSAGES
	      cerr << valvec[0] << valvec[1] << valvec[2] << endl;
	      cerr << "vector size: " << valvec.size() << endl;
	      cerr << "Polarization number: " << polarization << endl;
          #endif
	      return array_vals_comp.data();
	    }
	    break;
	    case casa::TpDComplex:
	      cerr << "Data type is DComplex." << endl;
	    break;
	    case casa::TpBool:
	      cerr << "Data type is Boolean." << endl;
	    break;
	    case casa::TpChar:
	      cerr << "Data type is Char." << endl;
	    break;
	    case casa::TpShort:
	      cerr << "Data type is Short." << endl;
	    break;
	    case casa::TpFloat:
	      cerr << "Data type is Float." << endl;
	    break;
	    case casa::TpDouble:
	    {
	      cerr << "Data type is Double." << endl;
	      casa::ROArrayColumn<casa::Double>
	        arcolumn( *casa_table_handle, colname );
	      int cell = 0;
	      casa::IPosition start (1,cell)/*, length (1,1)*/;
	      casa::Slicer slicer (start/*, length*/);
	      array_vals_dbl = arcolumn.getColumn( slicer );
          #ifdef DEBUGGING_MESSAGES
	      cerr << "number of dims: " << array_vals_dbl.ndim() << endl;
	      cerr << "shape: " << array_vals_dbl.shape() << endl;
	      cerr << "size: " << array_vals_dbl.size() << endl;
          #endif
	      vector<double> valvec;
	      array_vals_dbl.tovector( valvec );
          #ifdef DEBUGGING_MESSAGES
	      cerr << "vector size: " << valvec.size() << endl;
	      cerr << "Data from cell number: " << cell << endl;
          #endif
	      return array_vals_dbl.data();
	    }
	    break;
	    case casa::TpString:
	      cerr << "Data type is String." << endl;
	    break;
	    case casa::TpInt:
	    {
          #ifdef DEBUGGING_MESSAGES
	      cerr << "Data type is Int." << endl;
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
	      cerr << "Datatype not recognized." << endl;
	  }
	}
	else
	  cerr << "NO" << endl;

	if (col_desc.isScalar())
	{
	  cerr << "isScalar" << endl;
	  switch ( col_desc.dataType() )
	  {
	    case casa::TpComplex:
	      cerr << "Data type is Complex." << endl;
	    break;
	    case casa::TpBool:
	      cerr << "Data type is Boolean." << endl;
	    break;
	    case casa::TpChar:
	      cerr << "Data type is Char." << endl;
	    break;
	    case casa::TpShort:
	      cerr << "Data type is Short." << endl;
	    break;
	    case casa::TpFloat:
	      cerr << "Data type is Float." << endl;
	    break;
	    case casa::TpDouble:
	    {
	      cerr << "Data type is Double." << endl;
	      double * value = new double[100];
	      for (int ii=0; ii<100; ii++)
		  {
		    column.getScalar(ii,value[ii]);
		  }
          return value;
	    }
	    break;
	    case casa::TpString:
	      cerr << "Data type is String." << endl;
	    break;
	    case casa::TpInt:
	    {
	      cerr << "Data type is Int." << endl;
	      int * value = new int[100];
	      for (int ii=0; ii<100; ii++)
		  {
		    column.getScalar(ii,value[ii]);
		  }
		  return value;
	    }
	    break;
/*	  case casa::TpArrayShort:
	  cerr << "Data type is ArrayShort." << endl;
	  break;
	  case casa::TpArrayDouble:
	  cerr << "Data type is ArrayDouble." << endl;
	  break;
	  case casa::TpArrayComplex:
	  cerr << "Data type is ArrayComplex." << endl;
	  break;
	  case casa::TpTable:
	  cerr << "Data type is Table." << endl;
	  break;*/
	    default:
	      cerr << "Datatype not recognized." << endl;
	  }
	}
	else if (col_desc.isTable())
	{
		cerr << "isTable" << endl;
	}

	casa::TableRecord table_rec = column.keywordSet();
    #ifdef DEBUGGING_MESSAGES
 	cerr << "Number of fields: " << table_rec.nfields() << endl;
 	cerr << "Datatype: " << col_desc.dataType() << endl;
    #endif
	return NULL;
#else
    // prevent unused variable build warning when casacore isn't installed
    colname = colname;
	cerr << "ERROR: casacore not installed." << endl;
	return NULL;
#endif
    }
   else
   {
	cerr << "dalTable::getColumnData operation not supported for type "
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
	cerr << "CASA table name: " << casa_table_handle->tableName() << endl;
#else
	cerr << "CASA support not enabled." << endl;
#endif
   }
   else
   {
	cerr << "dalTable::getName operation not supported for type "
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
	casa_table_handle = new casa::Table;
#else
	cerr << "CASA support not enabled." << endl;
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
          try
          {
	     *casa_table_handle = casa::Table( tablename );
	     *casa_table_handle = casa::tableCommand( filter->get(),
                                                      *casa_table_handle );
          }
          catch (casa::AipsError x)
          {
             cerr << "ERROR: " << x.getMesg() << endl;
          }
	}
	else
	{
          try
          {
             *casa_table_handle = casa::Table( tablename );
          }
          catch (casa::AipsError x)
          {
             cerr << "ERROR: " << x.getMesg() << endl;
          }
	}
#else
	cerr << "CASA support not enabled." << endl;
	exit(-1);
#endif
   }
   else
   {
	cerr << "dalTable::openTable operation not supported for type "
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
#ifdef WITH_CASA
      try
      {
         const casa::Vector< casa::String > tables = reader->tables();
         unsigned int nelem (tables.nelements());
         bool table_found = false;
         for (unsigned int table(0); table<nelem; table++)
         {
            string table_val = tables(table);
            if ( table_val == tablename )
            {
              *casa_table_handle = reader->table( tablename );
              table_found = true;
            }
         }
         if (!table_found)
         {
           cerr << "WARNING: Table \'" << tablename << "\' not found in file." << endl;
         }
      }
      catch (casa::AipsError x)
      {
         cerr << "ERROR: " << x.getMesg() << endl;
      }
#else
	cerr << "CASA support not enabled." << endl;
	exit(-1);
#endif
   }
   else
   {
	cerr << "dalTable::openTable operation not supported for type "
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
#ifdef WITH_CASA
      try
      {
        *casa_table_handle = reader->table( tablename );
        *casa_table_handle = casa::tableCommand( filter->get(),
                                                 *casa_table_handle );
      }
      catch (casa::AipsError x)
      {
         cerr << "ERROR: " << x.getMesg() << endl;
      }
#else
	cerr << "CASA support not enabled." << endl;
	exit(-1);
#endif
   }
   else
   {
	cerr << "dalTable::openTable operation not supported for type "
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

	table_id = H5Dopen1 ( file_id, name.c_str() );
   }
   else
   {
	cerr << "dalTable::openTable operation not supported for type "
	  << type << endl;
   }
}

/****************************************************************
 *  Default destructor
 *
 *****************************************************************/
dalTable::~dalTable()
{
  delete filter;
   if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
	delete casa_table_handle;
#endif
   }
}

/****************************************************************
 *  print the columns in the table
 *
 *****************************************************************/
void dalTable::printColumns()
{
   if ( type == H5TYPE )
   {
      size_t * field_sizes = NULL;
      size_t * field_offsets = NULL;
      size_t * size_out = NULL;
 	
	// retrieve the input fields needed for the append_records call
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	
	field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
	field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
	size_out = (size_t*)malloc( sizeof(size_t) );

	/* Alocate space */ 
	field_names = (char**)malloc( sizeof(char*) * (size_t)nfields );
	for ( unsigned int i = 0; i < nfields; i++) { 
	 	  field_names[i]=(char*)malloc(sizeof(char) * MAX_COL_NAME_SIZE );
 	 }
	status = H5TBget_field_info( file_id, name.c_str(), field_names,
				     field_sizes, field_offsets, size_out );

	for (unsigned int ii=0; ii<nfields; ii++) {
		cerr << setw(17) << field_names[ii];
		free(field_names[ii]);
	}
	free(field_names);
	cerr << endl;
   }
   else if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
     casa::TableDesc td = casa_table_handle->tableDesc();
     cerr << td.columnNames() << endl;
#else
	cerr << "CASA support not enabled." << endl;
	exit(-1);
#endif
   }
   else
   {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
	// It is necessary to have at least one column for table creation
	 // so we create a dummy column that will be deleted when real
	 // columns are added
	//
	
	name = groupname + '/' + tablename;// set the private class variable: name
	// cast the voidfile to an hdf5 file
	hid_t * lclfile = (hid_t*)voidfile; // H5File object
	file = lclfile;
	file_id = *lclfile;  // get the file handle

        const char * lclfield_names[1]  = { "000dummy000" };
	size_t dst_offset[1] = { 0  };
        hid_t field_type[1] = { H5T_NATIVE_INT };	
	hsize_t chunk_size = (hsize_t)CHUNK_SIZE;
	int compress  = 0;  // 0=off 1=on
        int * fill = NULL;
        fill = new int[1];
        fill[0] = 0;

	typedef struct Particle {
		int dummy;
	} Particle;
	Particle data[1] = {{0}};
	size_t dst_size =  sizeof(Particle);

	dst_size = sizeof( Particle );
	dst_offset[0] = HOFFSET( Particle, dummy );

	tablename = groupname + '/' + tablename;

	status = H5TBmake_table( tablename.c_str(), file_id, tablename.c_str(),
				 1, 1, dst_size, lclfield_names,
				 dst_offset, field_type, chunk_size,
				 fill, compress, data );
        delete [] fill;
        fill = NULL;
        table_id = H5Dopen1( file_id, tablename.c_str() );
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
		cerr << "WARNING: Trying to add column without a name.  Skipping."
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
	field_names = new char * [nfields];
	for (unsigned int ii=0; ii<nfields; ii++) {
	  field_names[ii] = new char[MAX_COL_NAME_SIZE];
	}
	status = H5TBget_field_info( file_id, name.c_str(), field_names, NULL,
				     NULL, NULL );

	// check to make sure column doesn't already exist
	bool removedummy = false;
	for (unsigned int ii=0; ii<nfields; ii++) {
		if (0 == strcmp(colname.c_str(),field_names[ii])) {
			cerr << "WARNING: Cannot create column \'"
			     << colname.c_str()
			     <<	"\'. Column already exists." << endl;
			return;
		}
		else if (0 == strcmp("000dummy000",field_names[ii])) {
			removedummy = true;
		}
	}

	for (unsigned int ii=0; ii<nfields; ii++) {
		delete [] field_names[ii];
		field_names[ii] = NULL;
	}
	delete [] field_names;
	field_names = NULL;

	// set the column type
	hid_t	field_type_new = 0;
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
			cerr << "ERROR: column type " << coltype <<
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
			cerr << "ERROR: column type " << coltype <<
			  " is not supported." << endl;
			exit(99);
		}
	}
	
	// set additional required fields for new column call
	hsize_t	position = nfields;

        int * data = NULL;
        data = new int[1];
        data[0] = 0;

	// create the new column
	status = H5TBinsert_field( file_id, name.c_str(), colname.c_str(),
				   field_type_new, position, NULL, data );

        delete [] data;
        data = NULL;

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
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::getAttributes() {
   if ( type == H5TYPE )
   {

   //status = H5Aget_num_attrs(group_id);
   //printf ("H5Aget_num_attrs returns: %i\n", status);
   //status = H5Aiterate( table_id, NULL, attr_info, NULL );
   //printf ("\nH5Aiterate returns: %i\n", status);

   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
		cerr << "WARNING: Trying to add column without a name.  Skipping."
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
			cerr << "WARNING: Cannot create column \'"
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
	free(field_names);

	// set the column type
	hsize_t dd = indims;
	hsize_t * dims = &dd;
	hid_t	field_type = 0;
	hid_t   h5type = 0;
	
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
			//cerr << "subcolumn name is " << foo[ii].getName() 
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
			//cerr << "subcolumn name is " << foo[ii].getName() 
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
		cerr << "ERROR: column type " << coltype << " is not supported." 
		     << endl;
		exit(99);
	}
	field_type = H5Tarray_create1( h5type, 1, dims, NULL );

	// set additional required fields for new column call
	hsize_t	position = nfields;
        int * data = NULL;
        data = new int[ indims ];
        for (unsigned int idx=0; idx<indims; idx++)
          data[idx] = 0;

	// create the new column
	status = H5TBinsert_field( file_id, name.c_str(), colname.c_str(),
				   field_type, position, NULL, data );

        delete [] data;
        data = NULL;

	if ( removedummy )
		removeColumn("000dummy000");
	
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
			cerr << "WARNING: Cannot create column \'"
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
	free(field_names);
	size_t sz = 0;
	
	// compute the size of the compound column
	for ( unsigned int ii=0; ii<foo.size(); ii++) {
// 		cerr << "subcolumn name is " << foo[ii].getName() 
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
	hsize_t position = nfields;

	int * data = new int[subfields];
	for (int idx=0; idx<subfields; idx++)
          data[idx] = 0;

	// create the new column
	status = H5TBinsert_field( file_id, name.c_str(), compname.c_str(),
					fieldtype, position,
					data, data );

        delete [] data;
        data = NULL;

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
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }

}

/****************************************************************
 *  
 *
 *****************************************************************/
void dalTable::removeColumn(const string &colname)
{	
   if ( type == H5TYPE )
   {
	status = H5TBget_table_info( file_id, name.c_str(), &nfields, &nrecords );
	if ( nfields < 2 ) {
			cerr << "WARNING: Cannot delete last column." << endl;
			return;
	}
	

        hsize_t nfields_start = nfields;

        field_names = (char **)malloc(nfields_start * sizeof(char *));
	for (unsigned int ii=0; ii<nfields_start; ii++) {
	  field_names[ii] = (char *)malloc(MAX_COL_NAME_SIZE * sizeof(char));
	}

	status = H5TBget_field_info( file_id, name.c_str(), field_names, NULL,
				     NULL, NULL );

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

	for (unsigned int ii=0; ii<nfields_start; ii++) {
	  free(field_names[ii]);
	}
	free(field_names);

	if ( !columnpresent )
		cerr << "WARNING: Column \'" << colname <<
				"\' not present.  Cannot delete." << endl;
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
	
	size_t * field_sizes = NULL;
	size_t * field_offsets = NULL;
	size_t * size_out = NULL;
	
	// retrieve the input fields needed for the append_records call
	H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );
	
        field_sizes   = new size_t[ nfields ];
        field_offsets = new size_t[ nfields ];
        size_out      = new size_t[ 1 ];
// 	field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
// 	field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
// 	size_out = (size_t*)malloc( sizeof(size_t) );

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

        delete [] field_sizes;
        field_sizes = NULL;
        delete [] field_offsets;
        field_offsets = NULL;
        delete [] size_out;
        size_out = NULL;

   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
	size_t * field_sizes = NULL;
	size_t * field_offsets = NULL;
	size_t * size_out = NULL;
	
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
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
	size_t * field_sizes = NULL;
	size_t * field_offsets = NULL;
	size_t * size_out = NULL;

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
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

void dalTable::setAttribute_string( string attrname, string data ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_string( file_id, name.c_str(),
					attrname.c_str(), data.c_str() ) < 0 ) {
		cerr << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

void dalTable::setAttribute_char( string attrname, char * data, int size ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_char( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cerr << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

void dalTable::setAttribute_int( string attrname, int * data, int size ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_int( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cerr << "ERROR: could not set attribute " << attrname << endl;
	}
 
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

void dalTable::setAttribute_uint(string attrname, unsigned int * data, int size)
{
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_uint( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cerr << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

void dalTable::setAttribute_double( string attrname, double * data, int size ) {
   if ( type == H5TYPE )
   {
	if ( H5LTset_attribute_double( file_id, name.c_str(),
					attrname.c_str(), data, size ) < 0 ) {
		cerr << "ERROR: could not set attribute " << attrname << endl;
	}
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

std::vector<std::string> dalTable::listColumns()
{
   std::vector<std::string> colnames;
   colnames.clear();

   if ( type == H5TYPE )
   {
      size_t * field_sizes = NULL;
      size_t * field_offsets = NULL;
      size_t * size_out = NULL;

      // retrieve the input fields needed for the append_records call
      H5TBget_table_info ( file_id, name.c_str(), &nfields, &nrecords );

      field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
      field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
      size_out = (size_t*)malloc( sizeof(size_t) );

      /* Alocate space */ 
      field_names = (char**)malloc( sizeof(char*) * (size_t)nfields );
      for ( unsigned int i = 0; i < nfields; i++)
      {
        field_names[i]=(char*)malloc(sizeof(char) * MAX_COL_NAME_SIZE );
      }
      status = H5TBget_field_info( file_id, name.c_str(), field_names,
                  field_sizes, field_offsets, size_out );

      for (unsigned int ii=0; ii<nfields; ii++)
      {
        colnames.push_back( field_names[ii] );
        free(field_names[ii]);
      }
      free(field_names);
      return colnames;
   }
   else if ( type == MSCASATYPE )
   {
#ifdef WITH_CASA
     casa::TableDesc td = casa_table_handle->tableDesc();
     casa::Vector<casa::String> names = td.columnNames();
     casa::Array<casa::String> myarray = names;
     std::vector<casa::String> foovec;
     myarray.tovector( foovec );
     for ( uint idx=0; idx < foovec.size(); idx++ )
        colnames.push_back( std::string( foovec[ idx ].c_str() ) );
     return colnames;
#else
     cerr << "CASA support not enabled." << endl;
     exit(-1);
#endif
   }
   else
   {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
     return colnames;
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
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
	 return(-1);
   }
}

void dalTable::readRows( void * data_out, long nstart, long numberRecs, long buffersize )
{
   if ( type == H5TYPE )
   {
        size_t * field_sizes = NULL;
        size_t * field_offsets = NULL;
        size_t * size_out = NULL;

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
	free(field_names);
        if (status < 0) {
                cerr << "Problem reading records. Row buffer may be too big. "
                     << "Make sure the buffer is smaller than the size of the "
                     << "table." << endl;
                exit(897);
        }
   }
   else
   {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
   }
}

/****************************************************************
 *  
 *
 *****************************************************************/
casa::Bool dalTable::findAttribute( string attrname )
{
   if ( type == H5TYPE )
   {
	if ( H5Aexists( table_id, attrname.c_str() ) <= 0 ) {
		cerr << "Attribute " << attrname << " not found." << endl;
		return false;
	} else {
		return true;
	}
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
	if ( H5Aexists( table_id, attrname.c_str() ) <= 0 ) {
		cerr << "Attribute " << attrname << " not found." << endl;
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
		cerr << attrname << " = ";
		for (unsigned int ii=0; ii<*dims; ii++) {
		  cerr << data[ii];
		  if (ii < (*dims)-1)
		    cerr << ',';
		  else
		    cerr << endl;
		}
	}
	else if ( H5T_INTEGER == type_class ) {
		int data[*dims];
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), 
		  attrname.c_str(), H5T_NATIVE_INT, data) )
		{
			cerr << attrname << " not found" << endl;
		}
		else
		{
			cerr << attrname << " = ";
			for (unsigned int ii=0; ii<*dims; ii++) {
			  cerr << data[ii];
			  if (ii < (*dims)-1)
				cerr << ',';
			  else
				cerr << endl;
			}
		}
	}
	else if ( H5T_STRING == type_class ) {
		char* data;
		string fullname = "/" + name;
		data = (char *)malloc(rank * sizeof(char));
		H5LTget_attribute_string( file_id, fullname.c_str(),
					  attrname.c_str(),data);
		cerr << attrname << " = " << data << endl;
	}
	else {
		cerr << "Attribute " << attrname << " type unknown." << endl;
	}
   } else {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
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
	if ( H5Aexists( table_id, attrname.c_str() ) <= 0 ) {
		return NULL;
	}
	
	string fullname = "/" + name;

	int rank;
	H5LTget_attribute_ndims( file_id, fullname.c_str(),
	  attrname.c_str(), &rank );

	H5LTget_attribute_info( file_id, fullname.c_str(), attrname.c_str(),
				&dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		void * data = NULL;
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), 
		  attrname.c_str(),
			 H5T_NATIVE_DOUBLE, data) )
		  return NULL;
		else
		  return reinterpret_cast<double*>(data);
	}
	else if ( H5T_INTEGER == type_class ) {
		void * data = NULL;
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(),
		  attrname.c_str(),
			H5T_NATIVE_INT, data) )
		  return NULL;
		else
		  return reinterpret_cast<int*>(data);
	}
	else if ( H5T_STRING == type_class ) {
		char* data = NULL;
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
   }
   else if ( type == MSCASATYPE )
   {
      #ifdef WITH_CASA
      casa::TableRecord table_rec = casa_table_handle->keywordSet();
      casa::String dtype = GetKeywordType(attrname);
      if (dtype == "Float")
      {
        casa::Float result;
        if (GetKeyword(attrname,&result))
           return (casa::Float*)&result;
        else
        {
          cerr << "ERROR: dalTable:GetKeyword Could not get '" << attrname << endl; 
          return NULL;
        }
      }
      else
      {
         cerr << "(TBD) datatype is " << type << endl;
         return NULL;
      }
      #else
      cerr << "ERROR: CASA not enabled [dalTable::getAttribute()]" << endl;
      return NULL;
      #endif
   }
   else
   {
     cerr << "Operation not yet supported for type " << type << ".  Sorry.\n";
     return NULL;
   }

   return NULL;
}

#ifdef WITH_CASA

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName, casa::String *result)
  {
    #ifdef DEBUGGING_MESSAGES      
    cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
    #endif
    try {  
      if (!casa_table_handle->keywordSet().isDefined(KeywordName)) {
        cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName << 
          "\" does not exist" << endl;
        return casa::False;
      };
      *result = casa_table_handle->keywordSet().asString(KeywordName);
    } catch (casa::AipsError x) {
      cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
      return casa::False;
    }; 
    return casa::True;
  }

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName, casa::Double *result)
  {
    #ifdef DEBUGGING_MESSAGES      
    cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
    #endif
    try {  
      if (!casa_table_handle->keywordSet().isDefined(KeywordName)) {
        cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName << 
          "\" does not exist" << endl;
        return casa::False;
      };
      *result = casa_table_handle->keywordSet().asDouble(KeywordName);
    } catch (casa::AipsError x) {
      cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
      return casa::False;
    }; 
    return casa::True;
  }

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName, casa::Float *result)
  {
    #ifdef DEBUGGING_MESSAGES      
    cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
    #endif
    try {  
      if (!casa_table_handle->keywordSet().isDefined(KeywordName)) {
        cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName << 
          "\" does not exist" << endl;
        return casa::False;
      };
      *result = casa_table_handle->keywordSet().asFloat(KeywordName);
    } catch (casa::AipsError x) {
      cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
      return casa::False;
    }; 
    return casa::True;
  }

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName, casa::DComplex *result)
  {
    #ifdef DEBUGGING_MESSAGES      
    cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
    #endif
    try {  
      if (!casa_table_handle->keywordSet().isDefined(KeywordName)) {
        cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName << 
          "\" does not exist" << endl;
        return casa::False;
      };
      *result = casa_table_handle->keywordSet().asDComplex(KeywordName);
    } catch (casa::AipsError x) {
      cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
      return casa::False;
    }; 
    return casa::True;
  }

  casa::Bool dalTable::GetKeyword(casa::String const KeywordName, casa::Array<casa::Double> *result)
  {
    #ifdef DEBUGGING_MESSAGES
    cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
    #endif
    try {  
      if (!casa_table_handle->keywordSet().isDefined(KeywordName)) {
        cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName << 
          "\" does not exist" << endl;
        return casa::False;
      };
      result->reference(casa_table_handle->keywordSet().asArrayDouble(KeywordName));
    } catch (casa::AipsError x) {
      cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
      return casa::False;
    }; 
    return casa::True;
  }
  casa::Bool dalTable::GetKeyword(casa::String const KeywordName, casa::Array<casa::DComplex> *result)
  {
    #ifdef DEBUGGING_MESSAGES
    cerr << "dalTable::GetKeyword called for " << KeywordName << endl;
    #endif
    try {  
      if (!casa_table_handle->keywordSet().isDefined(KeywordName)) {
        cerr << "dalTable::GetKeyword: Keyword named \"" << KeywordName << 
          "\" does not exist" << endl;
        return casa::False;
      };
      result->reference(casa_table_handle->keywordSet().asArrayDComplex(KeywordName));
    } catch (casa::AipsError x) {
      cerr << "dalTable::GetKeyword: " << x.getMesg() << endl;
      return casa::False;
    }; 
    return casa::True;
  }


  // ==============================================================================
  // Get the type of the given keyword
  // ==============================================================================
  casa::String dalTable::GetKeywordType(casa::String const KeywordName)
  {
    try {  
      if (!casa_table_handle->keywordSet().isDefined(KeywordName)) {
        cerr << "dalTable::GetKeywordType: Keyword named \"" << KeywordName << 
          "\" does not exist" << endl;
        return "";
      };
      casa::DataType type = casa_table_handle->keywordSet().dataType(KeywordName);
      switch(type) {
      case casa::TpString:
        return "String";
      case casa::TpFloat:
        return "Float";
      case casa::TpDouble:
        return "Double";
      case casa::TpDComplex:
        return "DComplex";
      case casa::TpArrayDouble:
        return "Array<Double>";
      case casa::TpArrayDComplex:
        return "Array<DComplex>";
      case casa::TpInt:
        return "Int";
      case casa::TpUInt:
        return "uInt";
      default:
        return "unknown";
      }
    } catch (casa::AipsError x) {
      cerr << "dalTable::GetKeywordType: " << x.getMesg() << endl;
      return "unknown";
    }; 
  }
#endif // WITH_CASA

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
 *  wrapper for listColumns
 *
 *****************************************************************/
bpl::list dalTable::listColumns_boost()
{
  std::vector<std::string> cols = listColumns();
  std::vector<int> mydims;
  mydims.push_back( cols.size() );
  bpl::list mylist;
  for ( uint idx=0; idx < cols.size(); idx++ )
     mylist.append( cols[ idx ].c_str() );
  return mylist;
}


#ifdef WITH_CASA

/****************************************************************
 *  wrapper for openTable
 *    This opens a casa table, but not in a MeasurementSet
 *
 *****************************************************************/
void dalTable::ot_nonMStable( string tablename )
{
   openTable( tablename );
}

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

/*
bpl::numeric::array dalTable::getColumnData_boost( string colname )
{
     getColumnData( colname );
     for (unsigned int hh=0; hh<shape.size(); hh++)
     { mydims.push_back(shape[hh]); }
     bpl::numeric::array narray = num_util::makeNum((double*)data,mydims);
     vector<int> fshape = num_util::shape(narray);
     return narray;
}
*/
#endif // WITH_CASA

bpl::numeric::array dalTable::getAttribute_boost(std::string attrname)
{
   vector<int> mydims;
   mydims.push_back(1);
   float * dta = (float*)getAttribute(attrname);
   return num_util::makeNum(((float*)dta),mydims);
}

#endif // PYTHON

} // end namespace DAL
