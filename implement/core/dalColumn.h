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

#ifndef DALDATA_H
#include "dalData.h"
#endif

namespace DAL {
  
  /*!
    \class dalColumn
    \ingroup DAL
    \ingroup core
    \brief Represents a table column.
    \author Joseph Masters
    \date 12-11-2006
  */
  class dalColumn
  {
    std::string filetype;      // "HDF5", "MSCASA" or "FITS"; for example
    std::string name;          // Column name
    std::string tablename;     // Table name
    std::string dal_datatype;  // Column datatype
    int size;                  // Datatype size
    uint num_of_rows;          // Number of rows in the column

    //! HDF5 file object identifier
    hid_t fileID_p;
    //! HDF5 table object identifier
    hid_t tableID_p;
    //! HDF5 field count
    hsize_t nofFields_p;
    //! HDF5 record count
    hsize_t nofRecords_p;
    //! Identifier for the column type
    hid_t coltype;
    //! HDF5 call return status
    herr_t  status;
    
    dalData * data_object;  // object to hold column data
    
#ifdef HAVE_CASA
    
    std::string casa_datatype;  // column datatype
    
    casa::ColumnDesc casa_col_desc;
    casa::ROTableColumn * casa_column;
    
    // COLUMNs
    casa::ROArrayColumn<int> * roac_int;
    casa::ROArrayColumn<double> * roac_dbl;
    casa::ROArrayColumn< std::complex< float > > * roac_comp;
    casa::ROArrayColumn<casa::String> * roac_string;
    
    casa::ROScalarColumn<int> * rosc_int;
    casa::ROScalarColumn<bool> * rosc_bool;
    casa::ROScalarColumn<double> * rosc_dbl;
    casa::ROScalarColumn< std::complex< float > > * rosc_comp;
    casa::ROScalarColumn<casa::String> * rosc_string;
    
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
    casa::Vector<bool> scalar_vals_bool;
    vector< std::complex< float > > stl_vec_comp;
    
    bool deleteIt;
    
    std::string getCasaDataType();
    dalData * CasaData_scalar( );
    dalData * CasaData_array( );
#endif
    
    dalData * H5data (int &start,
		      int &length);
    
  public:

    //! Default constructor
    dalColumn();
    //! Create a new complex column.
    dalColumn( std::string complexcolname );
    //! Argumented constructor for a new column object.
    dalColumn( hid_t fileid,
	       hid_t tableid,
	       std::string filetype,
	       std::string lcl_tablename,
	       std::string colname,
	       std::string coldatatype );
    dalColumn( std::string colname,
	       std::string coltype );
    
#ifdef HAVE_CASA
    dalColumn( casa::Table table,
	       std::string colname );
#endif
    
    void addMember( std::string member_name, std::string type );
    //! Get the name of the column.
    std::string getName();
    //! Set the name of the column.
    void setName(std::string colname);
    //! Set the file type of the dataset containing the column.
    void setFileType( std::string type );
    std::string getDataType();
    int getSize();
    //! Close the column.
    void close();
    std::string getType();
    bool isArray();
    bool isScalar();
    vector<int> shape();
    //! Get the number of dimensions of the column.
    int ndims();
    //! Get the number of rows in the column.
    uint nrows();
    dalData * data (int &start,
		    int &length);
    //! Get the data object for the column.
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
