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

#ifndef DALTABLE_H
#define DALTABLE_H

#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

#ifndef DALCOLUMN_H
#include "dalColumn.h"
#endif

namespace DAL {
  
  /*!
    \class dalTable
    
    \ingroup DAL
    
    \brief Object representing a table within a dataset.
    
    \test tdalTable.cpp
    
    A dalTable can reside within a dataset, or within a group that is within
    a dataset.
  */
  
  class dalTable {
    
    void * file;  // can be HDF5File, FITS, MS
    
    //! HDF5 file_id
    hid_t fileID_p;
    //! HDF5 table id
    hid_t tableID_p;
    //! HDF5 field count
    hsize_t nfields;
    //! HDF5 record count
    hsize_t nrecords;
    //! HDF5 return status
    herr_t status;
    //! HDF5 list of columns
    char **field_names;
    //! Table access filter
    dalFilter * filter;
    
    bool firstrecord;
    string name;  // table name
    string type;  // "HDF5", "MSCASA" or "FITS"; for example
    vector<dalColumn> columns; // list of table columns
    
#ifdef HAVE_CASA
    casa::Table * casa_table_handle;
    casa::Array<casa::Double> array_vals_dbl;
    casa::Array<casa::Complex> array_vals_comp;
    casa::ROTableColumn * casa_column;
#endif
    
    void h5addColumn_setup (std::string const column_name,
			    bool &removedummy);
    void h5addColumn_insert (uint const & indims,
			     std::string const & colname,
			     hid_t const & field_type,
			     bool const & removedummy );
    
  public:
    
    // ------------------------------------------------ Construction
    
    //! Default table constructor
    dalTable();
    //! Table constructor for a specific file format.
    dalTable( string filetype );
    
    // ------------------------------------------------- Destruction
    
    //! Destructor
    ~dalTable();
    
    // -------------------------------------------- Parameter access
    
    //! Get the HDF5 file identifier
    inline hid_t fileID () const {
      return fileID_p;
    }
    //! Get the HDF5 table object identifier
    inline hid_t tableID () const {
      return tableID_p;
    }
    //! Get the number of fields within the table
    inline hsize_t nofFields () const {
      return nfields;
    }
    //! Get the numbe of records within the table
    inline hsize_t nofRecords () const {
      return nrecords;
    }
    
    void printColumns();
    inline void summary() {
      summary(std::cout);
    }
    void summary(std::ostream &os);
    void openTable( void * voidfile,
		    string tablename,
		    string groupname );
    
#ifdef HAVE_CASA
    void openTable( string tablename );
    //! Open the table in a measurement set.
    void openTable( string tablename,
		    casa::MSReader * reader );
    //! Open a filtered CASA measurement set table.
    void openTable( string tablename,
		    casa::MSReader * reader,
		    dalFilter * filter );
    //! Get keyword of type casa::String
    bool GetKeyword( casa::String const KeywordName,
		     casa::String *result);
    //! Get keyword of type casa::Double
    bool GetKeyword( casa::String const KeywordName,
		     casa::Double *result);
    //! Get keyword of type casa::Float
    bool GetKeyword( casa::String const KeywordName,
		     casa::Float *result);
    bool GetKeyword( casa::String const KeywordName,
		     casa::DComplex *result);
    bool GetKeyword( casa::String const KeywordName,
		     casa::Array<casa::Double> *result);
    bool GetKeyword( casa::String const KeywordName,
		     casa::Array<casa::DComplex> *result);
    casa::String GetKeywordType(casa::String const KeywordName);
#endif
    
