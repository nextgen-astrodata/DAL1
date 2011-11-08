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

#include <core/dalCommon.h>
#include <core/dalFileType.h>
#include <core/IO_Mode.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class dalObjectBase
    
    \ingroup DAL
    \ingroup core
    
    \brief Basic interface and internal properties common to DAL classes.
    
    \author Lars B&auml;hren

    \date 2011-07-11

    \test tdalObjectBase.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li><a href="http://code.google.com/p/casacore">casacore library</a>
      <li><a href="http://heasarc.gsfc.nasa.gov/fitsio">CFITSIO library</a>
      <li><a href="http://www.hdfgroup.org/HDF5">HDF5 library</a>
    </ul>
    
    <h3>Synopsis</h3>

    There are a number of properties shared (or even required) by all core DAL
    classes providing a common abstraction layer on top of the underlying
    low-level libraries (casacore, CFITSIO, HDF5).
    
    <h3>Example(s)</h3>
    
  */  
  class dalObjectBase {

  protected:

    //! Name of the object (File, Dataset, Table, etc.)
    std::string itsName;
    //! File type: CASA_MS, HDF5, FITS, etc.
    dalFileType itsFiletype;
    //! I/O mode flags
    IO_Mode itsFlags;
    //! Object handler, used with underlying low-level libraries
    void * itsObjectHandler;
    
    //! Unconditional copying
    void copy (dalObjectBase const &other);
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    dalObjectBase (dalFileType const &filetype=dalFileType());
    
    //! Default constructor
    dalObjectBase (std::string const &name,
		   dalFileType const &filetype=dalFileType(),
		   IO_Mode const &flags=IO_Mode());
    
    //! Copy constructor
    dalObjectBase (dalObjectBase const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    virtual ~dalObjectBase ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    dalObjectBase& operator= (dalObjectBase const &other); 
    
    // === Parameter access =====================================================

    /*!
      \brief Get the name of the object (file, dataset, table, etc.)
      \return name -- Name of the object (file, dataset, table, etc.)
    */
    virtual std::string name () const {
      return itsName;
    }
    
    /*!
      \brief Set the name of the object (file, dataset, table, etc.)
      \param name    -- Name of the object (file, dataset, table, etc.)
      \return status -- Status of the operation.
    */
    virtual bool setName (std::string const &name) {
      itsName = name;
      return true;
    }

    /*!
      \brief Get the file type: CASA_MS, HDF5, FITS, etc.
      \return type -- File type: CASA_MS, HDF5, FITS, etc.
    */
    virtual DAL::dalFileType::Type filetype () {
      return itsFiletype.type();
    }
    
    /*!
      \brief Get the file type: CASA_MS, HDF5, FITS, etc.
      \return name -- File type name: CASA_MS, HDF5, FITS, etc.
    */
    virtual std::string filetypeName () {
      return itsFiletype.name();
    }

    //! Set file type: CASA_MS, HDF5, FITS, etc.
    virtual bool setFiletype (dalFileType const &filetype) {
      itsFiletype = filetype;
      return true;
    }
    
    //! Set file type: CASA_MS, HDF5, FITS, etc.
    virtual bool setFiletype (std::string const &type) {
      itsFiletype = dalFileType(type);
      return true;
    }
  
    /*!
      \brief Get object I/O mode flags
      \return flags -- Object I/O mode flags.
    */
    inline int flags () const {
      return itsFlags.flags();
    }
    
    //! Get the object handler, used with underlying low-level libraries
    inline void * objectHandler () {
      return itsObjectHandler;
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
    virtual void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    /* virtual bool open (std::string const &name, */
    /* 		       IO_Mode const &flags=IO_Mode(IO_Mode::Open)) = 0; */
    
    // === Static methods =======================================================
    
    
    
  private:
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class dalObjectBase -- end
  
} // Namespace DAL -- end

#endif /* DALOBJECTBASE_H */
  
