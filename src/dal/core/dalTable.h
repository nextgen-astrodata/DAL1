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

#ifndef DALTABLE_H
#define DALTABLE_H

#include <iomanip>

#include <core/HDF5Attribute.h>
#include <core/dalFilter.h>
#include <core/dalColumn.h>

#ifdef DAL_WITH_CASA
#include <ms/MeasurementSets/MSReader.h>
#include <tables/Tables.h>
#endif

namespace DAL {
  
  /*!
    \class dalTable
    
    \ingroup DAL
    \ingroup core
    
    \brief Object representing a table within a dataset.
    
    \test tdalTable.cc
    
    \author Joseph Masters

    \date 2006-04-12

    A dalTable can reside within a dataset, or within a group that is within
    a dataset.
  */
  
  class dalTable : public dalObjectBase {
    
    //! File pointer (can be HDF5, FITS or CASA MS)
    void * file;
    //! HDF5 file_id
    hid_t itsFileID;
    //! HDF5 table id
    hid_t itsTableID;
    //! HDF5 field count
    hsize_t nfields;
    //! HDF5 record count
    hsize_t nofRecords_p;
    //! HDF5 return status
    herr_t status;
    //! Table access filter
    dalFilter itsFilter;
    //! HDF5 list of columns
    char **itsFieldNames;

    //! Access first record?
    bool itsFirstRecord;
    //! List of table columns
    std::vector<dalColumn> columns;
    
#ifdef DAL_WITH_CASA
    casa::Table * itsCasaTable;
    casa::Array<casa::Double> itsArrayDouble;
    casa::Array<casa::Complex> itsArrayComplex;
    casa::ROTableColumn * itsCasaColumn;
#endif
    
  public:
    
    // === Construction =========================================================

    //! Default table constructor
    dalTable ();
    //! Table constructor for a specific file format.
    dalTable (dalFileType const &filetype);
    //! Table constructor for a specific file format.
    dalTable (dalFileType::Type const &filetype);
    
    // === Destruction ==========================================================

    //! Destructor
    ~dalTable();
    
    // === Parameter access =====================================================
    
    //! Get the HDF5 file identifier
    inline hid_t fileID () const {
      return itsFileID;
    }
    //! Get the HDF5 table object identifier
    inline hid_t tableID () const {
      return itsTableID;
    }
    //! Get the number of fields within the table
    inline hsize_t nofFields () const {
      return nfields;
    }
    //! Get the numbe of records within the table
    inline hsize_t nofRecords () const {
      return nofRecords_p;
    }
    //! Get the number of columns in the table
    inline unsigned int nofColumns () const {
      return columns.size();
    }

    // === Public methods =======================================================

    //! Print a list of the columns contained in the table.
    void printColumns();
    //! Provide a summary of the object's internal parameters and status
    inline void summary() {
      summary(std::cout);
    }
    //! Provide a summary of the object's internal parameters and status
    void summary(std::ostream &os);
    //! Open the table.  Called from dalDataset, not from the user.
    void openTable (void * voidfile,
		    std::string const &tablename,
		    std::string const &groupname);
    
#ifdef DAL_WITH_CASA
    //! Open a CASA table, not in a MeasurementSet.
    bool openTable (std::string const &tablename);
    //! Open the table in a measurement set.
    void openTable (std::string const &tablename,
		    casa::MSReader * reader );
    //! Open a filtered CASA measurement set table.
    void openTable (std::string const &tablename,
		    casa::MSReader * reader,
		    dalFilter const &filter);
    //! Get keyword of type casa::String
    bool GetKeyword (casa::String const KeywordName,
		     casa::String *result);
    //! Get keyword of type casa::Double
    bool GetKeyword (casa::String const KeywordName,
		     casa::Double *result);
    //! Get keyword of type casa::Float
    bool GetKeyword (casa::String const KeywordName,
		     casa::Float *result);
    bool GetKeyword (casa::String const KeywordName,
		     casa::DComplex *result);
    bool GetKeyword (casa::String const KeywordName,
		     casa::Array<casa::Double> *result);
    bool GetKeyword (casa::String const KeywordName,
		     casa::Array<casa::DComplex> *result);
    casa::String GetKeywordType(casa::String const KeywordName);
#endif
    
