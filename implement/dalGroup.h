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

#ifndef DALGROUP_H
#define DALGROUP_H

#ifndef DALTABLE_H
#include "dalTable.h"
#endif

#ifndef DALARRAY_H
#include "dalArray.h"
#endif

namespace DAL
  {

  /*!
    \class dalGroup
    \ingroup DAL
    \brief Represents a group of tables, arrays or sub-groups.
  */
  class dalGroup
    {

    protected:

      void * file; // can be HDF5File, FITS, MS
      string groupname; // group name
      string groupname_full;
      void * group;
      dalFilter * filter; // filter associated with group
      hid_t file_id; // hdf5 file_id
      hid_t group_id; // hdf5 group_id
      herr_t status;

    public:

      dalGroup();
      dalGroup( const char * groupname, void * file );
      dalGroup( hid_t group_id, const char * gname );
      ~dalGroup();
      int open( void * file, std::string groupname );
      bool close();
      std::string getName();
      bool setName( string gname );

      dalArray * createShortArray(        std::string arrayname,
                                          std::vector<int> dims,
                                          short data[],
                                          std::vector<int>cdims);

      dalArray * createIntArray(          std::string arrayname,
                                          std::vector<int> dims,
                                          int data[],
                                          std::vector<int>cdims);

      dalArray * createFloatArray(        std::string arrayname,
                                          std::vector<int> dims,
                                          float data[],
                                          std::vector<int>cdims);

      dalArray * createComplexFloatArray( std::string arrayname,
                                          std::vector<int> dims,
                                          complex<float> data[],
                                          std::vector<int>cdims );

      dalArray * createComplexShortArray( std::string arrayname,
                                          std::vector<int> dims,
                                          complex<Int16> data[],
                                          std::vector<int>cdims );
      hid_t getId();
      std::vector<std::string> getMemberNames();

      // ------------------------------------------------------- getAttribute

      /*!
        \brief Get the value of an attribute.

        Get the value of an attribute.  This is different from printAttribute
        because the value of the attribute is returned into a structure
        instead of simply printing.

        \param attrname The name of the attribute you want to retrieve.
        \param value The value of the attribute.
        \return bool -- DAL::FAIL or DAL::SUCCESS
      */
      template <class T>
      bool getAttribute( std::string attrname, T &value )
      {
        return h5getAttribute( group_id, attrname, value );
      }

      bool setAttribute( std::string attrname, char * data, int size=1 );
      bool setAttribute( std::string attrname, short * data, int size=1 );
      bool setAttribute( std::string attrname, int * data, int size=1 );
      bool setAttribute( std::string attrname, uint * data, int size=1 );
      bool setAttribute( std::string attrname, long * data, int size=1 );
      bool setAttribute( std::string attrname, float * data, int size=1 );
      bool setAttribute( std::string attrname, double * data, int size=1 );
      bool setAttribute( std::string attrname, std::string data );
      bool setAttribute( std::string attrname, std::string * data, int size=1 );

      dalGroup * createGroup( const char * gname );

      /******************************************************************
       *
       * The following functions are boost wrappers to allow some previously
       *   defined functions to be easily called from a python prompt.
       *
       ******************************************************************/
#ifdef PYTHON
      dalArray * cia_boost1(string arrayname, bpl::list dims, bpl::list data);
      dalArray * cia_boost2(string arrayname, bpl::list dims, bpl::list data,
                            bpl::list cdims );
      dalArray * cia_boost_numarray1( string arrayname, bpl::list dims,
                                      bpl::numeric::array data );
      dalArray * cia_boost_numarray2( string arrayname, bpl::list dims,
                                      bpl::numeric::array data, bpl::list cdims );
      dalArray * cfa_boost( string arrayname, bpl::list dims, bpl::list data,
                            bpl::list cdims );
      dalArray * cfa_boost_numarray( string arrayname, bpl::list dims,
                                     bpl::numeric::array data, bpl::list cdims );
      bpl::numeric::array ria_boost( string arrayname );

      bool setAttribute_char( std::string attrname, char data );
      bool setAttribute_short( std::string attrname, short data );
      bool setAttribute_int( std::string attrname, int data );
      bool setAttribute_uint( std::string attrname, uint data );
      bool setAttribute_long( std::string attrname, long data );
      bool setAttribute_float( std::string attrname, float data );
      bool setAttribute_double( std::string attrname, double data );
      bool setAttribute_string( std::string attrname, std::string data );
#endif // end #ifdef PYTHON
    };

} // end namespace DAL

#endif  // end #ifndef DALGROUP_H
