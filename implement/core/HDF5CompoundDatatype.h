/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars Baehren (bahren@astron.nl)                                       *
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

#ifndef HDF5COMPOUNDDATATYPE_H
#define HDF5COMPOUNDDATATYPE_H

// DAL header files
#include <core/HDF5Datatype.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5CompoundDatatype
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class HDF5CompoundDatatype
    
    \author Lars B&auml;hren

    \date 2010/12/04

    \test tHDF5CompoundDatatype.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5CompoundDatatype {

    HDF5Datatype itsDatatype;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5CompoundDatatype ();
    
    //! Copy constructor
    HDF5CompoundDatatype (HDF5CompoundDatatype const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5CompoundDatatype ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5CompoundDatatype& operator= (HDF5CompoundDatatype const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5CompoundDatatype.
    */
    inline std::string className () const {
      return "HDF5CompoundDatatype";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    
    
  private:
    
    //! Unconditional copying
    void copy (HDF5CompoundDatatype const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5CompoundDatatype -- end
  
} // Namespace DAL -- end

#endif /* HDF5COMPOUNDDATATYPE_H */
  
