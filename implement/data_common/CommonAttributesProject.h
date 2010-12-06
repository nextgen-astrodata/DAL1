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

#ifndef COMMONATTRIBUTESPROJECT_H
#define COMMONATTRIBUTESPROJECT_H

#include "AttributesInterface.h"

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CommonAttributesProject
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Common LOFAR attributes for description of project
    
    \author Lars B&auml;hren

    \date 2010/12/03

    \test tCommonAttributesProject.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class CommonAttributesProject : public AttributesInterface {

    //! Unique identifier for the project
    std::string itsProjectID;
    //! Name of the project
    std::string itsProjectTitle;
    //! Name of the project's principal investigator
    std::string itsProjectPI;
    //! Name(s) of the project's co-PI(s)
    std::string itsProjectCoI;
    //! Names/Email-addresses of the project's primary contact person(s)
    std::string itsProjectContact;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CommonAttributesProject ();
    
    //! Argumented constructor
    CommonAttributesProject (std::string const &projectID,
			     std::string const &projectTitle,
			     std::string const &projectPI,
			     std::string const &projectCoI,
			     std::string const &projectContact);
    
    //! Copy constructor
    CommonAttributesProject (CommonAttributesProject const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~CommonAttributesProject ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    CommonAttributesProject& operator= (CommonAttributesProject const &other); 
    
    // === Parameter access =====================================================

    //! Unique identifier for the project
    inline std::string projectID () const {
      return itsProjectID;
    }
    //! Set unique identifier for the project
    inline void setProjectID (std::string const &id) {
      itsProjectID = id;
    }

    //! Name of the project
    inline std::string projectTitle () const {
      return itsProjectTitle;
    }
    //! Set name of the project
    inline void setProjectTitle (std::string const &title) {
      itsProjectTitle = title;
    }

    //! Name of the project's principal investigator
    inline std::string projectPI () const {
      return itsProjectPI;
    }
    //! Set name of the project's principal investigator
    inline void setProjectPI (std::string const &projectPI) {
      itsProjectPI = projectPI;
    }

    //! Name(s) of the project's co-PI(s)
    inline std::string projectCoI () const {
      return itsProjectCoI;
    }
    //! Set name(s) of the project's co-PI(s)
    inline void setProjectCoI (std::string const &projectCoI) {
      itsProjectCoI = projectCoI;
    }

    //! Names/Email-addresses of the project's primary contact person(s)
    inline std::string projectContact () const {
      return itsProjectContact;
    }
    //! Names/Email-addresses of the project's primary contact person(s)
    inline void setProjectContact (std::string const &contact) {
      itsProjectContact = contact;
    }
    
    // === Methods ==============================================================

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, CommonAttributesProject.
    */
    inline std::string className () const {
      return "CommonAttributesProject";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

#ifdef HAVE_HDF5
    //! Write the attributes to a HDF5 file
    bool h5write (hid_t const &groupID);
    
    //! Read the attributes from a HDF5 file
    bool h5read (hid_t const &groupID);
#endif    
    
  private:

    //! Set the atttribute names
    void setAttributes ();
    
    //! Unconditional copying
    void copy (CommonAttributesProject const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CommonAttributesProject -- end
  
} // Namespace DAL -- end

#endif /* COMMONATTRIBUTESPROJECT_H */
  
