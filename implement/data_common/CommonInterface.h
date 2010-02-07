/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 2286 2009-02-03 10:50:48Z baehren                    $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#ifndef COMMONINTERFACE_H
#define COMMONINTERFACE_H

// Standard library header files
#include <iostream>
#include <map>
#include <set>
#include <string>

// casacore header files
#ifdef HAVE_CASA
#include <casa/Arrays/Vector.h>
#endif

// DAL header files
#include <dalCommon.h>
#include <CommonAttributes.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CommonInterface

    \ingroup DAL
    \ingroup data_common
    
    \brief Common functionality for the high-level interfaces to the datasets
    
    \author Lars B&auml;hren

    \date 2009/10/29

    <h3 name="synopsis">Synopsis</h3>
    
    Bearing in mind the internal organization of the Data Access Library, there
    are quite a number of classes, which provide a simple and high-level interface
    to the underlying data; any of these classes typically will represent an
    element within the hierarchical structure of a dataset. This class aims at 
    proving a minimum amount of common functionality across these classes.

    Independent of the specific hierarchical stucture, there are a number of 
    book-keeping and access operations which need to be in place:
    <ul>
      <li>Keep track of the (HDF5) location identifier, which is used to access 
      the current structure within a file.
      <li>Keep a list of the attributes which are attached to the current
      structure.
      <li>Provide read/write access to the attributes attached to the current
      structure.
    </ul>

    The collection of attributes attached to a given structure is stored as
    a STL set:
    \code
    std::set<std::string> attributes_p;
    \endcode

    <h3>Requirements for derived classes</h3>

    The CommonInterface requires derived classes to implement the following
    methods:

    <ol>
      <li><b>setAttributes()</b> <br> Set up the list of attributes attached
      to the structure.
      \code
      void BF_PrimaryPointing::setAttributes ()
      {
        attributes_p.clear();
	
	attributes_p.insert("GROUPTYPE");
	attributes_p.insert("NOF_STATIONS");
	attributes_p.insert("STATIONS_LIST");
	attributes_p.insert("POINT_RA");
	attributes_p.insert("POINT_DEC");
	attributes_p.insert("TRACKING");
      }
      \endcode
      <li><b>open (hid_t const &, std::string const &, bool const &)</b> <br>
      Open a structure (file, group, dataset, etc.). Though this signature it
      rather generatic, there is at least one case, where not all of the input
      parameters can be utilized: when opening a file (e.g. with BF_Dataset)
      the <tt>location</tt> will not be evaluated.
      \code
  bool BF_PrimaryPointing::open (hid_t const &location,
				 std::string const &name,
				 bool const &create)
  {
    bool status (true);
    
    // Set up the list of attributes attached to the root group
    setAttributes();
    
    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      location_p = H5Gopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
    } else {
      location_p = 0;
    }

    if (location_p > 0) {
      status = true;
    } else {
      // If failed to open the group, check if we are supposed to create one
      if (create) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	// If creation was sucessful, add attributes with default values
	if (location_p > 0) {
	  // ... write default values for the attributes
	} else {
	  std::cerr << "[BF_PrimaryPointing::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_PrimaryPointing::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[BF_PrimaryPointing::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
      \endcode
      <li><b>openEmbedded (bool const &)</b> <br> Open the structures embedded
      within the current one.
      \code
      bool BF_PrimaryPointing::openEmbedded (bool const &create)
      {
        bool status (create);
	std::set<std::string> groupnames;
	std::set<std::string>::iterator it;
	
	// Retrieve the names of the groups attached to the PrimaryPointing group
	status = h5get_names (groupnames,
	                      location_p,
			      H5G_GROUP);
	
	if (groupnames.size() > 0) {
	  for (it=groupnames.begin(); it!=groupnames.end(); ++it) {
	    beams_p[*it] = BF_Beam (location_p,*it);
	  }
	}
      
	return status;
      }
      \endcode
    </ol>
  */  
  class CommonInterface {

  protected:

    /* === Protected data which are used by derived classes === */

    //! HDF5 identifier for the location
    hid_t location_p;
    //! Names of the attributes attached to the structure
    std::set<std::string> attributes_p;

    /* === Protected functions which define basic interface === */

    /*!
      \brief Open the structures embedded within the current one

      \param create -- Create the corresponding data structure, if it does not 
             exist yet?
      
      \return status -- Status of the operation; returns <tt>false</tt> in case
              an error was encountered.
    */
    virtual bool openEmbedded (bool const &create) = 0;
    //! Set up the list of attributes attached to the structure
    virtual void setAttributes () = 0;

  public:

    // === Destruction ==========================================================

    virtual ~CommonInterface () {};

    // === Operators ============================================================

    //! Copy operator
    CommonInterface& operator= (CommonInterface const &other); 
    
    // === Parameter accesss ====================================================
    
    //! Get the identifier for the location (within a dataset)
    inline hid_t locationID () const {
      return location_p;
    }
    
    //! Get the name of the location (within a dataset)
    std::string locationName ();

    //! Get the names of the attributes attached to the structure
    inline std::set<std::string> attributes () const {
      return attributes_p;
    }

    //! Get the number of attributes attached to the structure
    inline unsigned int nofAttributes () const {
      return attributes_p.size();
    }
    //! Is an attribute of given name attached to the structure?
    inline bool haveAttribute (std::string const &name) const {
      return static_cast<bool>(attributes_p.count(name));
    }
    //! Get the attribute name by index
    std::string attribute (unsigned int const &index);
    //! Add an attribute to the interally cached list
    bool addAttribute (std::string const &name);
    //! Add attributes to the interally kept set
    bool addAttributes (std::set<std::string> const &names);
    //! Remove an attribute from the interally cached list
    bool removeAttribute (std::string const &name);
    //! Remove attributes from the interally kept set
    bool removeAttributes (std::set<std::string> const &names);
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, CommonInterface.
    */
    inline std::string className () const {
      return "CommonInterface";
    }
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

    // === Methods ==============================================================

    //! Verify the ID so it can be passed into an H5I C function.
    bool hasValidID ();

    //! Verify a given ID is a valid id so it can be passed into an H5I C function.
    static bool hasValidID (hid_t const &object_id);
    
    //! Get the type (File, Group, Dataset, etc.) of the object
    H5I_type_t objectType ();

    //! Get the type (File, Group, Dataset, etc.) of an object
    static H5I_type_t objectType (hid_t const &object_id);
    
    //! Get the name of the object
    std::string objectName ();
    
    //! Open a structure (file, group, dataset, etc.)
    bool open (hid_t const &location);
    
    /*!
      \brief Open a structure (file, group, dataset, etc.)
      
      Though this signature it rather generatic, there is at least one case,
      where not all of the input parameters can be utilized: when opening a file
      (e.g. with BF_Dataset) the <tt>location</tt> will not be evaluated.
      
      \param location -- Identifier of the location to which the to be opened
             structure is attached
      \param name   -- Name of the structure (file, group, dataset, etc.) to be
             opened.
      \param create -- Create the corresponding data structure, if it does not 
             exist yet?
      
      \return status -- Status of the operation; returns <tt>false</tt> in case
              an error was encountered.
    */
    virtual bool open (hid_t const &location,
		       std::string const &name,
		       bool const &create) = 0;

    /*!
      \brief Get the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool getAttribute (std::string const &name,
				T &val)
      {
	/* Check if connected to a file */
	if (location_p > 0) {
	  /* Check if the attribute name is valid */
	  if (haveAttribute(name)) {
	    /* Forward the function call to perform the actual retrieval */
	    return DAL::h5get_attribute(location_p,
					name,
					val);
	  } else {
	    std::cerr << "[CommonInterface::getAttribute]"
		      << " Invalid attribute name " << name
		      << std::endl;
	    return false;
	  }
	} else {
	  std::cerr << "[CommonInterface::getAttribute]"
		    << " No connection to dataset or file!"
		    << std::endl;
	  return false;
	}
      }
    
    /*!
      \brief Get the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool getAttribute (std::string const &name,
				std::vector<T> &val)
      {
	/* Check if connected to a file */
	if (location_p > 0) {
	  /* Check if the attribute name is valid */
	  if (haveAttribute(name)) {
	    /* Forward the function call to perform the actual retrieval */
	    return DAL::h5get_attribute(location_p,
					name,
					val);
	  } else {
	    std::cerr << "[CommonInterface::getAttribute]"
		      << " Invalid attribute name " << name
		      << std::endl;
	    return false;
	  }
	} else {
	  std::cerr << "[CommonInterface::getAttribute]"
		    << " No connection to dataset or file!"
		    << std::endl;
	  return false;
	}
      }

#ifdef HAVE_CASA
    /*!
      \brief Get the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool getAttribute (std::string const &name,
				casa::Vector<T> &val)
      {
	/* Check if connected to a file */
	if (location_p > 0) {
	  /* Check if the attribute name is valid */
	  if (haveAttribute(name)) {
	    /* Forward the function call to perform the actual retrieval */
	    return DAL::h5get_attribute(location_p,
					name,
					val);
	  } else {
	    std::cerr << "[CommonInterface::getAttribute]"
		      << " Invalid attribute name " << name
		      << std::endl;
	    return false;
	  }
	} else {
	  std::cerr << "[CommonInterface::getAttribute]"
		    << " No connection to dataset or file!"
		    << std::endl;
	  return false;
	}
      }
