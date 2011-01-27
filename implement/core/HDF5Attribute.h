/***************************************************************************
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

#ifndef HDF5ATTRIBUTE_H
#define HDF5ATTRIBUTE_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include "HDF5Object.h"

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Attribute
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class HDF5Attribute
    
    \author Lars B&auml;hren

    \date 2011/01/27

    \test tHDF5Attribute.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Attribute : public HDF5Object {

    //! Datatype identifier
    hid_t itsDatatype;
    //! Dataspace identifier
    hid_t itsDataspace;
    //! Rank of the dataspace
    int   itsRank;
    //! Error status
    herr_t itsStatus;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Attribute ();
    
    //! Copy constructor
    HDF5Attribute (HDF5Attribute const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Attribute ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Attribute& operator= (HDF5Attribute const &other); 
    
    // === Parameter access =====================================================
    
    //! Get the datatype identifier
    inline hid_t datatype () const {
      return itsDatatype;
    }

    //! Get the dataspace identifier
    inline hid_t dataspace () const {
      return itsDataspace;
    }

    //! Get the rank of the dataspace
    inline int rank () const {
      return itsRank;
    }

    //! Get error status of the object
    inline herr_t status () const {
      return itsStatus;
    }

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5Attribute.
    */
    inline std::string className () const {
      return "HDF5Attribute";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    bool open (hid_t const &location,
	       std::string const &name,
	       hid_t const &access=H5P_DEFAULT);

    //! Returns the datatype class identifier.
    inline H5T_class_t datatypeClass () {
      return HDF5Object::datatypeClass(itsLocation);
    }
    
    // === Static methods =======================================================
    
    /*!
      \brief Set attribute
      \param location
      \param name
      \param data    -- Data value(s) to be assigned to the attribute
      \param size    -- nof. element in the data array.
      \return status -- Status of the operation
    */
    template <class T>
      static bool setAttribute (hid_t const &location,
				std::string const &name,
				T const *data,
				unsigned int const &size,
				hid_t const &datatype)
      {
	bool status = true;
	
	cout << "[HDF5Attribute::setAttribute]" << endl;
	cout << "-- Location   = " << location << endl;
	cout << "-- Name       = " << name     << endl;
	cout << "-- Data size  = " << size     << endl;
	cout << "-- Data type  = " << datatype << endl;
	cout << "-- Data array = [";
	for (unsigned int n=0; n<size; ++n) {
	  cout << " " << data[n];
	}
	cout << " ]" << endl;
	
	return status;
      }
    
    /*!
      \brief Set attribute
      \param location
      \param name
      \param data    -- Data value(s) to be assigned to the attribute
      \param size    -- nof. element in the data array.
      \return status -- Status of the operation
    */
    template <class T>
      static bool setAttribute (hid_t const &location,
				std::string const &name,
				T const *data,
				unsigned int const &size);
    
    /*!
      \brief Set attribute
      \param location
      \param name
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      static bool setAttribute (hid_t const &location,
			 std::string const &name,
			 std::vector<T> const &data)
      {
	return setAttribute (location, name, &data[0], data.size());
      }
    
    /*!
      \brief Set attribute
      \param location
      \param name
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      static bool setAttribute (hid_t const &location,
				std::string const &name,
				T const &data)
      {
	return setAttribute (location, name, &data, 1);
      }
    
  private:
    
    //! Initialize internal parameters to default values
    void init ();

    //! Unconditional copying
    void copy (HDF5Attribute const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Attribute -- end
  
} // Namespace DAL -- end

#endif /* HDF5ATTRIBUTE_H */
  
