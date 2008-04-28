/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

/*!
  \ingroup DAL

  \brief Object representing a table within a dataset.

  \author Joseph Masters

  \date 12-04-06
*/

#ifndef DALTABLE_H
#define DALTABLE_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

#ifndef DALCOLUMN_H
#include "dalColumn.h"
#endif

namespace DAL {

/*!
A dalTable can reside within a dataset, or within a group that is within
a dataset.
*/
class dalTable{

	void * file;		//!< can be HDF5File, FITS, MS

	// HDF5-specific variables
	hid_t file_id; 	//!< hdf5 file_id
	hid_t table_id;	//!< hdf5 table id
	hsize_t nfields; //!< hdf5 field count
	hsize_t nrecords; //!< hdf5 record count
	herr_t status; //!< hdf5 return status
	char **field_names;  //!< hdf5 list of columns
	
	dalFilter * filter; //!< table filter

	bool firstrecord;
	string name; //!< table name
	string type;  //!< "HDF5", "MSCASA" or "FITS"; for example
	vector<dalAttribute> attributes; //!< list of table attributes
	vector<dalColumn> columns; //!< list of table columns

	long nrows; //!< number of table rows
	int ncols; //!< number of table columns

#ifdef WITH_CASA

	casa::Table * casa_table_handle;
	casa::Array<casa::Double> array_vals_dbl;
	casa::Array<casa::Complex> array_vals_comp;

#endif

  public:

    /*!
	  \brief Default table constructor.
	  
	  The default table constructor.
	 */
	dalTable();

    /*!
	  \brief Table constructor for a specific file format.

	  Table constructor for a specific file format.

	  \param filetype The type of table you want to create (i.e. 
			"HDF5", "MSCASA", etc.)
	 */
	dalTable( string filetype );

    /*!
	  \brief Default table destructor. 
	  
	  Default table destructor. 
	 */
	~dalTable();

    /*!
	  \brief Print a list of table attributes.
	  
	  Print a list of table attributes.
	 */
 	void getAttributes();

    /*!
	  \brief Print a list of the columns contained in the table.
	  
	  Print a list of the columns contained in the table.
	 */
	void printColumns();
	
    /*!
	  \brief Open the table.  Called from dalDataset, not from the user.
	  
	  Open the table.  Called from dalDataset, not from the user.

	  \param voidfile A pointer to the file you want to open.
	  \param tablename The name of the table you want to open.
	  \param groupname The name of the group containing the table you want
	                   to open.
	 */
	void openTable( void * voidfile, string tablename, string groupname );

#ifdef WITH_CASA
    /*!
	  \brief Open a CASA table.
	  
	  Open a CASA table.

	  \param tablename The name of the table you want to open.
	 */
	void openTable( string tablename );

    /*!
	  \brief Open the table in a measurement set.
	  
	  Open the table in a measurement set.
	  
	  \param tablename The name of the table you want to open.
	  \param reader The CASA MS table reader object.
	 */
	void openTable( string tablename,
	  casa::MSReader * reader );

    /*!
	  \brief Open a filtered CASA measurement set table.
	  
	  Open a filtered CASA measurement set table.
	 */
	void openTable( string tablename,
	                casa::MSReader * reader,
	                dalFilter * filter );
#endif

    /*!
	  \brief Create a new table.
	  
	  Create a new table in the dataset, possibly within a group.  This
	  function is usually called by dalDataset, and not by the developer.
	  
	  \param voidfile A pointer to the file.
	  \param tablename The name of the table you want to create.
	  \param groupname The name of the group you where you want to create
					   the table.
	 */
	void createTable( void * voidfile, string tablename, string groupname );

    /*!
	  \brief Get a column object from a table.
	  
	   Gets a column object from a table object.
	   
	   \param colname The name of the column you want to get from the table.
	   \return dalColumn Pointer to a column object.
	 */
	dalColumn * getColumn_complexInt16( string colname );
	dalColumn * getColumn_complexFloat32( string colname );
	dalColumn * getColumn_Float32( string colname );
	dalColumn * getColumn( string colname );

    /*!
	  \brief Add a column to the table.
	  
	  Add a column to the table.

	  \param colname Name of the column you want to add.
	  \param coltype Datatype of the column you want to add (i.e. dalINT,
	                 dalFLOAT, dalSTRING, etc.)
	  \param size Optional parameter that specifies the array size of the
	              column.  The default is 1, meaning the column will be scalar.
	 */
	void addColumn( string colname, string coltype, int size=1 );

    /*!
	  \brief Create an array column.
	  
	  Create an array column.  This is usually called by the addColumn method
	  and not by the developer.
	  
	  \param colname Name of the column you want to add.
	  \param coltype Datatype of the column you want to add (i.e. dalINT,
	                 dalFLOAT, dalSTRING, etc.)
	  \param dims Number of dimensions of the column you want to add.
	 */
	void addArrayColumn( string colname, string coltype, unsigned int dims);

    /*!
	  \brief Create a complex type column.
	  
	  Create an complex type column to the table.  This is usually called by the
	  addColumn method when the addColumn datatype is dalCOMPLEX, and not by the
	  developer.
	  
	  \param compname Name of the column you want to add.
	  \param ri The real and imaginary column components, as represented by
	            dalColumn objects.
	  \param subfields This is the dimensionality of the complex column type
	                   and should always be 2.
	 */
    void addComplexColumn( string compname,
	                       vector<dalColumn> ri,
				           int subfields );
				
    /*!
	  \brief Remove a column.
	  
	  Remove a column from the table.
	  
	  \param colname Name of the column you want to remove from the table.
	 */
	void removeColumn( const string &colname );

