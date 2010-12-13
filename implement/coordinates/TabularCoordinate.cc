/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include "TabularCoordinate.h"

namespace DAL {  // Namespace DAL -- begin

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  /*!
    \param axisNames -- World axis names.
    \param axisUnits -- World axis units.
  */
  template <class T>
  TabularCoordinate<T>::TabularCoordinate (std::string const &axisNames,
					   std::string const &axisUnits)
  {
    init (axisNames,
	  axisUnits);
  }
  
  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  /*!
    \param axisNames   -- World axis names.
    \param axisUnits   -- World axis units.
    \param pixelValues -- Pixel axis values.
    \param worldValues -- World axis values.
  */
  template <class T>
  TabularCoordinate<T>::TabularCoordinate (std::string const &axisNames,
					   std::string const &axisUnits,
					   std::vector<double> const &pixelValues,
					   std::vector<T> const &worldValues)
  {
    // Initialize basic parameters
    init (axisNames,
	  axisUnits);
    // Set axis values
    setAxisValues (pixelValues,
		   worldValues);
  }
  
  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  template <class T>
  TabularCoordinate<T>::TabularCoordinate (TabularCoordinate<T> const &other)
    : CoordinateInterface<T> (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         copy

  /*!
    \param other -- Other TabularCoordinate object to make copy of.
   */
  template <class T>
  void TabularCoordinate<T>::copy (TabularCoordinate const &other)
  {
    CoordinateInterface<T>::copy (other);
  }

  // ============================================================================
  //
  //  Parameter access 
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                               setPixelValues
  
  /*!
    \param names   -- World axis names.
    \return status -- Status of the operation; returns \e false in case an 
            error was encountered.
  */
  template <class T>
  bool TabularCoordinate<T>::setAxisNames (std::string const &names)
  {
    std::vector<std::string> tmp (1,names);

    return CoordinateInterface<T>::setAxisNames (tmp);
  }

  //_____________________________________________________________________________
  //                                                               setPixelValues

  /*!
    \param units   -- World axis units.
    \return status -- Status of the operation; returns \e false in case an 
            error was encountered.
  */
  template <class T>
  bool TabularCoordinate<T>::setAxisUnits (std::string const &units)
  {
    std::vector<std::string> tmp (1,units);

    return CoordinateInterface<T>::setAxisUnits (tmp);
  }
  
  //_____________________________________________________________________________
  //                                                                      summary

  template <class T>
  void TabularCoordinate<T>::summary (std::ostream &os)
  {
    os << "[TabularCoordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type       = " << this->type()
       << " / " <<  this->name() << std::endl;
    os << "-- Storage type          = " << this->storageType_p.type()
       << " / " <<  this->storageType_p.name() << std::endl;
    os << "-- nof. axes             = " << this->nofAxes_p     << std::endl;
    os << "-- World axis names      = " << this->axisNames_p   << std::endl;
    os << "-- World axis units      = " << this->axisUnits_p   << std::endl;
    os << "-- Pixel values          = " << this->pixelValues_p << std::endl;
    os << "-- World values          = " << this->worldValues_p << std::endl;
    os << "-- Reference value       = " << this->refValue_p    << std::endl;
    os << "-- Reference pixel       = " << this->refPixel_p    << std::endl;
    os << "-- Transformation matrix = " << this->pc_p          << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
#ifdef HAVE_HDF5
  
  //_____________________________________________________________________________
  //                                                                    read_hdf5

  template <class T>
  void TabularCoordinate<T>::read_hdf5 (hid_t const &groupID)
  {
    std::string coordinateTypeName;

    DAL::h5get_attribute( groupID, "COORDINATE_TYPE",  coordinateTypeName );
    DAL::h5get_attribute( groupID, "NOF_AXES",         this->nofAxes_p );
    DAL::h5get_attribute( groupID, "AXIS_NAMES",       this->axisNames_p );
    DAL::h5get_attribute( groupID, "AXIS_UNITS",       this->axisUnits_p );
    DAL::h5get_attribute( groupID, "PIXEL_VALUES",     this->pixelValues_p );
    DAL::h5get_attribute( groupID, "WORLD_VALUES",     this->worldValues_p );
  }

  //_____________________________________________________________________________
  //                                                                    read_hdf5

  template <class T>
  void TabularCoordinate<T>::read_hdf5 (hid_t const &locationID,
					std::string const &name)
  {
    hid_t groupID (0);
    
    groupID = H5Gopen1 (locationID,
                        name.c_str());
    
    if (groupID) {
      read_hdf5 (groupID);
    }
    else {
      std::cerr << "[TabularCoordinate::read_hdf5] Error opening group "
		<< name
		<< std::endl;
    }
    
    H5Gclose (groupID);
  }
  
  //_____________________________________________________________________________
  //                                                                   write_hdf5
  
  template <class T>
  void TabularCoordinate<T>::write_hdf5 (hid_t const &groupID)
  {
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE",  this->name() );
    DAL::h5set_attribute( groupID, "NOF_AXES",         this->nofAxes_p );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",       this->axisNames_p );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",       this->axisUnits_p );
    DAL::h5set_attribute( groupID, "PIXEL_VALUES",     this->pixelValues_p );
    DAL::h5set_attribute( groupID, "WORLD_VALUES",     this->worldValues_p );
  }

  //_____________________________________________________________________________
  //                                                                      write_hdf5

  template <class T>
  void TabularCoordinate<T>::write_hdf5 (hid_t const &locationID,
					 std::string const &name)
  {
    hid_t groupID (0);
    // create HDF5 group
    if (H5Lexists (locationID, name.c_str(), H5P_DEFAULT)) {
      groupID = H5Gopen (locationID,
			 name.c_str(),
			 H5P_DEFAULT);
      
    } else {
      groupID = H5Gcreate( locationID,
			   name.c_str(),
			   H5P_DEFAULT,
			   H5P_DEFAULT,
			   H5P_DEFAULT );
    }
    // write coordinate attributes
    write_hdf5 (groupID);
    // close the group after write
    H5Gclose (groupID);
  }

#endif

  //_____________________________________________________________________________
  //                                                             exportCoordinate

#ifdef HAVE_CASA
  template <class T>
  void TabularCoordinate<T>::exportCoordinate (casa::TabularCoordinate &coord)
  {
    unsigned int nelem = this->pixelValues_p.size();
    casa::Vector<double> pixel (nelem);
    casa::Vector<T> world (nelem);

    for (unsigned int n(0); n<nelem; ++n) {
      pixel(n) = this->pixelValues_p[n];
      world(n) = this->worldValues_p[n];
    }

    coord = casa::TabularCoordinate (pixel,
				     world,
				     this->axisUnits_p[0],
				     this->axisNames_p[0]);
  }
#endif

  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init

  /*!
    \param axisNames   -- World axis names.
    \param axisUnits   -- World axis units.
    \param pixelValues -- Pixel axis values.
    \param worldValues -- World axis values.
  */
  template <class T>
  void TabularCoordinate<T>::init (std::vector<std::string> const &axisNames,
				   std::vector<std::string> const &axisUnits,
				   std::vector<double> const &pixelValues,
				   std::vector<T> const &worldValues)
  {
    /* Initialize base class */
    CoordinateInterface<T>::init (Coordinate::TABULAR,
			       1,
			       Coordinate::TABULAR);
    
    CoordinateInterface<T>::setAxisNames (axisNames);
    CoordinateInterface<T>::setAxisUnits (axisUnits);
    setAxisValues (pixelValues, worldValues);
  }
  
  
} // Namespace DAL -- end
