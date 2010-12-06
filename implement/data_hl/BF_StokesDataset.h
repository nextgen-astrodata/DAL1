/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

#ifndef BF_STOKESDATASET_H
#define BF_STOKESDATASET_H

// Standard library header files
#include <iostream>
#include <string>

#include <HDF5Dataset.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_StokesDataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class BF_StokesDataset
    
    \author Lars B&auml;hren

    \date 2010/12/05

    \test tBF_StokesDataset.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class BF_StokesDataset : public HDF5Dataset {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_StokesDataset ();
    
    //! Argumented constructor
    BF_StokesDataset (hid_t const &location,
		      std::string const &name);

    //! Argumented constructor
    BF_StokesDataset (hid_t const &location,
		      std::string const &name,
		      std::vector<hsize_t> const &shape,
		      hid_t const &datatype);
    
    //! Copy constructor
    BF_StokesDataset (BF_StokesDataset const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~BF_StokesDataset ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    BF_StokesDataset& operator= (BF_StokesDataset const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, BF_StokesDataset.
    */
    inline std::string className () const {
      return "BF_StokesDataset";
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
    void copy (BF_StokesDataset const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class BF_StokesDataset -- end
  
} // Namespace DAL -- end

#endif /* BF_STOKESDATASET_H */
  