    /*!
	  \brief Write data to a column identified by it's position.
	  
	  Write data to a column identified by it's position within the table.
	  
	  \param structure A pointer to a structure containing the data you want
	                   to write.
	  \param index The position of the column you want to write to.
	  \param rownum The row position where you want to start writing data.
	 */
	void writeDataByColNum( void * structure, int index, int rownum );
	
    /*!
	  \brief Set a filter on the table.
	  
	  Sets a filter on the table so that when it is opened, it will only
	  contain a subset of information contained within the full dataset table.
	  
	  \param columns A comma-separated list of columns you wish read from the
	                 table.
	 */
	void setFilter( string columns );

    /*!
	  \brief Set a filter on the table.
	  
	  Sets a filter on the table so that when it is opened, it will only
	  contain a subset of information contained within the full dataset table.
	  
	  \param columns A comma-separated list of columns you wish read from the
	                 table.
	  \param conditions The condition you wish to apply to the columns in the
	                   filter.  For example: "TIME>100".
	 */
	void setFilter( string columns, string conditions );

    /*!
	  \brief Append a row to the table.
	  
	  Append a row to the table.
	  
	  \param data The data you want to write at the end of the table.  The
				  structure of the data parameter should match that of the
				  table itself.
	 */
	void appendRow( void * data );

    /*!
	  \brief Append multiple rows.
	
	  Append multiple rows to the end of the table.
	  
	  \param data The data you want to write at the end of the table.  The
				  structure of the data parameter should match that of the
				  table itself.
	  \param number_of_rows The number of rows you wish to append.
	 */
	void appendRows( void * data, long number_of_rows );

    /*!
	  \brief List the table columns.
	  
	  Print a list of columns contained within the opened table.
	 */
	void listColumns();

    /*!
	  \brief Read rows from the table.
	  
	  Read some or all rows from the table.
	  
	  \param data_out A structure which the data will be read in to.  It should
	                  match the structure of the table itself.
	  \param start The row number to start reading from.
	  \param stop The row number where you want to stop reading.
	  \param buffersize An optional parameter specifying the size of the output
	                    buffer.
	 */
	void readRows( void * data_out, long start, long stop, long buffersize=0 );

    /*!
	  \brief Get the value of an attribute.
	  
	  Get the value of a table attribute.
	  
	  \param attrname The name of the attribute you want to read.
	  \return A pointer to the attribute data.
	 */
	void * getAttribute( string attrname );

    /*!
	  \brief Print the value of an attribute.
	  
	  Print the value of an attribute.
	  \param attrname The name of the attribute you wish to print.
	 */
	void printAttribute( string attrname );

    /*!
	  \brief Add a string attribute.
	  
	  Add a string attribute to the table.
	  
	  \param attrname The name of the attribute you wish to add.
	  \param data The value of the attribute you wish to add.
	 */
	void setAttribute_string( string attrname, string data );

    /*!
	  \brief Add a character attribute.
	  
	  Add a character attribute to the table.	  

	  \param attrname The name of the attribute you wish to add.
	  \param data The value of the attribute you wish to add.
	  \param size An optional parameter specifiying the array size of the
	              attribute.  If not supplied, it will default to being
				  a scalar attribute.
	 */
	void setAttribute_char( string attrname, char * data, int size=1 );

    /*!
	  \brief Add a integer attribute.
	  
	  Add a integer attribute to the table.

	  \param attrname The name of the attribute you wish to add.
	  \param data The value of the attribute you wish to add.
	  \param size An optional parameter specifiying the array size of the
	              attribute.  If not supplied, it will default to being
				  a scalar attribute.
	 */
	void setAttribute_int( string attrname, int * data, int size=1 );

    /*!
	  \brief Add a unsigned integer attribute.
	  
	  Add a unsigned integer attribute to the table.

	  \param attrname The name of the attribute you wish to add.
	  \param data The value of the attribute you wish to add.
	  \param size An optional parameter specifiying the array size of the
	              attribute.  If not supplied, it will default to being
				  a scalar attribute.
	 */
	void setAttribute_uint( string attrname, unsigned int * data, int size=1 );

    /*!
	  \brief Add a double precision floating point attribute.
	  
	  Add a double precision floating point attribute to the table.

	  \param attrname The name of the attribute you wish to add.
	  \param data The value of the attribute you wish to add.
	  \param size An optional parameter specifiying the array size of the
	              attribute.  If not supplied, it will default to being
				  a scalar attribute.
	 */
	void setAttribute_double( string attrname, double * data, int size=1 );

    /*!
	  \brief Find an attribute.
	  
	  Look for the prescence of an attribute.
	  
	  \param attrname The name of the attribute you want to find.
	  \return True if found.  False if not found.
	 */
	bool findAttribute( string attrname );

    /*!
	  \brief Get the number of rows in a table.
	  
	  Get the number of rows in a table.
	  
	  \return The number of rows in the table as a long precision integer.
	 */
	long getNumberOfRows();
	
    /*!
	  \brief Print table name.
	  
	  Print the name of a table.
	 */
	void getName();
	
	/*!
	  \brief Retrieve a dalColumn by name.
	  
	  Retrieve a dalColumn by name.
	  \param colname The name of the column to retrieve.
	  \return A generic pointer to the column data.
	 */
	void * getColumnData( string colname );

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON

	void ot_hdf5( void * voidfile, string tablename, string groupname );
	void append_row_boost( bpl::object data );

#ifdef WITH_CASA
	void ot_nonMStable( string tablename );
	void setFilter_boost1(string);
	void setFilter_boost2(string,string);
#endif

#endif
};

} // end namespace DAL
#endif
