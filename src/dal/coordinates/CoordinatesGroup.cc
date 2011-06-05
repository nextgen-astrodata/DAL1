/***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include <coordinates/CoordinatesGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             CoordinatesGroup
  
  CoordinatesGroup::CoordinatesGroup ()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                             CoordinatesGroup
  
  /*!
    \param location -- HDF5 object identifier for the group from which the 
           object is being created.
  */
  CoordinatesGroup::CoordinatesGroup (hid_t const &location)
  {
    init ();
    h5read (location);
  }
    
  //_____________________________________________________________________________
  //                                                             CoordinatesGroup
  
  CoordinatesGroup::CoordinatesGroup (hid_t const &location,
				      std::string const &name)
  {
    init ();
    h5read (location, name);
  }

  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  CoordinatesGroup::~CoordinatesGroup ()
  {
    destroy();
  }
  
  //_____________________________________________________________________________
  //                                                                      destroy
  
  void CoordinatesGroup::destroy ()
  {
    itsAttributes.clear();
    itsRefLocationValue.clear();
    itsRefLocationUnits.clear();
  }
  
  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                            referenceLocation
  
  /*!
    \retval value  -- Reference location value.
    \retval units  -- Reference location units.
    \retval frame  -- Reference location frame identifier.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool CoordinatesGroup::referenceLocation (std::vector<double> &value,
					    std::vector<std::string> &units,
					    std::string &frame)
  {
    bool status (true);

    if (itsRefLocationValue.empty()) {
      status = false;
    } else {
      value.clear();
      value = itsRefLocationValue;
    }

    if (itsRefLocationUnits.empty()) {
      status = false;
    } else {
      units.clear();
      units = itsRefLocationUnits;
    }

    frame = itsRefLocationFrame;

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                         setReferenceLocation
  
  /*!
    \param value   -- Reference location value.
    \param units   -- Reference location units.
    \param frame   -- Reference location frame identifier.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool CoordinatesGroup::setReferenceLocation (std::vector<double> const &value,
					       std::vector<std::string> const &units,
					       std::string const &frame)
  {
    bool status (true);
    
    /* Basic check of the input parameters */
    if (value.empty() || units.empty()) {
      return false;
    }

    /* Store values */
    itsRefLocationValue.resize(value.size());
    itsRefLocationUnits.resize(units.size());

    itsRefLocationValue = value;
    itsRefLocationUnits = units;
    itsRefLocationFrame = frame;

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                             setReferenceTime
  
  /*!
    \retval value  -- Reference time value.
    \retval units  -- Reference time units.
    \retval frame  -- Reference time frame identifier.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool CoordinatesGroup::referenceTime (double &value,
					std::string &units,
					std::string &frame)
  {
    value = itsRefTimeValue;
    units = itsRefTimeUnits;
    frame = itsRefTimeFrame;

    return true;
  }
  
  //_____________________________________________________________________________
  //                                                             setReferenceTime
  
  /*!
    \param value   -- Reference time value.
    \param units   -- Reference time units.
    \param frame   -- Reference time frame identifier.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool CoordinatesGroup::setReferenceTime (double const &value,
					   std::string const &units,
					   std::string const &frame)
  {
    itsRefTimeValue = value;
    itsRefTimeUnits = units;
    itsRefTimeFrame = frame;

    return true;
  }

  //_____________________________________________________________________________
  //                                                                      nofAxes

  /*!
    \todo Implement body of function!
  */
  int CoordinatesGroup::nofAxes () 
  {
    int nelem (0);
    
    if (!itsCoordinates.empty()) {
      for (unsigned int n(0); n<itsCoordinates.size(); ++n) {
      }
    }
    
    return nelem;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void CoordinatesGroup::summary (std::ostream &os)
  {
    os << "[CoordinatesGroup] Summary of internal parameters." << std::endl;
    os << "-- Group type descriptor = " << itsGroupType          << std::endl;
    os << "-- Ref. location value   = " << itsRefLocationValue   << std::endl;
    os << "-- Ref. location unit    = " << itsRefLocationUnits   << std::endl;
    os << "-- Ref. location frame   = " << itsRefLocationFrame   << std::endl;
    os << "-- Ref. time value       = " << itsRefTimeValue       << std::endl;
    os << "-- Ref. time unit        = " << itsRefTimeUnits       << std::endl;
    os << "-- Ref. time frame       = " << itsRefTimeFrame       << std::endl;
    os << "-- nof. coordinates      = " << nofCoordinates()      << std::endl;
    os << "-- Coordinate types      = " << itsCoordinateTypes    << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  bool CoordinatesGroup::init ()
  {
    bool status (true);

    /*________________________________________________________________
      Set up the attributes atteched to the group
    */
    
    itsAttributes.clear();
    itsAttributes.insert("GROUPTYPE");
    itsAttributes.insert("REF_LOCATION_VALUE");
    itsAttributes.insert("REF_LOCATION_UNIT");
    itsAttributes.insert("REF_LOCATION_FRAME");
    itsAttributes.insert("REF_TIME_VALUE");
    itsAttributes.insert("REF_TIME_UNIT");
    itsAttributes.insert("REF_TIME_FRAME");
    itsAttributes.insert("NOF_COORDINATES");
    itsAttributes.insert("NOF_AXES");
    itsAttributes.insert("COORDINATE_TYPES");
    
    /*________________________________________________________________
      Initialize the values of the parameters/attributes
    */

    std::string undefined = "UNDEFINED";
    
    itsGroupType         = "CoordinatesGroup";
    itsRefLocationValue  = std::vector<double> (3, 0.0);
    itsRefLocationUnits  = std::vector<std::string> (3, undefined);
    itsRefLocationFrame  = undefined;
    itsRefTimeValue      = 0.0;
    itsRefTimeUnits      = "s";
    itsRefTimeFrame      = "UTC";
    itsNofAxes           = 0;
    itsCoordinateTypes   = std::vector<std::string> (1, undefined);

    return status;
  }

  //_____________________________________________________________________________
  //                                                                       h5read
  
  /*!
    \param location -- HDF5 object identifier of the group from which to read the
           coordinate information.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered, such e.g. that the expected attributes storing 
	    coordinate parameters are not found.
  */
  bool CoordinatesGroup::h5read (hid_t const &location)
  {
    bool status (true);

    if (!H5Iis_valid(location)) {
      std::cerr << "[CoordinatesGroup::h5read] Invalid object!" << std::endl;
      return false;
    }

    /*________________________________________________________________
      Check if the basic set of attributes exist at the given
      location; if this is not the case, attributes values cannot be
      read from the HDF5 file.
    */
    if (HDF5Object::objectType(location,"GROUPTYPE") == H5I_ATTR) {

      unsigned int ncoords;
      unsigned int naxes;

      status *= HDF5Attribute::write (location, "GROUPTYPE",          itsGroupType);
      status *= HDF5Attribute::write (location, "REF_LOCATION_VALUE", itsRefLocationValue);
      status *= HDF5Attribute::write (location, "REF_LOCATION_UNIT",  itsRefLocationUnits);
      status *= HDF5Attribute::write (location, "REF_LOCATION_FRAME", itsRefLocationFrame);
      status *= HDF5Attribute::write (location, "REF_TIME_VALUE",     itsRefTimeValue);
      status *= HDF5Attribute::write (location, "REF_TIME_UNIT",      itsRefTimeUnits);
      status *= HDF5Attribute::write (location, "REF_TIME_FRAME",     itsRefTimeFrame);
      status *= HDF5Attribute::write (location, "NOF_COORDINATES",    ncoords);
      status *= HDF5Attribute::write (location, "NOF_AXES",           naxes);
    } else {
      std::cerr << "[CoordinatesGroup::h5read] Unable to find attribute GROUPTYPE!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  /*!
    \param location -- HDF5 object identifier.
    \param name     -- Name of the HDF5 group storing the coordinates information.
  */
  bool CoordinatesGroup::h5read (hid_t const &location,
				 std::string const &name)
  {
    bool status (true);
    hid_t id = DAL::HDF5Object::open (location, name);

    if (id<0) {
      status = false;
    } else {
      status = h5read (id);
    }
    
    DAL::HDF5Object::close (id);
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  /*!
    \param location -- HDF5 object identifier.
  */
  bool CoordinatesGroup::h5write (hid_t const &location)
  {
    bool status         = true;
    unsigned int ncoord = itsCoordinates.size();

    status *= HDF5Attribute::write (location, "REF_LOCATION_VALUE", itsRefLocationValue );
    status *= HDF5Attribute::write (location, "REF_LOCATION_UNIT",  itsRefLocationUnits );
    status *= HDF5Attribute::write (location, "REF_LOCATION_FRAME", itsRefLocationFrame );
    status *= HDF5Attribute::write (location, "REF_TIME_VALUE",     itsRefTimeValue     );
    status *= HDF5Attribute::write (location, "REF_TIME_UNIT",      itsRefTimeUnits     );
    status *= HDF5Attribute::write (location, "REF_TIME_FRAME",     itsRefTimeFrame     );
    status *= HDF5Attribute::write (location, "NOF_COORDINATES",    ncoord              );
    status *= HDF5Attribute::write (location, "NOF_AXES",           itsNofAxes          );

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  bool CoordinatesGroup::h5write (hid_t const &location,
				  std::string const &name)
  {
    bool status (true);
    hid_t id = DAL::HDF5Object::open (location, name);

    if (id<0) {
      status = false;
    } else {
      status = h5write (id);
    }
    
    DAL::HDF5Object::close (id);
    
    return status;
  }
  
  
} // Namespace DAL -- end
