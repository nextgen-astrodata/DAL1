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

namespace DAL {
  
  /*!
    \class dalGroup
    \brief Represents a group of tables, arrays or sub-groups.

    \ingroup DAL
    \ingroup core

    \test tdalGroup.cc

    \author Joseph Masters
  */
  class dalGroup {
    
  protected:
    
    //! File handler, can be HDF5File, FITS, MS
    void * file;
    //! Name of the group
    string groupname_p;
    //! Full name of the group
    string groupname_full;
    void * group;
    //! Filter associated with group
    dalFilter * filter;
    //! HDF5 file ID
    hid_t file_id;
    //! HDF5 group ID
    hid_t group_id;
    //! HDF5 error code
    herr_t status;
    
  public:
    
    // === Construction / Destruction ===========================================

    //! Default constructor
    dalGroup ();
    //! Argumented constructor, creating a subgroup in an existing group
    dalGroup (hid_t location_id,
	      const char * name);
    //! Argumented constructor, creating a group in a certain file
    dalGroup (const char * name,
	      void * file );
    //! Default destructor
    ~dalGroup();

    // === Parameter access =====================================================

    //! Get the name of the group
    std::string getName();
    //! Set the name of the group
    bool setName( string gname );
    //! Retrieve the identifier for the group.
    hid_t getId();

    // === Methods ==============================================================

    //! Open an existing group.
    int open ( void * file, std::string groupname);
    //! Close the group
    bool close();
    //! Provide a summary of the object's properties
    inline void summary() {
      summary(std::cout);
    }
    //! Provide a summary of the object's properties
    void summary(std::ostream &os);
    //! Create an array of shorts within the group.
    dalArray * createShortArray(        std::string arrayname,
					std::vector<int> dims,
					short data[],
					std::vector<int>cdims);
    //! Create an array of ints within the group.
    dalArray * createIntArray(          std::string arrayname,
					std::vector<int> dims,
					int data[],
					std::vector<int>cdims);
    //! Create an array of floats within the group.
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
    //! Retrief the array or table member names from the group.
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
        return h5get_attribute( group_id, attrname, value );
      }
    
    bool setAttribute ( std::string attrname, char * data, int size=1 );
    bool setAttribute ( std::string attrname, short * data, int size=1 );
    bool setAttribute ( std::string attrname, int * data, int size=1 );
    bool setAttribute ( std::string attrname, uint * data, int size=1 );
    bool setAttribute ( std::string attrname, long * data, int size=1 );
    bool setAttribute ( std::string attrname, float * data, int size=1 );
    bool setAttribute ( std::string attrname, double * data, int size=1 );
    bool setAttribute ( std::string attrname, std::string data );
    bool setAttribute ( std::string attrname, std::string * data, int size=1 );
    
    dalGroup * createGroup( const char * gname );

    // === Boost.Python wrappers ================================================

#ifdef PYTHON
    dalArray * csa_boost_list( std::string arrayname, bpl::list dims, bpl::list data);
    dalArray * cia_boost_list( std::string arrayname, bpl::list dims, bpl::list data);
    dalArray * cfa_boost_list( std::string arrayname, bpl::list dims, bpl::list data );
    
    bpl::numeric::array ria_boost( std::string arrayname );
    
    bool setAttribute_char( std::string attrname, char data );
    bool setAttribute_short( std::string attrname, short data );
    bool setAttribute_int( std::string attrname, int data );
    bool setAttribute_uint( std::string attrname, uint data );
    bool setAttribute_long( std::string attrname, long data );
    bool setAttribute_float( std::string attrname, float data );
    bool setAttribute_double( std::string attrname, double data );
    bool setAttribute_string( std::string attrname, std::string data );
    bool setAttribute_char_vector( std::string attrname, bpl::list data );
    bool setAttribute_short_vector( std::string attrname, bpl::list data );
    bool setAttribute_int_vector( std::string attrname, bpl::list data );
    bool setAttribute_uint_vector( std::string attrname, bpl::list data );
    bool setAttribute_long_vector( std::string attrname, bpl::list data );
    bool setAttribute_float_vector( std::string attrname, bpl::list data );
    bool setAttribute_double_vector( std::string attrname, bpl::list data );
    bool setAttribute_string_vector( std::string attrname, bpl::list data );
    
    bpl::numeric::array getAttribute_float_boost (std::string attrname);
    bpl::numeric::array getAttribute_double_boost (std::string attrname);
    bpl::numeric::array getAttribute_int_boost (std::string attrname);
    bpl::numeric::array getAttribute_uint_boost (std::string attrname);
    bpl::numeric::array getAttribute_short_boost (std::string attrname);
    bpl::numeric::array getAttribute_long_boost (std::string attrname);
    bpl::list getAttribute_string_boost (std::string attrname);
    
#endif // end #ifdef PYTHON

    // === private functions ====================================================

    void init ();

  }; // end class dalGroup
  
} // end namespace DAL

#endif  // end #ifndef DALGROUP_H