    //! Create a new table
    void createTable (void * voidfile,
		      std::string const &tablename,
		      std::string groupname);
    //! Get a column object
    dalColumn * getColumn_complexInt16 (std::string colname);
    //! Get a column object
    dalColumn * getColumn_complexFloat32 (std::string colname);
    //! Get a column object
    dalColumn * getColumn_Float32 (std::string colname);
    //! Get a column object
    dalColumn * getColumn (std::string colname);
    //! Add a new column to the table
    void addColumn (std::string colname,
		    std::string coltype,
		    uint dims=1);
    //! Add a new column of type complex to the table
    void addComplexColumn( std::string compname,
			   std::vector<dalColumn> ri,
			   int subfields);
    //! Remove a column from the table
    void removeColumn (const std::string &colname);
    //! Write data by column number
    void writeDataByColNum (void * structure,
			    int index,
			    int rownum,
			    long nrecords=1);
    //! Set filter to select \e columns.
    void setFilter (std::string const &columns);
    //! Set filter to select \e columns with additional \e conditions applied.
    void setFilter (std::string const &columns,
		    std::string const &conditions);
    //! Append row of data to the table.
    void appendRow (void * data );
    //! Append rows of data to the table.
    void appendRows (void * data, long number_of_rows );
    //! List the column of the table
    std::vector<std::string> listColumns();
    //! Read rows from the table
    void readRows (void * data_out,
		   long start,
		   long stop,
		   long buffersize=0);
    //! Get attribute attached to the table
    void * getAttribute( std::string attrname );
    
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
        return HDF5Attribute::read (itsTableID, attrname, value );
      }
    
    /*!
      \brief Write attribute value
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \param size    -- nof. element in the data array.
      \return status -- Status of the operation
    */
    template <class T>
      bool setAttribute (std::string const &name,
			 T const *data,
			 unsigned int const &size=1)
      {
	return HDF5Attribute::write (itsFileID,
				     name,
				     data,
				     size);
      }

    bool setAttribute( std::string attrname, std::string data );
    //! Find an attribute associated with the table
    bool findAttribute( std::string attrname );
    //! Get the number of rows within the table
    long getNumberOfRows();
    //! Get the name of the table
    std::string tableName ();
    //! Retrieve a dalColumn by name.
    void * getColumnData ( std::string colname );
    
    /************************************************************************
     *
     * The following functions are boost wrappers to allow some previously
     *   defined functions to be easily called from a python prompt.
     *
     ************************************************************************/

#ifdef PYTHON
    
    bool append_row_boost (boost::python::object data );
    bool append_rows_boost (boost::python::object data,
			    long nrows );
    void write_col_by_index_boost (boost::python::numeric::array data,
				   int index,
				   int rownum,
				   long nrecords );
    boost::python::list listColumns_boost();
    boost::python::numeric::array getAttribute_boost(std::string);
    PyObject* readRows_boost (int start,
			      int nrecs);
    
    //! Set attribute of type \e char
    bool setAttribute_char( std::string attrname, char data );
    //! Set attribute of type \e string
    bool setAttribute_string( std::string attrname, std::string data );
    bool setAttribute_char_vector (std::string attrname, boost::python::list data);
    bool setAttribute_short_vector (std::string attrname, boost::python::list data);
    bool setAttribute_int_vector (std::string attrname, boost::python::list data);
    bool setAttribute_uint_vector (std::string attrname, boost::python::list data);
    bool setAttribute_long_vector (std::string attrname, boost::python::list data);
    bool setAttribute_float_vector (std::string attrname, boost::python::list data);
    bool setAttribute_double_vector (std::string attrname, boost::python::list data);
    bool setAttribute_string_vector (std::string attrname, boost::python::list data);
    
#ifdef DAL_WITH_CASA
    void ot_nonMStable( std::string const &tablename );
    void setFilter_boost1 (std::string);
    void setFilter_boost2 (std::string,std::string);
#endif
    
#endif

  // === Private methods ========================================================

 private:

  //! Initialize internal parameters
  void init ();
  //! Setup for adding another column to an HDF5 table
  bool h5addColumn_setup (std::string const &column_name,
			  bool &removedummy);
  void h5addColumn_insert (uint const & indims,
			   std::string const & colname,
			   hid_t const & field_type,
			   bool const & removedummy );

  };
  
} // end namespace DAL
#endif
