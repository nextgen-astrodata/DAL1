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

#ifndef HDF5DATASPACE_H
#define HDF5DATASPACE_H

// Standard library header files
#include <iostream>
#include <string>

#include "HDF5Object.h"

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Dataspace
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class HDF5Dataspace
    
    \author Lars B&auml;hren

    \date 2011/02/28

    \test tHDF5Dataspace.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Dataspace : public HDF5Object {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Dataspace ();

    //! Identifier of the object to which the dataspace is attached
    HDF5Dataspace (hid_t const &location);

    //! Copy constructor
    HDF5Dataspace (HDF5Dataspace const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Dataspace ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Dataspace& operator= (HDF5Dataspace const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5Dataspace.
    */
    inline std::string className () const {
      return "HDF5Dataspace";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Open dataspace (either directly or from host object)
    bool open (hid_t const &location);

    //! Determines whether a dataspace is a simple dataspace.
    inline htri_t isSimple () {
      return isSimple (itsLocation);
    }
    
    // === Static methods =======================================================
    
    //! Determines whether a dataspace is a simple dataspace.
    static htri_t isSimple (hid_t const location);

    //! Retrieves dataspace dimension size.
    static bool shape (hid_t const &location,
		       std::vector<hsize_t> &dimensions,
		       bool const &getMaximum=false);
    
    //! Retrieves dataspace dimension size and maximum size.
    static bool shape (hid_t const &location,
		       std::vector<hsize_t> &dimensions,
		       std::vector<hsize_t> &maxDimensions);
    
  private:
    
    //! Unconditional copying
    void copy (HDF5Dataspace const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // END -- class HDF5Dataspace
  
} // END -- namespace DAL

#endif /* HDF5DATASPACE_H */
  
