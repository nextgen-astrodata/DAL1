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

#ifndef DALCOLUMN_H
#define DALCOLUMN_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

#ifndef DALDATA_H
#include "dalData.h"
#endif

#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

namespace DAL {

/*!
  \class dalColumn
  
  \ingroup DAL

  \brief Represents a table column.
  
  \author Joseph Masters
*/
class dalColumn {

	string filetype;  //!< "HDF5", "MSCASA" or "FITS"; for example
	string name; //!< column name
	string tablename; //!< table name
	string dal_datatype;  //!< column datatype
	int size; //!< datatype size
	int totsize; //!< total column size
	vector<dalAttribute> attributes; //!< list of column attributes
	unsigned int num_of_rows;  //!< number of rows in the column

	// HDF5-specific variables
	hid_t file_id; 	//!< hdf5 file_id
	hid_t table_id;	//!< hdf5 table id
	hsize_t nfields; //!< hdf5 field count
	hsize_t nrecords; //!< hdf5 record count
	hid_t coltype;
	herr_t  status;  //!< hdf5 call return status

	dalData * data_object;  //!< object to hold column data

#ifdef WITH_CASA

	string casa_datatype;  //!< column datatype

	casa::ColumnDesc casa_col_desc;
	casa::ROTableColumn * casa_column;

	// COLUMNs
	casa::ROArrayColumn<int> * roac_int;
	casa::ROArrayColumn<double> * roac_dbl;
	casa::ROArrayColumn< complex< float > > * roac_comp;
	casa::ROArrayColumn<casa::String> * roac_string;

	casa::ROScalarColumn<int> * rosc_int;
	casa::ROScalarColumn<double> * rosc_dbl;
	casa::ROScalarColumn< complex< float > > * rosc_comp;
	casa::ROScalarColumn<casa::String> * rosc_string;
	casa::ROScalarColumn<bool> * rosc_bool;

	// ARRAYs
	casa::Array<casa::Double> array_vals_dbl;
	casa::Array<casa::Complex> array_vals_comp;
	casa::Array<casa::Int> array_vals_int;
	casa::Array<casa::String> array_vals_string;

	// VECTORs
	casa::Vector<casa::Double> scalar_vals_dbl;
	casa::Vector<casa::Complex> scalar_vals_comp;
	casa::Vector<casa::Int> scalar_vals_int;
	casa::Vector<casa::String> scalar_vals_string;
	casa::Vector<casa::Bool> scalar_vals_bool;
	vector< complex< float > > stl_vec_comp;

	casa::Bool deleteIt;
#endif

 public:
	
	/*!
	  \brief Default constructor.
	  
	  Default constructor.
	*/
	dalColumn();
	
	/*!
	  \brief Create a new complex column.
	  
	  Create a new column with a complex floating point datatype.
	  
	  \param complexcolname Name of the column you want to create.
	*/
	dalColumn( string complexcolname );
	
	/*!
	  \brief Create a new column object.

	  \param fileid
	  \param tableid
	  \param filetype
	  \param lcl_tablename
	  \param colname
	  \param coldatatype
	*/
	dalColumn( hid_t fileid,
                   hid_t tableid,
                   string filetype,
                   string lcl_tablename,
                   string colname,
                   string coldatatype );
	
	/*!
	  \brief Create a new column object.
	  
	  Create a new column object.
	  
	  \param colname The name of the column you want to create.
	  \param coltype The datatype of the column you want to craete (i.e.
	                 dalINT, dalFLOAT, dalSTRING, etc.)
	 */
	dalColumn( string colname,
		   string coltype );

#ifdef WITH_CASA
	/*!
	  \brief Create a new column object.
	 */
	dalColumn( casa::Table table,
		   string colname );
#endif

	/*!
	 \brief Open a column
	
	 \param colname Name of the colum to be opened.
	 */
	void open( std::string colname );

	/*!
	 \brief Add a member to a compound column.
	 
	 Add a member to a compound column.  A compound column is made up of
	 several simple datatypes.  For example a compound column might be two
	 integers and a floating point.
	 
	 \param member_name The name of the member you want to add.
	 \param type The datatype of the column member you  want to add.
	 */
	void addMember( string member_name, string type );
	
	/*!
	  \brief Get the name of the column.
	  
	  Retrieve the name of the column.
	  
	  \return The name of the column.
	 */
	string getName();
	
	/*!
	  \brief Set the name of the column.
	  
	  Set the name of the column.
	  
	  \param colname The name of the column.
	 */
	void setName(string colname);
	
	/*!
	  \brief Set the file type of the dataset containing the column.
	  
	  Set the file type of the dataset containing the column.
	  
	  \param type The type of file.
	 */
	void setFileType( string type );

	/*!
	  \brief Get the column datatype.
	  
	  Get the datatype of the column.
	  
	  \return A string describing the datatype of the column.
	 */
	string getDataType();
	
	/*!
	  \brief Get the size of the column.
	  
	  Get the size of the column, in bytes.
	  
	  \return An integer describing the size, in bytes, of the column.
	 */
	int getSize();
	
	/*!
	  \brief Close the column.
	  
	  Close the column.
	  
	 */
	void close();
	
	/*!
	  \brief Get the data type.
	  
	  Retrieve the datatype of the column.
	  
	  \return A string describing the column datatype.
	 */
	string getType();
	
	/*!
	  \brief Is the column an array?
	  
	  Check to see if the column is an array.
	  
	  \return Non-zero if the column is an array.
	 */
	int isArray();
	
	/*!
	  \brief Is the column a scalar?
	  
	  Check to see if the column is scalar.
	  
	  \return Non-zero if the column is scalar.
	 */
	int isScalar();
	
	/*!
	  \brief Get the shape of the column.
	  
	  Get the shape of the column.
	  
	  \return A vector of integers specifying the shape of the column.
	 */
	vector<int> shape();
	
	/*!
	  \brief Get the number of dimensions of the column.
	  
	  Get the number of dimensions of the column.
	  
	  \return A integer specifying the number of column dimensions.
	 */
	unsigned int ndims();
	
	
	/*!
	  \brief Get the number of rows in the column.
	  
	  Get the number of rows in the column.
	  
	  \return A number specifying the number of column rows.
	 */
	unsigned int nrows();

	/*!
	  \brief Get column data.
	  
	  Get the data object for the column.
	  
	  \param start Start cell number for column.
	  \param length Number of cells to retrieve into the data object.
	  \return A dalData object containing the column data.
	 */
	dalData * data( int start, int &length );
	dalData * data();

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON

    bpl::tuple shape_boost();
    bpl::numeric::array data_boost1( );
    bpl::numeric::array data_boost2( int32_t length );
    bpl::numeric::array data_boost3( int64_t offset, int32_t length );

#endif
};  // dalColumn class


} // DAL namespace

#endif // DALCOLUMN_H
