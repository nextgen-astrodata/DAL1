/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 4027 2010-01-21 13:05:37Z baehren                    $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#ifndef DALFILETYPE_H
#define DALFILETYPE_H

// Standard library header files
#include <iostream>
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
      //! HDF5 file -- low-level access handled through HDF5 library
      HDF5,
      //! FITS file -- low-level access handled through CFITSIO library
      FITS,
      //! CASA MeasurementSet -- low-level access handled through casacore library
      MSCASA,
      //! Undefined type of file
      UNDEFINED
    };

  private:

    //! Type of the data file
    dalFileType::Type type_p;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    dalFileType ();
    
    //! Argumented constructor
    dalFileType (dalFileType::Type const &filetype);
    
    //! Argumented constructor
    dalFileType (std::string const &filetypeName);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another dalFileType object from which to create this new
             one.
    */
    dalFileType (dalFileType const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~dalFileType ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another dalFileType object from which to make a copy.
    */
    dalFileType& operator= (dalFileType const &other); 
    
    // === Parameter access =====================================================

    //! Get file type
    inline dalFileType::Type type () {
      return type_p;
    }

    //! Set the file type
    bool setType (dalFileType::Type const &filetype);

    //! Set the file type
    bool setType (std::string const &filetypeName);

    //! Get file type as name
    std::string name ();

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

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    //! Get file type
    static dalFileType::Type type (std::string const &fileTypeName);

    //! Convert type to name
    static std::string name (dalFileType::Type const &fileType);
    
  private:
    
    //! Unconditional copying
    void copy (dalFileType const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class dalFileType -- end
  
} // Namespace DAL -- end

#endif /* DALFILETYPE_H */
  
