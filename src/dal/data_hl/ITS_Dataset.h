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

#ifndef ITS_DATASET_H
#define ITS_DATASET_H

/* Standard library header files */
#include <iostream>
#include <string>

/* DAL header files*/
#include <data_comon/HDF5DatasetBase.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class ITS_Dataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class ITS_Dataset
    
    \author Lars B&auml;hren

    \date 2011-05-29

    \test tITS_Dataset.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class ITS_Dataset : public HDF5DatasetBase {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    ITS_Dataset ();
    
    //! Copy constructor
    ITS_Dataset (ITS_Dataset const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~ITS_Dataset ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    ITS_Dataset& operator= (ITS_Dataset const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, ITS_Dataset.
    */
    inline std::string className () const {
      return "ITS_Dataset";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    
    
    // === Static methods =======================================================
    
    
    
  private:
    
    //! Unconditional copying
    void copy (ITS_Dataset const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class ITS_Dataset -- end
  
} // Namespace DAL -- end

#endif /* ITS_DATASET_H */
  
