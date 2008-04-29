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

namespace DAL {

/*!
  \class dalGroup
  
  \ingroup DAL

  \brief Represents a group of tables, arrays or sub-groups.
  
  \author Joseph Masters
*/
class dalGroup{

 protected:
	void * file; //!< can be HDF5File, FITS, MS
	string name; //!< group name
	void * group;
	vector<dalTable> tables; //!< list of tables within groups
	vector<dalAttribute> attributes; //!< list of group attributes
	dalFilter * filter; //!< filter associated with group
	hid_t file_id; //!< hdf5 file_id
	hid_t group_id; //!< hdf5 group_id
	herr_t	status; //!< hdf5 return status

	dalGroup * subgroup;
	
 public:

	/*!
          \brief Default constructor.

          Default constructor.
         */
	dalGroup();

	/*!
          \brief Create a group in a certain file.

          Create a group in a certain file.

          \param groupname The name of the group to create.
          \param file A pointer to the file where you want to create the group.
         */
	dalGroup( const char * groupname, void * file );

	/*!
          \brief Create a subgroup.

          Create a subgroup in an existing group.

          \param group_id The parent group identifier.
          \param gname The name of the subgroup.
         */
	dalGroup( hid_t group_id, const char * gname );

	/*!
          \brief Default destructor.

          Default destructor.
         */
	~dalGroup();

	/*!
          \brief Open an existing group.

          Open an existing group.

          \param file A pointer to the file.
          \param groupname The name of the group you want to open.

          \return An identifier for the new group.
         */
	int open( void * file, string groupname );

	/*!
          \brief Close an existing group.

          Close an existing group.

         */
	int close();

	/*!
          \brief Get group name.

          Retrieve the name of the group object.

          \return The name of the group.
         */
	string getName();

	/*!
          \brief Set group name.

          Set the name of the group.

          \param gname The name of the group.

          \return Zero on success. Non-zero on failure.
         */
	bool setName( string gname );

        /*!
          \brief Create an array of shorts within the group.

          Create an array of shorts with any dimensions.  This is usually
          called from the dataset object and not from the developer.

          \param arrayname A string containing he name of the array.
          \param dims A vector specifying the array dimensions.
          \param data A structure containing the data to be written.  The size
                      of the data must match the provided dimensions.
          \param cdims The chunk dimensions for an extendible array.

          \return dalArray * A pointer to an array object.
        */
	dalArray * createShortArray(
				string arrayname,
				vector<int> dims,
				short data[],
				vector<int>cdims);

        /*!
          \brief Create an array of integers within the group.

          Create an array of integers with any dimensions.  This is usually
          called from the dataset object and not from the developer.

          \param arrayname A string containing he name of the array.
          \param dims A vector specifying the array dimensions.
          \param data A structure containing the data to be written.  The size
                      of the data must match the provided dimensions.
          \param cdims The chunk dimensions for an extendible array.

          \return dalArray * A pointer to an array object.
        */
	dalArray * createIntArray(
				string arrayname,
				vector<int> dims,
				int data[],
				vector<int>cdims);

        /*!
          \brief Create an array of floating point values within the group.

          Create an array of floating point values with any dimensions.  This
          is usually called from the dataset object and not from the developer.

          \param arrayname A string containing he name of the array.
          \param dims A vector specifying the array dimensions.
          \param data A structure containing the data to be written.  The size
                      of the data must match the provided dimensions.
          \param cdims The chunk dimensions for an extendible array.

          \return dalArray * A pointer to an array object.
        */
	dalArray * createFloatArray(
				string arrayname,
				vector<int> dims,
				float data[],
				vector<int>cdims);
        /*!
          \brief Get the group ID.

          Retrieve the identifier for the group.

          \return The group identifier as an integer.
        */
	hid_t getId();

        /*!
          \brief Retrieve the array or table member names.

          Retrief the array or table member names from the group.

          \return A vector of strings representing the member names.
         */
        vector<string> getMemberNames();

        /*!
          \brief Print the group attributes.

          Print the group attributes.
        */
	void getAttributes();

        /*!
          \brief Print the value of a specified attribute.

          Print the value of a specified attribute.

          \param attrname The name of the attribute you want to print.
        */
	void printAttribute( string attrname );

        /*!
          \brief Get the value of an attribute.

          Get the value of an attribute.  This is different from printAttribute
          because the value of the attribute is returned into a structure
          instead of simply printing.

          \param attrname The name of the attribute you want to retrieve.

          \return void * A pointer to the data in the attribute. 
        */
	void * getAttribute( string attrname );

        /*!
          \brief Define a string attribute.

          Define a string attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
        */
	void setAttribute_string( string attrname, string data );

        /*!
          \brief Define a integer attribute.

          Define a integer attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
          \param size Optional parameter specifying the array size of the
                      attribute.  Default is scalar.
        */
	void setAttribute_int( string attrname, int * data, int size=1 );

        /*!
          \brief Define an unsigned integer attribute.

          Define an unsigned integer attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
          \param size Optional parameter specifying the array size of the
                      attribute.  Default is scalar.
        */
	void setAttribute_uint(string attrname, unsigned int * data,int size=1);

        /*!
          \brief Define a double precision floating point attribute.

          Define a double precision floating point attribute.

          \param attrname The name of the attribute you want to create.
          \param data The value of the attribute you want to create.
          \param size Optional parameter specifying the array size of the
                      attribute.  Default is scalar.
        */
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
