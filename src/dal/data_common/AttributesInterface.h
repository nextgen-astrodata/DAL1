/***************************************************************************
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

#ifndef ATTRIBUTESINTERFACE_H
#define ATTRIBUTESINTERFACE_H

#include <core/dalCommon.h>
#include <core/HDF5Attribute.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class AttributesInterface
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Definition of common interface for classes storing sets of attributes
    
    \author Lars B&auml;hren

    \date 2010/12/03

    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class AttributesInterface {
    
  protected:

    //! Attributes names
    std::set<std::string> itsAttributes;
    
    //! Set up the list of attributes attached to the structure
    virtual void setAttributes () = 0;
    
  public:
    
    // === Construction / Destruction ===========================================

    // Default constructor
    AttributesInterface () {};
    
    //! Copy constructor
    AttributesInterface (AttributesInterface const &other) {
      copy (other);
    }
    
    //! Destructor
    virtual ~AttributesInterface ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    AttributesInterface& operator= (AttributesInterface const &other); 
    
    // === Parameter access =====================================================

    //! Attribute names
    inline std::set<std::string> attributes () const {
      return itsAttributes;
    }

    //! Is attribute of given \e name part of the group?
    inline bool haveAttribute (std::string const &name) const {
      return static_cast<bool>(itsAttributes.count(name));
    }
    
    // === Methods ==============================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, AttributesInterface.
    */
    virtual std::string className () const = 0;

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    virtual void summary (std::ostream &os) = 0;

#ifdef DAL_WITH_HDF5
    //! Write the attributes to a HDF5 file
    virtual bool h5write (hid_t const &groupID) = 0;
    
    //! Read the attributes from a HDF5 file
    virtual bool h5read (hid_t const &groupID) = 0;
#endif    

  private:
    
    //! Unconditional copying
    void copy (AttributesInterface const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class AttributesInterface -- end
  
} // Namespace DAL -- end

#endif /* ATTRIBUTESINTERFACE_H */
  
