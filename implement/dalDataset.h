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

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

namespace DAL
  {

  /*!
    \class dalDataset
    \ingroup DAL
    \brief Represents the file containing all sub-structures (tables, arrays, etc.)

    <h3>Prerequisite</h3>

    <ul>
      <li><a href="http://www.hdfgroup.org/HDF5/doc/RM/PredefDTypes.html">HDF5
      Predefined Datatypes</a>
    </ul>

    <h3>Synopsis</h3>

    The dalDataset is the highest level container for dalData.  It may
    consist of one or more files on disk, each of which contain multiple tables,
    images and attributes.  These tables and images can be grouped.

    <h3>Examples</h3>

    <ol>
      <li>Create new object through default constuctor:
      \code
      #include <dal/dalDataset.h>

      DAL::dalDataset dataset;
      \endcode
      and display some of its internal properties
      \code
      dataset.summary();
      \endcode
      <li>Create object for an already existing dataset on disk:
      \code
      std::string filename ("mydata.h5");

      DAL::dalDataset dataset (filename.c_str(),"HDF5");
      \endcode
      <li>Obtain a summary of the object's internal properties:
      \code
      dataset.summary();
      \endcode
      The output will look something like this:
      \verbatim
      [dalDataset] Summary of object properties
      -- Dataset type     = HDF5
      -- Dataset name     = /Users/lars/Code/lofar/usg/data/bf_20080604_121337.cor.h5
      -- HDF5 file handle = 16777216
      -- HDF5 group ID    = 16777216
      \endverbatim
      <li>Get the values of the internal parameters handling/describing the access
      to the dataset on disk:
      \code
      // Get the group ID
      dataset.getId();
      // Retrieve the dataset type ("HDF5", "MSCASA", etc.)
      dataset.getType();
      // Retrieve the name of the data set
      dataset.getName();
      // Get the HDF5 file handle identifier
      dataset.getFileHandle();
      \endcode
    </ol>
  */

  class dalDataset
    {

      void init();

      //! can be HDF5File, FITS, MS
      void * file;
      //! "HDF5", "MSCASA" or "FITS"; for example
      std::string type;
      //! Dataset name
      std::string name;
      //! Dataset filter
      dalFilter filter;
      //! HDF5 file handle
      hid_t h5fh_p;

#ifdef HAVE_CASA
      casa::MeasurementSet * ms; // CASA measurement set pointer
      casa::MSReader * ms_reader; // CASA measurement set reader pointer
      casa::Vector<casa::String> ms_tables; // vector of CASA MS tables
#endif

    public:
      dalDataset();
      dalDataset( const char * name,
                  std::string filetype,
                  const bool &overwrite=false);
      bool open( const char * datasetname );
      bool close();
      bool getAttributes();
      hid_t getId();

      /*!
      \brief Provide a summary of the internal status
      */
      inline void summary ()
      {
        summary (std::cout);
      }
      void summary (std::ostream &os);

      // ---------------------------------------------------------- getAttribute

      /*!
        \brief Get the value of an attribute.

        Get the value of an attribute.  This is different from printAttribute
        because the value of the attribute is returned into a structure
        instead of simply printing.

        \param attrname The name of the attribute you want to retrieve.
        \param value Attribute value to return.
        \return bool -- DAL::FAIL or DAL::SUCCESS
       */
      template <class T>
      bool getAttribute( std::string attrname, T &value )
      {
        return h5get_attribute( h5fh_p, attrname, value );
      }

      //! Define a char attribute.
      bool setAttribute( std::string attrname, char * data, int size=1 );
      //! Define a short attribute.
      bool setAttribute( std::string attrname, short * data, int size=1 );
      //! Define an integer attribute.
      bool setAttribute( std::string attrname, int * data, int size=1 );
#ifndef INT64_IS_LONG_INT
      //! Define an integer attribute.
      bool setAttribute( std::string attrname, int64_t * data, int size=1 );
#endif
      //! Define an unsigned integer attribute.
      bool setAttribute( std::string attrname, uint * data, int size=1 );
      //! Define a long attribute.
      bool setAttribute( std::string attrname, long * data, int size=1 );
      //! Define a floating point attribute.
      bool setAttribute( std::string attrname, float * data, int size=1 );
      //! Define a double precision floating point attribute.
      bool setAttribute( std::string attrname, double * data, int size=1 );
      //! Define a string attribute.
      bool setAttribute( std::string attrname, std::string data );
      //! Define a string attribute.
      bool setAttribute( std::string attrname, std::string * data, int size=1 );
      //! Define a string attribute.
      bool setAttribute_string( std::string attrname,
                                std::vector<std::string> data );

      dalArray * createArray( std::string arrayname, dalData * data_object );
      dalArray * createIntArray( std::string arrayname, std::vector<int> dims,
                                 int data[], std::vector<int>cdims );
      dalArray * createFloatArray( std::string arrayname, std::vector<int> dims,
                                   float data[], std::vector<int>cdims );
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
      //! Open a table within a group
      dalTable * openTable( std::string const &tablename,
                            std::string const &groupname );
      dalArray * openArray( std::string arrayname );
      dalArray * openArray( std::string arrayname, std::string groupname );
      std::vector<std::string> getGroupNames();
      dalGroup * openGroup( std::string groupname );
      std::vector<std::string> listTables();
      std::string getType();

      /*!
        \brief Retrieve the name of the data set.

        Retrieve the name of the data set.

        \return name -- A string holding the name of the data set.
       */
      inline std::string getName () const
        {
          return name;
        }

      /*!
        \brief Get the HDF5 file handle identifier.

        Get the HDF5 file handle identifier.

        \return h5fh -- File handle identifier to communicate with the HDF5
                file through the HDF5 library.
       */
      inline hid_t getFileHandle () const
        {
          return h5fh_p;
        }

      void read_tbb(std::string id,
                    int start,
                    int length,
                    short data_out[]);

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

      bool setAttribute_char( std::string attrname, char data );
      bool setAttribute_short( std::string attrname, short data );
      bool setAttribute_int( std::string attrname, int data );
      bool setAttribute_uint( std::string attrname, uint data );
      bool setAttribute_long( std::string attrname, long data );
      bool setAttribute_float( std::string attrname, float data );
      bool setAttribute_double( std::string attrname, double data );
      bool setAttribute_string_boost( std::string attrname, std::string data );
      bool setAttribute_char_vector( std::string attrname, bpl::list data );
      bool setAttribute_short_vector( std::string attrname, bpl::list data );
      bool setAttribute_int_vector( std::string attrname, bpl::list data );
      bool setAttribute_uint_vector( std::string attrname, bpl::list data );
      bool setAttribute_long_vector( std::string attrname, bpl::list data );
      bool setAttribute_float_vector( std::string attrname, bpl::list data );
      bool setAttribute_double_vector( std::string attrname, bpl::list data );
      bool setAttribute_string_vector( std::string attrname, bpl::list data );
#endif
    };

} //end namespace DAL

#endif
