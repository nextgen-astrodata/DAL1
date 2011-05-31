/***************************************************************************
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

#ifndef SKY_ROOTGROUP_H
#define SKY_ROOTGROUP_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5GroupBase.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Sky_RootGroup
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Root group of a Radio Sky Image Cube
    
    \author Lars B&auml;hren

    \date 2010/10/19

    \test tSky_RootGroup.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICD: Radio Sky Image Cubes (LOFAR-USG-ICD-004)
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class Sky_RootGroup : public HDF5GroupBase {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    Sky_RootGroup ();
    
    //! Copy constructor
    Sky_RootGroup (Sky_RootGroup const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~Sky_RootGroup ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another Sky_RootGroup object from which to make a copy.
    */
    Sky_RootGroup& operator= (Sky_RootGroup const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, Sky_RootGroup.
    */
    inline std::string className () const {
      return "Sky_RootGroup";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));

  private:

    void setAttributes ()
    {
      attributes_p.clear();
    }
    
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate))
    {
      return flags.flags();
    }

    //! Unconditional copying
    void copy (Sky_RootGroup const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Sky_RootGroup -- end
  
} // Namespace DAL -- end

#endif /* SKY_ROOTGROUP_H */
  
