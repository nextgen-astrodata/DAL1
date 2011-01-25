/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

#ifndef HDF5OBJECT_H
#define HDF5OBJECT_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>

#ifdef HAVE_HDF5
#include <hdf5.h>
#endif

#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Object
    
    \ingroup DAL
    \ingroup core
    
    \brief Basic common functionality of a HDF5 object
    
    \author Lars B&auml;hren

    \date 2011/01/17

    \test tHDF5Object.cc
    
    <h3>Prerequisite</h3>
    
    <h3>Synopsis</h3>

    Given the fact that sometimes even simple inspection operations on an HDF5 
    object may require multiple HDF5 library calls, the attempt was made to wrap
    some of the frequently required operations in a simpler to use form.

    <ul type="square">
      <li>H5I: Identifier Interface of the HDF5 library. These functions
      provides tools for working with object identifiers and object names.

      <li>H5L: Link Interface of the HDF5 library. These functions create and
      manipulate links in an HDF5 group

      The main functions utilized/wrapped are:

      <ul>
        <li>\c H5Literate -- Iterates through links in a group.
	\code
	herr_t H5Literate (hid_t group_id,
	                   H5_index_t index_type,
			   H5_iter_order_t order,
			   hsize_t *idx,
			   H5L_iterate_t op,
			   void *op_data ) 
	\endcode
	H5Literate iterates through the links in a group, specified by
	\c group_id, in the order of the specified index, \c index_type,
	using a user-defined callback routine \c op. \c H5Literate does not
	recursively follow links into subgroups of the specified group.
	
	The protoype of the callback function op is as follows:
	\code
	herr_t (*H5L_iterate_t) (hid_t g_id,
	                         const char *name,
				 const H5L_info_t *info,
				 void *op_data ) 
	\endcode
      </ul>

      <li>H5O: Object Interface of the HDF5 library. The Object interface, H5O,
      functions manipulate objects in an HDF5 file. This interface is designed
      to be used in conjunction with the Links interface (H5L).

      The main functions utilized/wrapped are:
      <ul>
        <li>\c H5Oget_info -- Retrieves the metadata for an object specified by
	an identifier.
	\code
	herr_t H5Oget_info (hid_t object_id,
	                    H5O_info_t *object_info )
	\endcode
        <li>\c H5Ovisit -- Recursively visits all objects accessible from a
	specified object.
	\code
	herr_t H5Ovisit (hid_t object_id,
	                 H5_index_t index_type,
			 H5_iter_order_t order,
			 H5O_iterate_t op,
			 void *op_data )
	\endcode
	H5Ovisit is a recursive iteration function to visit the object 
	\c object_id and, if \c object_id is a group, all objects in and below
	it in an HDF5 file, thus providing a mechanism for an application to
	perform a common set of operations across all of those objects or a
	dynamically selected subset. For non-recursive iteration across the
	members of a group, see \e H5Literate.

	If \c object_id is a group identifier, that group serves as the root
	of a recursive iteration. If \c object_id is a file identifier, that
	file’s root group serves as the root of the recursive iteration. If
	\c object_id is any other type of object, such as a dataset or named
	datatype, there is no iteration.

	Two parameters are used to establish the iteration: \c index_type and
	\c order. 
      </ul>
    </ul>
    
    <h3>Example(s)</h3>

    \todo Consider splitting off most of the static functions to a separate file
    \e HDF5ObjectUtils.
  */  
  class HDF5Object {

  protected:

    //! HDF5 object identifier
    hid_t itsLocation;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Object ();
    
    //! Argumented constructor
    HDF5Object (std::string const &filename,
		hid_t const &access=H5P_DEFAULT);
    
    //! Argumented constructor
    HDF5Object (hid_t const &location,
		std::string const &name,
		hid_t const &access=H5P_DEFAULT);
    
    //! Copy constructor
    HDF5Object (HDF5Object const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Object ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Object& operator= (HDF5Object const &other); 
    
    // === Parameter access =====================================================
    
    //! Name of the object
    inline std::string name () {
      return name (itsLocation);
    }
    //! Get object type
    inline H5I_type_t objectType () {
      return objectType (itsLocation);
    }
    //! Get object type as name
    inline std::string objectName () {
      return objectName (itsLocation);
    }
    //! Object address in file
    inline haddr_t objectAddress () {
      return objectAddress (itsLocation);
    }
    //! Reference count of object
    inline int referenceCount () {
      return referenceCount (itsLocation);
    }
    //! Access time
    inline time_t accessTime () {
      return accessTime (itsLocation);
    }
    //! Modification time
    inline time_t modificationTime () {
      return modificationTime (itsLocation);
    }
    //! Change time
    inline time_t changeTime () {
      return changeTime (itsLocation);
    }
    //! Birth time
    inline time_t birthTime () {
      return birthTime (itsLocation);
    }
    //! nof. attributes attached to the object
    inline hsize_t nofAttributes () {
      return nofAttributes (itsLocation);
    }

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5Object.
    */
    inline std::string className () const {
      return "HDF5Object";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Open HDF5 file
    bool open (std::string const &filename,
	       hid_t const &access=H5P_DEFAULT);
    
    // === Static methods =======================================================

    //! Map between object types and object names
    static std::map<H5I_type_t,std::string> objectTypesMap ();
    //! Object types
    static std::vector<H5I_type_t> objectTypes ();
    //! Object types
    static std::vector<std::string> objectNames ();

    //! Retrieves a name of an object based on the object identifier.
    static std::string name (hid_t const &location);
    //! Get object type
    static H5I_type_t objectType (hid_t const &location);
    //! Get object type
    static H5I_type_t objectType (hid_t const &location,
				  std::string const &name);
    //! Get object type as name/string.
    static std::string objectName (hid_t const &location);
    //! Retrieves the metadata for an object specified by \e location.
    static H5O_info_t objectInfo  (hid_t const &location);
    //! Object address in file
    static haddr_t objectAddress (hid_t const &location);
    //! Get reference count of object
    static int referenceCount (hid_t const &location);
    //! Access time
    static time_t accessTime (hid_t const &location);
    //! Modification time
    static time_t modificationTime (hid_t const &location);
    //! Change time
    static time_t changeTime (hid_t const &location);
    //! Birth time
    static time_t birthTime (hid_t const &location);
    //! nof. attributes attached to the object
    static hsize_t nofAttributes (hid_t const &location);
    
    //! Open an object in an HDF5 file
    static hid_t open (hid_t const &location,
		       std::string const &name,
		       hid_t const &access=H5P_DEFAULT);
    //! Open an object in an HDF5 file
    static hid_t open (hid_t const &location,
		       std::string const &name,
		       H5I_type_t const &otype,
		       hid_t const &access=H5P_DEFAULT);
    //! Closes an object in an HDF5 file.
    static herr_t close (hid_t const &location);
    
  private:

    //! Unconditional copying
    void copy (HDF5Object const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Object -- end
  
} // Namespace DAL -- end

#endif /* HDF5OBJECT_H */
  
