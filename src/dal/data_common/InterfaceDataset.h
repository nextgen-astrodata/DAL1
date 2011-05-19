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

#ifndef DATASETINTERFACE_H
#define DATASETINTERFACE_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <core/HDF5Dataset.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class InterfaceDataset
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Common interface for datasets using HDF5 for underlying storage
    
    \author Lars B&auml;hren

    \date 2011/02/01

    \test tInterfaceDataset.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_004
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class InterfaceDataset : public HDF5Dataset {

  protected:
    
    //! Set of attributes attached to the dataset
    std::set<std::string> itsAttributes;
    //! Group type descriptor
    std::string itsGroupType;
    //! Path to the coordinates group
    std::string itsWCSinfo;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    InterfaceDataset ();
    
    //! Argumented constructor to open existing dataset
    InterfaceDataset (hid_t const &location,
		      std::string const &name);
    
    //! Argumented constructor to open existing dataset
    InterfaceDataset (hid_t const &location,
		      unsigned int const &index);

    //! Argumented constructor to create new dataset
    InterfaceDataset (hid_t const &location,
		      std::string const &name,
		      std::vector< hsize_t > const &shape,
		      hid_t const &datatype=H5T_NATIVE_DOUBLE);
    
    //! Copy constructor
    InterfaceDataset (InterfaceDataset const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~InterfaceDataset ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    InterfaceDataset& operator= (InterfaceDataset const &other); 
    
    // === Parameter access =====================================================
    
    //! Attributes attached to the dataset
    inline std::set<std::string> attributes () const {
      return itsAttributes;
    }

    //! Get the group type descriptor
    inline std::string groupType () const {
      return itsGroupType;
    }

    //! Get the path to the coordinates group
    inline std::string wcsInfo () const {
      return itsWCSinfo;
    }
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, InterfaceDataset.
    */
    inline std::string className () const {
      return "InterfaceDataset";
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
    void copy (InterfaceDataset const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class InterfaceDataset -- end
  
} // Namespace DAL -- end

#endif /* DATASETINTERFACE_H */
  
