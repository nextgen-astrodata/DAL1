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

#include "CommonAttributesProject.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                      CommonAttributesProject
  
  CommonAttributesProject::CommonAttributesProject ()
  {
    itsProjectID      = "UNDEFINED";
    itsProjectTitle   = "UNDEFINED";
    itsProjectPI      = "UNDEFINED";
    itsProjectCoI     = "UNDEFINED";
    itsProjectContact = "UNDEFINED";
  }

  //_____________________________________________________________________________
  //                                                      CommonAttributesProject
  
  /*!
    \param projectID      -- Unique identifier for the project.
    \param projectTitle   -- Name of the project.
    \param projectPI      -- Name of the project's principal investigator.
    \param projectCoI     -- Name(s) of the project's co-PI(s).
    \param projectContact -- Names/Email-addresses of the project's primary
    contact person(s).
  */
  CommonAttributesProject::CommonAttributesProject (std::string const &projectID,
						    std::string const &projectTitle,
						    std::string const &projectPI,
						    std::string const &projectCoI,
						    std::string const &projectContact)
  {
    itsProjectID      = projectID;
    itsProjectTitle   = projectTitle;
    itsProjectPI      = projectPI;
    itsProjectCoI     = projectCoI;
    itsProjectContact = projectContact;
  }
  
  //_____________________________________________________________________________
  //                                                      CommonAttributesProject
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  CommonAttributesProject::CommonAttributesProject (CommonAttributesProject const &other)
    : AttributesInterface (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  CommonAttributesProject::~CommonAttributesProject ()
  {
    destroy();
  }
  
  void CommonAttributesProject::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another CommonAttributesProject object from which to make a copy.
  */
  CommonAttributesProject& CommonAttributesProject::operator= (CommonAttributesProject const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void CommonAttributesProject::copy (CommonAttributesProject const &other)
  {
    itsProjectID      = other.itsProjectID;
    itsProjectTitle   = other.itsProjectTitle;
    itsProjectPI      = other.itsProjectPI;
    itsProjectCoI     = other.itsProjectCoI;
    itsProjectContact = other.itsProjectContact;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void CommonAttributesProject::setAttributes ()
  {
    itsAttributes.clear();
    
    itsAttributes.insert("PROJECT_ID");
    itsAttributes.insert("PROJECT_TITLE");
    itsAttributes.insert("PROJECT_PI");
    itsAttributes.insert("PROJECT_CO_I");
    itsAttributes.insert("PROJECT_CONTACT");
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void CommonAttributesProject::summary (std::ostream &os)
  {
    os << "[CommonAttributesProject] Summary of internal parameters." << std::endl;
    os << "-- Project ID      = " << itsProjectID      << std::endl;
    os << "-- Project title   = " << itsProjectTitle   << std::endl;
    os << "-- Project PI      = " << itsProjectPI      << std::endl;
    os << "-- Project CoI     = " << itsProjectCoI     << std::endl;
    os << "-- Project contact = " << itsProjectContact << std::endl;
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
#ifdef DAL_WITH_HDF5
  
  bool CommonAttributesProject::h5write (hid_t const &groupID)
  {
    bool status (true);
    
    HDF5Attribute::setAttribute (groupID, "PROJECT_ID",      itsProjectID );
    HDF5Attribute::setAttribute (groupID, "PROJECT_TITLE",   itsProjectTitle );
    HDF5Attribute::setAttribute (groupID, "PROJECT_PI",      itsProjectPI );
    HDF5Attribute::setAttribute (groupID, "PROJECT_CO_I",    itsProjectCoI );
    HDF5Attribute::setAttribute (groupID, "PROJECT_CONTACT", itsProjectContact );
    
    return status;
  }
  
  bool CommonAttributesProject::h5read (hid_t const &groupID)
  {
    bool status (true);
    
    DAL::h5get_attribute( groupID, "PROJECT_ID",       itsProjectID );
    DAL::h5get_attribute( groupID, "PROJECT_TITLE",    itsProjectTitle );
    DAL::h5get_attribute( groupID, "PROJECT_PI",       itsProjectPI );
    DAL::h5get_attribute( groupID, "PROJECT_CO_I",     itsProjectCoI );
    DAL::h5get_attribute( groupID, "PROJECT_CONTACT",  itsProjectContact );
    
    return status;
  }
  
#endif
  
} // Namespace DAL -- end
