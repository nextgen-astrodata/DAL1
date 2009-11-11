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

    \todo Enable checking of attribute names before attempting to read/write.

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

    <h3 name="attributes">Working with the attached attributes</h3>

    The collection of attributes attached to a given structure is stored as
    a STL set:
    \code
    std::set<std::string> attributes_p;
    \endcode

  */  
  class CommonInterface {

  protected:

    //! HDF5 identifier for the location
    hid_t location_p;
    //! Names of the attributes attached to the structure
    std::set<std::string> attributes_p;
    //! Set up the list of attributes attached to the structure
    virtual void setAttributes () = 0;
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
		       bool const &create=true) = 0;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    CommonInterface ();
    
    // -------------------------------------------------------------- Destruction
    
    //! Destructor
    virtual ~CommonInterface ();
    
    // --------------------------------------------------------------- Parameters
    
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

    //! Set the names of the attributes attached to the structure
    bool setAttributes (std::set<std::string> const &attributes);

    //! Get the number of attributes attached to the structure
    inline unsigned int nofAttributes () const {
      return attributes_p.size();
    }
    //! Is an attribute of given name atached to the structure?
    inline bool haveAttribute (std::string const &name) const {
      return static_cast<bool>(attributes_p.count(name));
    }
    //! Get the attribute name by index
    std::string attribute (unsigned int const &index);
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

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods

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
	  /* Forward the function call to perform the actual retrieval */
	  return DAL::h5get_attribute(location_p,
				      name,
				      val);
	} else {
	  std::cerr << "[BF_Dataset::getAttribute]"
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
	  /* Forward the function call to perform the actual retrieval */
	  return DAL::h5get_attribute(location_p,
				      name,
				      val);
	} else {
	  std::cerr << "[BF_Dataset::getAttribute]"
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
				T const &val)
      {
	/* Check if connected to a file */
	if (location_p > 0) {
	  return setAttribute (location_p,
			       name,
			       val);
	} else {
	  std::cerr << "[BF_Dataset::setAttribute]"
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
	  return setAttribute (location_p,
			       name,
			       val);
	} else {
	  std::cerr << "[BF_Dataset::setAttribute]"
		    << " No connection to dataset or file!"
		    << std::endl;
	  return false;
	}
      }
    
  private:
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CommonInterface -- end
  
} // Namespace DAL -- end

#endif /* COMMONINTERFACE_H */
  
