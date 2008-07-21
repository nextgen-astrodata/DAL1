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

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALTABLE_H
#include "dalTable.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif


#ifndef DALFILTER_H
#include "dalFilter.h"
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
    \author Joseph Masters
  */
  class dalGroup
    {

    protected:

      void * file; //!< can be HDF5File, FITS, MS
      string groupname; //!< group name
      string groupname_full;
      void * group;
      vector<dalTable> tables; //!< list of tables within groups
      vector<dalAttribute> attributes; //!< list of group attributes
      dalFilter * filter; //!< filter associated with group
      hid_t file_id; //!< hdf5 file_id
      hid_t group_id; //!< hdf5 group_id
      herr_t status; //!< hdf5 return status
      dalGroup * subgroup;

    public:

      dalGroup();
      dalGroup( const char * groupname, void * file );
      dalGroup( hid_t group_id, const char * gname );
      ~dalGroup();
      int open( void * file, string groupname );
      string getName();
      bool setName( string gname );
      dalArray * createShortArray(
        string arrayname,
        vector<int> dims,
        short data[],
        vector<int>cdims);
      dalArray * createIntArray(
        string arrayname,
        vector<int> dims,
        int data[],
        vector<int>cdims);
      dalArray * createFloatArray(
        string arrayname,
        vector<int> dims,
        float data[],
        vector<int>cdims);
      dalArray * createComplexFloatArray( string arrayname,
                                          vector<int> dims,
                                          complex<float> data[],
                                          vector<int>cdims );
      dalArray * createComplexShortArray( string arrayname,
                                          vector<int> dims,
                                          complex<Int16> data[],
                                          vector<int>cdims );
      hid_t getId();
      vector<string> getMemberNames();
      void getAttributes();
      void printAttribute( string attrname );
      void * getAttribute( string attrname );
      void setAttribute_string( string attrname, string data );
      void setAttribute_string( string attrname, string * data, int size=1 );
      void setAttribute_int( string attrname, int * data, int size=1 );
      void setAttribute_uint(string attrname, unsigned int * data,int size=1);
      void setAttribute_double( string attrname, double * data, int size=1 );

      /************************************************************************
       *
       * The following functions are boost wrappers to allow some previously
       *   defined functions to be easily called from a python prompt.
       *
       ************************************************************************/
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
#endif // end #ifdef PYTHON
    };

} // end namespace DAL

#endif  // end #ifndef DALGROUP_H
