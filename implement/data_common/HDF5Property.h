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

#ifndef HDF5PROPERTY_H
#define HDF5PROPERTY_H

// Standard library header files
#include <iostream>
#include <map>
#include <string>

#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Property
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Brief description for class HDF5Property
    
    \author Lars B&auml;hren

    \date 2010/06/21

    \test tHDF5Property.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Property {

    //! Link traversal file access flag
    std::map<hid_t,std::string> fileAccessFlag_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Property ();
    
    //! Copy constructor
    HDF5Property (HDF5Property const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Property ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another HDF5Property object from which to make a copy.
    */
    HDF5Property& operator= (HDF5Property const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, HDF5Property.
    */
    inline std::string className () const {
      return "HDF5Property";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    
    
  private:

    //! Initialize the object's internal parameters
    void init ();
    
    //! Unconditional copying
    void copy (HDF5Property const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Property -- end
  
} // Namespace DAL -- end

#endif /* HDF5PROPERTY_H */
  
