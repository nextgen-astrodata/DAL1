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

#ifndef DALDATASET_H
#define DALDATASET_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALARRAY_H
#include "dalArray.h"
#endif

#ifndef DALTABLE_H
#include "dalTable.h"
#endif

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

namespace DAL
  {

  /*!
    \class dalDataset

    \ingroup DAL

    \brief Represents the file containing all sub-structures
           (tables, arrays, etc.)

    \author Joseph Masters

    <h3>Synopsis</h3>

    The dalDataset is the highest level container for dalData.  It may
    consist of one or more files on disk, each of which contain multiple tables,
    images and attributes.  These tables and images can be gruoped.
  */
  class dalDataset
    {

      void * file;  //!< can be HDF5File, FITS, MS
      std::string type;  //!< "HDF5", "MSCASA" or "FITS"; for example
      std::string name;  //!< dataset name
      std::vector<std::string> files;  //!< list of files
      std::vector<dalTable> tables; //!< list of tables
      std::vector<dalGroup> groups; //!< list of groups
      std::vector<dalAttribute> attrs;  //!< list of attributes

      dalFilter filter;	//!< dataset filter

      hid_t h5fh;   //!< hdf5 file handle
      herr_t status;

#ifdef WITH_CASA
      casa::MeasurementSet * ms; //!< CASA measurement set pointer
      casa::MSReader * ms_reader; //!< CASA measurement set reader pointer
      casa::Vector<casa::String> ms_tables; //!< vector of CASA MS tables
#endif

    public:
      dalDataset();
      void init();
      dalDataset( const char * name, std::string filetype );
      int open( const char * datasetname );
      int close();
      void getAttributes();
      void printAttribute( std::string attrname );
      void * getAttribute( std::string attrname );
      void setAttribute_string( std::string attrname, std::string data );
      void setAttribute_string( std::string attrname, std::vector<std::string> data );
      void setAttribute_int( std::string attrname, int * data, int size=1 );
      void setAttribute_uint(std::string attrname, unsigned int * data,int size=1);
      void setAttribute_double( std::string attrname, double * data, int size=1 );
      dalArray * createArray(
        std::string arrayname,
        dalData * data_object);
      dalArray * createIntArray(
        std::string arrayname,
        std::vector<int> dims,
        int data[],
        std::vector<int>cdims);
      dalArray * createFloatArray(
        std::string arrayname,
        std::vector<int> dims,
        float data[],
        std::vector<int>cdims);
      dalArray * createComplexFloatArray( std::string arrayname,
                                          std::vector<int> dims,
                                          complex<float> data[],
                                          std::vector<int>cdims );
      dalTable * createTable( std::string tablename );
      dalTable * createTable( std::string tablename, std::string groupname );
      dalGroup * createGroup( const char * groupname );
      dalTable * openTable( std::string tablename );
      void setFilter( std::string columns );
      void setFilter( std::string columns, std::string conditions );
      dalTable * openTable( std::string tablename, std::string groupname );
      dalArray * openArray( std::string arrayname );
      dalArray * openArray( std::string arrayname, std::string groupname );
      std::vector<std::string> getGroupNames();
      dalGroup * openGroup( std::string groupname );
      std::vector<std::string> listTables();
      std::string getType();
      /*!
        \brief Retrieve the name of the data set
        \return name -- A string holding the name of the data set.
       */
      inline std::string getName () const
        {
          return name;
        }
      /*!
        \brief Retrieve the list of files
        \return files -- List of files
      */
      inline std::vector<std::string> getFiles () const
        {
          return files;
        }
      /*!
        \brief Get the HDF5 file handle identifier.
        \return h5fh -- File handle identifier to communicate with the HDF5
                file through the HDF5 library.
       */
      inline hid_t getFileHandle () const
        {
          return h5fh;
        }

      void read_tbb(std::string id, int start, int length, short data_out[]);

      /************************************************************************
       *
       * The following functions are boost wrappers to allow some previously
       *   defined functions to be easily called from a python prompt.
       *
       ************************************************************************/
#ifdef PYTHON

      // create[]Array wrappers
      dalArray * cia_boost1(std::string arrayname, bpl::list dims, bpl::list data);
      dalArray * cia_boost2(std::string arrayname, bpl::list dims, bpl::list data,
                            bpl::list cdims );
      dalArray * cia_boost_numarray1( std::string arrayname, bpl::list dims,
                                      bpl::numeric::array data );
      dalArray * cia_boost_numarray2( std::string arrayname, bpl::list dims,
                                      bpl::numeric::array data, bpl::list cdims );
      dalArray * cfa_boost( std::string arrayname, bpl::list dims, bpl::list data,
                            bpl::list cdims );
      dalArray * cfa_boost_numarray( std::string arrayname, bpl::list dims,
                                     bpl::numeric::array data, bpl::list cdims );

      bpl::numeric::array ria_boost( std::string arrayname );
      bpl::numeric::array rfa_boost( std::string arrayname );

      // createTable wrappers
      dalTable * ct1_boost( std::string tablename );
      dalTable * ct2_boost( std::string tablename, std::string groupname );

      // openTable wrappers
      dalTable * ot1_boost( std::string tablename );
      dalTable * ot2_boost( std::string tablename, std::string groupname );

      void setFilter_boost1(std::string);
      void setFilter_boost2(std::string,std::string);

      bpl::list listTables_boost();
#endif
    };

} //end namespace DAL

#endif
