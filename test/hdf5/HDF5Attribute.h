/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#ifndef HDF5ATTRIBUTE_H
#define HDF5ATTRIBUTE_H

#include <iostream>
#include <string>
#include <vector>

#include "hdf5.h"

namespace DAL {

  /*!
    \class HDF5Attribute
    \ingroup DAL
    \ingroup data_common
    \brief Collection of methods to deal with HDF5 attributes
    \author Lars B&auml;hren
    \date 2010/01/11

    <h3>Synopsis</h3>

    \code
    template <class T>
      static bool getAttribute (hid_t const &location,
				std::string const &name,
				T &value);
    
    ## Cast single value into vector -->

    template <class T>
      static bool getAttribute (hid_t const &location,
				std::string const &name,
				std::vector<T> &value);
    \endcode
   */
  class HDF5Attribute {

    //! HDF5 object identifier
    hid_t location_p;
    //! Name of the attribute
    std::string name_p;
    //! Datatype of the attribute
    hid_t datatype_p;

  public:

    // === Construction =========================================================

    //! Default constructor
    HDF5Attribute ();

    //! Argumented constructor
    HDF5Attribute (hid_t const &location,
		   std::string const &name);

    // === Destruction ==========================================================

    //! Default destructor
    ~HDF5Attribute ();

    // === Parameter access =====================================================

    //! Get the HDF5 object identifier
    inline hid_t location () const {
      return location_p;
    }

    //! Get the name of the attribute
    inline std::string name () const {
      return name_p;
    }

    // === Methods ==============================================================

    //! Get the of the attribute's dataspace
    int rank ();

    //! Provide a summary of the objects internal parameters
    void summary (std::ostream &os=std::cout);

    // === Static methods =======================================================

    //! Get the of the attribute's dataspace
    static int rank (hid_t const &location);

    //! Get the name of a datatype
    static std::string getDatatype (hid_t const &id);
    
    //! Get attribute \e name attached to \e location in a HDF5 file
    template <class T>
      static bool getAttribute (hid_t const &location,
				std::string const &name,
				T * value,
				int &size,
				hid_t const &datatype)
      {
	bool status (true);

	if (H5Aexists(location,name.c_str())) {
	  herr_t h5error;
	  hid_t attributeID;
	  hid_t datatypeID;
	  // Open the attribute and get its datatype
	  attributeID = H5Aopen (location,
				 name.c_str(),
				 H5P_DEFAULT);
	  datatypeID  = H5Aget_type (attributeID);

	  // Check if the datatype matches 
	  if (datatypeID == datatype) {
	    // Additional local variables
	    hsize_t * sdim = new hsize_t[64];
	    hid_t dataspaceID;
	    // Get attribute dataspace, rank, and dimensions.
	    dataspaceID = H5Aget_space (attributeID);
	    h5error     = H5Sget_simple_extent_dims (dataspaceID, sdim, NULL);
	    size        = H5Sget_simple_extent_npoints (dataspaceID);
	    // Resize the array returning the value of the attribute
	    if (value == NULL) {
	      delete [] value;
	    }
	    value = new T [size];
	    // Read the attribute
	    h5error = H5Aread (attributeID, datatype, value);
	    // relase allocated memory
	    delete [] sdim;
	    // Close HDF5 identifiers
	    h5error = H5Sclose (dataspaceID);
	  }  //  end -- (datatypeID == datatype)
	  else {
	    std::cerr << "[HDF5Attribute::getAttribute] Attribute "
		      << name << " not of correct datatype!"          << std::endl;
	    std::cerr << " -- Expected : " << getDatatype(datatype)   << std::endl;
	    std::cerr << " -- Found    : " << getDatatype(datatypeID) << std::endl;
	    std::cerr << std::flush;
	    status = false;
	  }
	  // Close HDF5 identifiers
	  h5error = H5Tclose (datatypeID);
	  h5error = H5Aclose (attributeID);
	} else {
	  std::cerr << "[HDF5Attribute::getAttribute] No attribute "
		    << name << " at location " << location << "!"
		    << std::endl;
	  status = false;
	}
	
	return status;
      }
    
    //! Get attribute \e name attached to \e location in a HDF5 file
    template <class T>
      static bool getAttribute (hid_t const &location,
				std::string const &name,
				T * value,
				int size);
    
    //! Get attribute \e name attached to \e location in a HDF5 file
    template <class T>
      static bool getAttribute (hid_t const &location,
				std::string const &name,
				std::vector<T> &value)
      {
	int size (0);
	T *tmp;
	bool status = getAttribute(location,name,tmp,size);
	
	if (status) {
	  value.resize(size);
	  for (int n(0); n<size; ++n) {
	    value[n] = tmp[n];
	  }
	}
	
	// release allocated memory
	if (tmp != NULL) delete [] tmp;

	return status;
      }
    
    //! Get attribute \e name attached to \e location in a HDF5 file
    template <class T>
      static bool getAttribute (hid_t const &location,
				std::string const &name,
				T &value)
      {
	int size (0);
	T *tmp;
	bool status = getAttribute(location,name,tmp,size);
	
	if (status && size==1) {
	  value = tmp[0];
	} else {
	  value = T();
	}
	
	// release allocated memory
	if (tmp != NULL) delete [] tmp;

	return status;
      }

  private:

    void init ();
    
  };  //  end -- class HDF5Attribute
  
};  //  end -- namespace DAL

#endif  // end -- HDF5ATTRIBUTE_H
