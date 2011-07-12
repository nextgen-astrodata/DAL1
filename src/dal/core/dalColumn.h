/***************************************************************************
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

#include <core/dalData.h>

#ifdef PYTHON
#include <pydal/num_util.h>
#endif

#ifdef DAL_WITH_CASA
#include <tables/Tables/ScalarColumn.h>
#include <tables/Tables/ArrayColumn.h>
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
    std::string itsDatatype;   // Column datatype
    int size;                  // Datatype size
    uint num_of_rows;          // Number of rows in the column

    //! HDF5 file object identifier
    hid_t itsFileID;
    //! HDF5 table object identifier
    hid_t itsTableID;
    //! HDF5 field count
    hsize_t nofFields_p;
    //! HDF5 record count
    hsize_t nofRecords_p;
    //! Identifier for the column type
    hid_t coltype;
    //! HDF5 call return status
    herr_t  status;
    
    dalData * itsColumnData;  // object to hold column data
    
#ifdef DAL_WITH_CASA
    
    //! Column data type
    std::string casa_datatype;
    //! Column descriptor
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
    
    // VECTORs
    casa::Vector<casa::Complex> scalar_vals_comp;
    vector< std::complex< float > > stl_vec_comp;
    
    bool deleteIt;
    
    std::string getCasaDataType();
    dalData * CasaData_scalar( );
    dalData * CasaData_array( );
#endif
    
    dalData * H5data (int &start,
		      int &length);
    
  public:

    // === Construction =========================================================

    //! Default constructor
    dalColumn();
    //! Create a new complex column.
    dalColumn (std::string complexcolname);
    //! Argumented constructor for a new column object.
    dalColumn (std::string const &colname,
	       std::string const &coltype);
    //! Argumented constructor for a new column object.
    dalColumn( hid_t fileid,
	       hid_t tableid,
	       std::string filetype,
	       std::string lcl_tablename,
	       std::string colname,
	       std::string coldatatype );
    
#ifdef DAL_WITH_CASA
    //! Create a new column object from a CASA table.
    dalColumn (casa::Table table,
	       std::string colname);
#endif
    
    // === Methods ==============================================================
    
    void addMember (std::string member_name,
		    std::string type);
    /*!
      \brief Get the name of the column.
      \return name -- The name of the column.
    */
    inline std::string getName() {
      return name;
    }
    //! Set the name of the column.
    void setName(std::string colname);
    //! Set the file type of the dataset containing the column.
    void setFileType( std::string type );
    std::string getDataType();
    int getSize();
    //! Close the column.
    void close();
    //! Retrieve the datatype of the column.
    inline std::string getType() {
      return itsDatatype;
    }
    //! Is the column an array?
    bool isArray();
    //! Is the column a scalar?
    bool isScalar();
    //! Get the number of dimensions of the column.
    int ndims();
    //! Get the shape of the column data
    std::vector<int> shape();
    //! Get the number of rows in the column.
    uint nofRows ();
    //! Get the data object for the column.
    dalData * data (int &start,
		    int &length);
    //! Get the data object for the column.
    dalData * data();
    
    //! Provide a summary of the object's internal parameters and status
    inline void summary() {
      summary(std::cout);
    }
    //! Provide a summary of the object's internal parameters and status
    void summary(std::ostream &os);

    // === Boost.Python wrappers ================================================

#ifdef PYTHON
    
    bpl::tuple shape_boost();
    bpl::numeric::array data_boost1( );
    bpl::numeric::array data_boost2( int32_t length );
    bpl::numeric::array data_boost3( int64_t offset, int32_t length );

#endif

    // === Private methods ======================================================

  private:

    //! Initialize the object's internal parameters
    void init (std::string const &columnName="");

  };  // dalColumn class
  
  
} // DAL namespace

#endif // DALCOLUMN_H
