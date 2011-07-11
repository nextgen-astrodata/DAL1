/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#ifndef DALOBJECTBASE_H
#define DALOBJECTBASE_H

// Standard library header files
#include <iostream>
#include <string>

#include <core/dalFileType.h>
#include <core/IO_Mode.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class dalObjectBase
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class DAL::dalObjectBase
    
    \author Lars B&auml;hren

    \date 2011-07-11

    \test tdalObjectBase.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class dalObjectBase {

  protected:

    //! Object handler, used with underlying low-level libraries
    void * itsObjectHandler;
    //! File type: CASA_MS, HDF5, FITS, etc.
    dalFileType itsFiletype;
    //! I/O mode flags
    IO_Mode itsFlags;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    dalObjectBase ();
    
    //! Copy constructor
    dalObjectBase (dalObjectBase const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~dalObjectBase ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    dalObjectBase& operator= (dalObjectBase const &other); 
    
    // === Parameter access =====================================================

    inline DAL::dalFileType::Type filetype () {
      return itsFiletype.type();
    }
    
    inline std::string filetypeName () {
      return itsFiletype.name();
    }
    
    //! Get object I/O mode flags
    inline int flags () const {
      return itsFlags.flags();
    }

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, dalObjectBase.
    */
    inline std::string className () const {
      return "dalObjectBase";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    virtual bool open (std::string const &name,
		       IO_Mode const &flags=IO_Mode(IO_Mode::Open)) = 0;
    
    // === Static methods =======================================================
    
    
    
  private:
    
    //! Unconditional copying
    void copy (dalObjectBase const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class dalObjectBase -- end
  
} // Namespace DAL -- end

#endif /* DALOBJECTBASE_H */
  
