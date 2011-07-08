/***************************************************************************
 *   Copyright (C) 2010-2011                                               *
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

#ifndef DALFILETYPE_H
#define DALFILETYPE_H

// Standard library header files
#include <iostream>
#include <map>
#include <string>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class dalFileType
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class dalFileType
    
    \author Lars B&auml;hren

    \date 2010/04/07

    \test tdalFileType.cc
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class dalFileType {
    
  public:
    
    //! Underlying file type
    enum Type {
      //! Undefined type of file
      UNDEFINED,
      //! HDF5 file -- low-level access handled through HDF5 library
      HDF5,
      //! FITS file -- low-level access handled through CFITSIO library
      FITS,
      //! CASA MeasurementSet -- low-level access handled through casacore library
      CASA_MS,
      //! CASA PagedImage -- low-level access handled through casacore library
      CASA_IMAGE
    };

  private:

    //! Type of the data file
    dalFileType::Type itsType;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    dalFileType ();
    //! Argumented constructor
    dalFileType (dalFileType::Type const &filetype);
    //! Argumented constructor
    dalFileType (std::string const &filetypeName);
    //! Copy constructor
    dalFileType (dalFileType const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~dalFileType ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    dalFileType& operator= (dalFileType const &other); 
    
    // === Parameter access =====================================================

    //! Get file type
    inline dalFileType::Type type () {
      return itsType;
    }

    //! Set the file type
    bool setType (dalFileType::Type const &filetype);

    //! Set the file type
    bool setType (std::string const &filetypeName);

    //! Get file type as name
    std::string name ();

    // === Public methods =======================================================

    //! Is the file of type FITS?
    bool isFITS ();
    //! Is the file of type HDF5?
    bool isHDF5 ();
    //! Is the file of type CASA?
    bool isCASA ();

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, dalFileType.
    */
    inline std::string className () const {
      return "dalFileType";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

    // === Static methods =======================================================

    //! Get map of types and their corresponding name
    static std::map<dalFileType::Type,std::string> getMap ();
    
    //! Get file type
    static dalFileType::Type getType (std::string const &name);

    //! Convert type to name
    static std::string getName (dalFileType::Type const &fileType);
    
  private:
    
    //! Unconditional copying
    void copy (dalFileType const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class dalFileType -- end
  
} // Namespace DAL -- end

#endif /* DALFILETYPE_H */
  
