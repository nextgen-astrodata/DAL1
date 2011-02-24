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

#ifndef HDF5DATATYPE_H
#define HDF5DATATYPE_H

#include <core/HDF5Object.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Datatype
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class HDF5Datatype
    
    \author Lars B&auml;hren

    \date 2011/01/31

    \test tHDF5Datatype.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Datatype : public HDF5Object {

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Datatype ();
    
    //! Copy constructor
    HDF5Datatype (HDF5Datatype const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Datatype ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Datatype& operator= (HDF5Datatype const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5Datatype.
    */
    inline std::string className () const {
      return "HDF5Datatype";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    
    
    // === Static methods =======================================================
    
    //! Get map of datatypes and their corresponding name
    static std::map<hid_t,std::string> datatypesMap ();

    //! Get list of available datatypes
    static std::vector<hid_t> datatypes ();

    //! Get list of names corresponding to datatypes
    static std::vector<std::string> datatypeNames ();

    //! Get name for the datatype
    static hid_t datatype (hid_t const &id);
    
    //! Get name for the datatype
    static std::string datatypeName (hid_t const &id);
    
  private:
    
    //! Unconditional copying
    void copy (HDF5Datatype const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Datatype -- end
  
} // Namespace DAL -- end

#endif /* HDF5DATATYPE_H */
  
