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

#ifndef FILEACCESSPROPERTIES_H
#define FILEACCESSPROPERTIES_H

#include <core/HDF5Object.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class FileAccessProperties
    
    \ingroup DAL
    \ingroup core
    
    \brief Maintain file access properties
    
    \author Lars B&auml;hren

    \date 2010/10/19

    \test tFileAccessProperties.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>

    Available file access property flags:

    <ul>
      <li>\b HDF5
      These are the bits that can be passed to the `flags' argument of
      H5Fcreate() and H5Fopen(). Use the bit-wise OR operator (|) to combine
      them as needed.  As a side effect, they call H5check_version() to make sure
      that the application is compiled with a version of the hdf5 header files
      which are compatible with the library to which the application is linked.
      We're assuming that these constants are used rather early in the hdf5
      session.
      \code
      #define H5F_ACC_RDONLY  (H5CHECK 0x0000u)       //  absence of rdwr => rd-only
      #define H5F_ACC_RDWR    (H5CHECK 0x0001u)       //  open for read and write
      #define H5F_ACC_TRUNC   (H5CHECK 0x0002u)       //  overwrite existing files
      #define H5F_ACC_EXCL    (H5CHECK 0x0004u)       //  fail if file already exists
      #define H5F_ACC_DEBUG   (H5CHECK 0x0008u)       //  print debug info
      #define H5F_ACC_CREAT   (H5CHECK 0x0010u)       //  create non-existing files
      \endcode
      
      <li>\b CFITSIO
      The iomode parameter defines the read/write access allowed in the file and can
      have values of READONLY (0) and READWRITE (1).
    </ul>
    
    <h3>Example(s)</h3>
    
  */  
  class FileAccessProperties {

    hid_t itsAccessProperties;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    FileAccessProperties ();
    
    //! Copy constructor
    FileAccessProperties (FileAccessProperties const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~FileAccessProperties ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another FileAccessProperties object from which to make a copy.
    */
    FileAccessProperties& operator= (FileAccessProperties const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, FileAccessProperties.
    */
    inline std::string className () const {
      return "FileAccessProperties";
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
    void copy (FileAccessProperties const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class FileAccessProperties -- end
  
} // Namespace DAL -- end

#endif /* FILEACCESSPROPERTIES_H */
  
