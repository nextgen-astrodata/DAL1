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

#ifndef SKY_IMAGEDATASET_H
#define SKY_IMAGEDATASET_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5DatasetBase.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Sky_ImageDataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class Sky_ImageDataset
    
    \author Lars B&auml;hren

    \date 2011/02/01

    \test tSky_ImageDataset.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_004
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class Sky_ImageDataset : public HDF5DatasetBase {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    Sky_ImageDataset ();
    
    //! Argumented constructor to open existing dataset
    Sky_ImageDataset (hid_t const &location,
		     std::string const &name,
		     IO_Mode const &flags=IO_Mode());
    
    //! Argumented constructor to create new dataset
    Sky_ImageDataset (hid_t const &location,
		      std::string const &name,
		      std::vector< hsize_t > const &shape,
		      hid_t const &datatype=H5T_NATIVE_DOUBLE,
		      IO_Mode const &flags=IO_Mode(IO_Mode::CreateNew));
    
    //! Copy constructor
    Sky_ImageDataset (Sky_ImageDataset const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~Sky_ImageDataset ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    Sky_ImageDataset& operator= (Sky_ImageDataset const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, Sky_ImageDataset.
    */
    inline std::string className () const {
      return "Sky_ImageDataset";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    
    
  private:

    //! Set up and initialize the list of attributes
    void setAttributes ();
    
    //! Unconditional copying
    void copy (Sky_ImageDataset const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Sky_ImageDataset -- end
  
} // Namespace DAL -- end

#endif /* SKY_IMAGEDATASET_H */
  