#endif
    
    /*!
      \brief Set the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool setAttribute (std::string const &name,
				T const &val)
      {
	/* Check if connected to a file */
	if (location_p > 0) {
	  /* Check if the attribute name is valid */
	  if (haveAttribute(name)) {
	    /* Forward the function call to perform the actual write */
	    return h5set_attribute (location_p,
				    name,
				    val);
	  } else {
	    std::cerr << "[CommonInterface::setAttribute]"
		      << " Invalid attribute name " << name
		      << std::endl;
	    return false;
	  }
	} else {
	  std::cerr << "[CommonInterface::setAttribute]"
		    << " No connection to dataset or file!"
		    << std::endl;
	  return false;
	}
      }
    
    /*!
      \brief Set the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool setAttribute (std::string const &name,
				std::vector<T> const &val)
      {
	/* Check if connected to a file */
	if (location_p > 0) {
	  /* Check if the attribute name is valid */
	  if (haveAttribute(name)) {
	    /* Forward the function call to perform the actual write */
	    return h5set_attribute (location_p,
				    name,
				    val);
	  } else {
	    std::cerr << "[CommonInterface::setAttribute]"
		      << " Invalid attribute name " << name
		      << std::endl;
	    return false;
	  }
	} else {
	  std::cerr << "[CommonInterface::setAttribute]"
		    << " No connection to dataset or file!"
		    << std::endl;
	  return false;
	}
      }

#ifdef HAVE_CASA
    /*!
      \brief Set the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool setAttribute (std::string const &name,
				casa::Vector<T> const &val)
      {
	/* Check if connected to a file */
	if (location_p > 0) {
	  /* Check if the attribute name is valid */
	  if (haveAttribute(name)) {
	    /* Forward the function call to perform the actual write */
	    return h5set_attribute (location_p,
				    name,
				    val);
	  } else {
	    std::cerr << "[CommonInterface::setAttribute]"
		      << " Invalid attribute name " << name
		      << std::endl;
	    return false;
	  }
	} else {
	  std::cerr << "[CommonInterface::setAttribute]"
		    << " No connection to dataset or file!"
		    << std::endl;
	  return false;
	}
      }
#endif

    // === Private methods ======================================================

  private:

    //! Increment the reference count for a HDF5 object
    void incrementRefCount ();

    //! Unconditional copying
    void copy (CommonInterface const &other);
    
    //! Unconditional deletion 
    void destroy(void);

  }; // Class CommonInterface -- end
  
} // Namespace DAL -- end

#endif /* COMMONINTERFACE_H */
  
