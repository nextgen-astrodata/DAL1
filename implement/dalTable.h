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

namespace DAL
  {

  /*!
    \class dalTable
    \ingroup DAL
    \brief Object representing a table within a dataset.

    A dalTable can reside within a dataset, or within a group that is within
    a dataset.

   */

  class dalTable
    {

      void * file;  // can be HDF5File, FITS, MS

      // HDF5-specific variables
      hid_t file_id; // hdf5 file_id
      hid_t table_id; // hdf5 table id
      hsize_t nfields; // hdf5 field count
      hsize_t nrecords; // hdf5 record count
      herr_t status; // hdf5 return status
      char **field_names;  // hdf5 list of columns

      dalFilter * filter; // table filter

      bool firstrecord;
      string name; // table name
      string type;  // "HDF5", "MSCASA" or "FITS"; for example
      vector<dalColumn> columns; // list of table columns

#ifdef WITH_CASA

      casa::Table * casa_table_handle;
      casa::Array<casa::Double> array_vals_dbl;
      casa::Array<casa::Complex> array_vals_comp;
      casa::ROTableColumn * casa_column;
#endif

      void h5addColumn_setup( std::string const column_name, bool &removedummy );
      void h5addColumn_insert( uint const & indims, std::string const & colname,
							   hid_t const & field_type, bool const & removedummy );

    public:

      dalTable();
      dalTable( string filetype );
      ~dalTable();
      void printColumns();
      void openTable( void * voidfile, string tablename, string groupname );

#ifdef WITH_CASA
      void openTable( string tablename );
      void openTable( string tablename,
                      casa::MSReader * reader );
      void openTable( string tablename,
                      casa::MSReader * reader,
                      dalFilter * filter );

      casa::Bool GetKeyword( casa::String const KeywordName,
                             casa::String *result);
      casa::Bool GetKeyword( casa::String const KeywordName,
                             casa::Double *result);
      casa::Bool GetKeyword( casa::String const KeywordName,
                             casa::Float *result);
      casa::Bool GetKeyword( casa::String const KeywordName,
                             casa::DComplex *result);
      casa::Bool GetKeyword( casa::String const KeywordName,
                             casa::Array<casa::Double> *result);
      casa::Bool GetKeyword( casa::String const KeywordName,
                             casa::Array<casa::DComplex> *result);
      casa::String GetKeywordType(casa::String const KeywordName);
#endif

      void createTable( void * voidfile, string tablename, string groupname );
      dalColumn * getColumn_complexInt16( string colname );
      dalColumn * getColumn_complexFloat32( string colname );
      dalColumn * getColumn_Float32( string colname );
      dalColumn * getColumn( string colname );
      void addColumn( string colname, string coltype, uint dims=1);
      void addComplexColumn( string compname,
                             vector<dalColumn> ri,
                             int subfields );
      void removeColumn( const string &colname );
      void writeDataByColNum( void * structure, int index, int rownum );
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
        return h5getAttribute( table_id, attrname, value );
      }

      bool setAttribute_string( string attrname, string data );
      void setAttribute_char( string attrname, char * data, int size=1 );
      bool setAttribute_int( string attrname, int * data, int size=1 );
      bool setAttribute_uint( string attrname, unsigned int * data, int size=1 );
      bool setAttribute_double( string attrname, double * data, int size=1 );
      bool findAttribute( string attrname );
      long getNumberOfRows();
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
      void append_row_boost( bpl::object data );
      bpl::list listColumns_boost();
      bpl::numeric::array getAttribute_boost(std::string);

#ifdef WITH_CASA
      void ot_nonMStable( string tablename );
      void setFilter_boost1(string);
      void setFilter_boost2(string,string);
// 	bpl::numeric::array getColumnData_boost( string colname );
#endif

#endif
    };

} // end namespace DAL
#endif