    //! Create a new table
    void createTable (void * voidfile,
		      string tablename,
		      string groupname);
    //! Get a column object
    dalColumn * getColumn_complexInt16( string colname );
    //! Get a column object
    dalColumn * getColumn_complexFloat32( string colname );
    //! Get a column object
    dalColumn * getColumn_Float32( string colname );
    //! Get a column object
    dalColumn * getColumn( string colname );
    //! Add a new column to the table
    void addColumn( string colname, string coltype, uint dims=1);
    //! Add a new column of type complex to the table
    void addComplexColumn( string compname,
			   vector<dalColumn> ri,
			   int subfields );
    //! Remove a column from the table
    void removeColumn( const string &colname );
    void writeDataByColNum( void * structure, int index, int rownum, long nrecords=1 );
    void setFilter( string columns );
    void setFilter( string columns, string conditions );
    void appendRow( void * data );
    void appendRows( void * data, long number_of_rows );
    std::vector<std::string> listColumns();
    void readRows( void * data_out, long start, long stop, long buffersize=0 );
    void * getAttribute( string attrname );
    
    // ---------------------------------------------------------- getAttribute
    
    /*!
      \brief Get the value of an attribute.
      
      Get the value of an attribute.  This is different from printAttribute
      because the value of the attribute is returned into a structure
      instead of simply printing.
      
      \param attrname The name of the attribute you want to retrieve.
    */
    template<class T>
      bool getAttribute( std::string attrname, T &value )
      {
        return h5get_attribute( tableID_p, attrname, value );
      }
    
    bool setAttribute( std::string attrname, char const * data, int size=1 );
    bool setAttribute( std::string attrname, short const * data, int size=1 );
    bool setAttribute( std::string attrname, int const * data, int size=1 );
    bool setAttribute( std::string attrname, uint const * data, int size=1 );
    bool setAttribute( std::string attrname, long const * data, int size=1 );
    bool setAttribute( std::string attrname, float const * data, int size=1 );
    bool setAttribute( std::string attrname, double const * data, int size=1 );
    bool setAttribute( std::string attrname, std::string data );
    bool setAttribute( std::string attrname, std::string const * data, int size=1 );
    //! Find an attribute associated with the table
    bool findAttribute( string attrname );
    //! Get the number of rows within the table
    long getNumberOfRows();
    //! Print the name of the table
    void getName();
    void * getColumnData( string colname );
    
    /************************************************************************
     *
     * The following functions are boost wrappers to allow some previously
     *   defined functions to be easily called from a python prompt.
     *
     ************************************************************************/
#ifdef PYTHON
    
    void ot_hdf5( void * voidfile, string tablename, string groupname );
    bool append_row_boost( bpl::object data );
    bool append_rows_boost( bpl::object data, long nrows );
    void write_col_by_index_boost( bpl::numeric::array data, int index,
				   int rownum, long nrecords );
    bpl::list listColumns_boost();
    bpl::numeric::array getAttribute_boost(std::string);
    PyObject* readRows_boost( int start, int nrecs );
    
    bool setAttribute_char( std::string attrname, char data );
    bool setAttribute_short( std::string attrname, short data );
    bool setAttribute_int( std::string attrname, int data );
    bool setAttribute_uint( std::string attrname, uint data );
    bool setAttribute_long( std::string attrname, long data );
    bool setAttribute_float( std::string attrname, float data );
    bool setAttribute_double( std::string attrname, double data );
    bool setAttribute_string( std::string attrname, std::string data );
    bool setAttribute_char_vector (std::string attrname, bpl::list data);
    bool setAttribute_short_vector (std::string attrname, bpl::list data);
    bool setAttribute_int_vector (std::string attrname, bpl::list data);
    bool setAttribute_uint_vector (std::string attrname, bpl::list data);
    bool setAttribute_long_vector (std::string attrname, bpl::list data);
    bool setAttribute_float_vector (std::string attrname, bpl::list data);
    bool setAttribute_double_vector (std::string attrname, bpl::list data);
    bool setAttribute_string_vector (std::string attrname, bpl::list data);
    
#ifdef HAVE_CASA
    void ot_nonMStable( string tablename );
    void setFilter_boost1(string);
    void setFilter_boost2(string,string);
    // 	bpl::numeric::array getColumnData_boost( string colname );
#endif
    
#endif
  };
  
} // end namespace DAL
#endif
