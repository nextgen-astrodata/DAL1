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

#include "Sky_ImageGroup.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                               Sky_ImageGroup

  Sky_ImageGroup::Sky_ImageGroup ()
    : HDF5GroupBase()
  {;}
  
  //_____________________________________________________________________________
  //                                                               Sky_ImageGroup

  /*!
    \param location -- Identifier of the object the image group is attached to.
    \param name     -- Name of the beam group to be opened.
  */
  Sky_ImageGroup::Sky_ImageGroup (hid_t const &location,
				  std::string const &name)
  {
    open (location,name,IO_Mode(IO_Mode::Open));
  }
  
  //_____________________________________________________________________________
  //                                                               Sky_ImageGroup
  
  /*!
    \param location -- Identifier of the object the beam group is attached to.
    \param index    -- Index of the image group.
    \param flags    -- I/O mode flags.
  */
  Sky_ImageGroup::Sky_ImageGroup (hid_t const &location,
				  unsigned int const &index,
				  IO_Mode const &flags)
  {
    std::string name = getName(index);
    open (location,name,flags);
  }
  
  //_____________________________________________________________________________
  //                                                               Sky_ImageGroup
  
  /*!
    \param other -- Another Sky_ImageGroup object from which to create this new
           one.
  */
  Sky_ImageGroup::Sky_ImageGroup (Sky_ImageGroup const &other)
    : HDF5GroupBase (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Sky_ImageGroup::~Sky_ImageGroup ()
  {
    destroy();
  }
  
  void Sky_ImageGroup::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another Sky_ImageGroup object from which to make a copy.
  */
  Sky_ImageGroup& Sky_ImageGroup::operator= (Sky_ImageGroup const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Another Sky_ImageGroup object from which to make a copy.
  */
  void Sky_ImageGroup::copy (Sky_ImageGroup const &other)
  {
    itsCoordinates = other.itsCoordinates;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void Sky_ImageGroup::summary (std::ostream &os)
  {
    os << "[Sky_ImageGroup] Summary of internal parameters." << std::endl;
    os << "-- Location ID     = " << location_p                   << std::endl;
    os << "-- Group name      = " << HDF5Object::name(location_p) << std::endl;
    os << "-- nof. attributes = " << attributes_p.size()          << std::endl;
    os << "-- Attributes      = " << attributes_p                 << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           image group is being attached.
    \param name   -- Name of the group to be opened.
    \param flags  -- I/O mode flags.
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool Sky_ImageGroup::open (hid_t const &location,
			     std::string const &name,
			     IO_Mode const &flags)
  {
    bool status    = true;
    bool truncated = HDF5GroupBase::open (location_p,
					  location,
					  name,
					  flags);
    
    if ( H5Iis_valid(location_p) ) {

      // List of recognized attributes _____________________
      setAttributes();
      
      if (truncated) {

	// Initial values for the attributes _______________
	
	double valDouble       = 0.0;
	std::string grouptype  = "Image";
	std::string undefined  = "UNDEFINED";
	std::vector<std::string> vectString (1,undefined);
	
	// Write attribute values __________________________
	
	HDF5Attribute::write (location_p,"GROUPTYPE",                 grouptype );
	HDF5Attribute::write (location_p,"REFERENCE_FREQUENCY_VALUE", valDouble );
	HDF5Attribute::write (location_p,"REFERENCE_FREQUENCY_UNIT",  undefined );
	HDF5Attribute::write (location_p,"REFERENCE_BANDWIDTH_VALUE", valDouble );
	HDF5Attribute::write (location_p,"REFERENCE_BANDWIDTH_UNIT",  undefined );
	HDF5Attribute::write (location_p,"EFFECTIVE_FREQUENCY_VALUE", valDouble );
	HDF5Attribute::write (location_p,"EFFECTIVE_FREQUENCY_UNIT",  undefined );
	HDF5Attribute::write (location_p,"EFFECTIVE_BANDWIDTH_VALUE", valDouble );
	HDF5Attribute::write (location_p,"EFFECTIVE_BANDWIDTH_UNIT",  undefined );
      }
      
      // Open embedded groups ______________________________
      
      status = openEmbedded (flags);
      
    } else {
      std::cerr << "[Sky_ImageGroup::open]"
		<< " Failed to open/create group " << name
		<< std::endl;
      return false;
    }
    
    
    return status;
  }
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the beam.

    \return name -- The name of the beam group, <tt>Beam<index></tt>
  */
  std::string Sky_ImageGroup::getName (unsigned int const &index)
  {
    char uid[10];
    sprintf (uid, "%03d", index);
    
    std::string name (uid);
    
    name = "IMAGE_" + name;
    
    return name;
  }
  
  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void Sky_ImageGroup::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("REFERENCE_FREQUENCY_VALUE");
    attributes_p.insert("REFERENCE_FREQUENCY_UNIT");
    attributes_p.insert("REFERENCE_BANDWIDTH_VALUE");
    attributes_p.insert("REFERENCE_BANDWIDTH_UNIT");
    attributes_p.insert("EFFECTIVE_FREQUENCY_VALUE");
    attributes_p.insert("EFFECTIVE_FREQUENCY_UNIT");
    attributes_p.insert("EFFECTIVE_BANDWIDTH_VALUE");
    attributes_p.insert("EFFECTIVE_BANDWIDTH_UNIT");
  }

  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \param flags   -- I/O mode flags.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool Sky_ImageGroup::openEmbedded (IO_Mode const &flags)
  {
    bool status = true;

    /* Check HDF5 identifier */
    if (H5Iis_valid(location_p)) {

      /* Check for coordinates group */
      if ( H5Lexists (location_p, "COORDINATES", H5P_DEFAULT) ) {
	  itsCoordinates = CoordinatesGroup (location_p, "COORDINATES");
      }

      /* Check for image dataset */
      if (H5Lexists (location_p, "DATA", H5P_DEFAULT)) {
	  itsDataset.open(location_p, "DATA", flags);
      }

      /* Check for source table */
      if (H5Lexists (location_p, "SOURCE_TABLE", H5P_DEFAULT)) {
	  status = true;
      }

      return status;
    } else {
      return false;
    }
  }

} // Namespace DAL -- end
