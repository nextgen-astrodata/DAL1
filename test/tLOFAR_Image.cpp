/*-------------------------------------------------------------------------*
 | $Id:: tHDF5Common.cpp 2022 2008-09-26 12:06:09Z baehren               $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

/* Standard header files */
#include <iostream>
#include <string>

/* DAL header files */
#include <dalCommon.h>

using std::cout;
using std::cerr;
using std::endl;

/*!
  \file tLOFAR_Image.cpp

  \ingroup DAL

  \brief Testwise creation of HDF5-based images 
 
  \author Lars B&auml;hren
 
  \date 2009/06/23

  <h3>Synopsis</h3>

*/

// ==============================================================================
//
//  Class DAL::Coordinate
//
// ==============================================================================

namespace DAL {

  /*!
    \class Coordinate
    
    \ingroup DAL

    \brief A basic container for a coordinate object added to a HDF5 file
  */
  class Coordinate {

  protected:
    
    //! The type of coordinate
    std::string coordinateType_p;
    //! The number of coordinate axes
    unsigned int nofAxes_p;
    //! World axis names
    std::vector<std::string> axisNames_p;
    //! World axis units
    std::vector<std::string> axisUnits_p;
    //! Reference value (CRVAL)
    std::vector<double> refValue_p;
    //! Reference pixel (CRPIX)
    std::vector<double> refPixel_p;
    //! Coordinate axis increment (CDELT)
    std::vector<double> increment_p;
    //! Transformation matrix
    std::vector<double> pc_p;

  public:

    //! Default constructor
    Coordinate() {
      coordinateType_p="UNDEFINED";
      nofAxes_p=1;
      axisNames_p.resize(nofAxes_p);
      axisUnits_p.resize(nofAxes_p);
      axisNames_p[0]="UNDEFINED";
      axisUnits_p[0]="UNDEFINED";
    };

    //! Get the world axis names
    std::vector<std::string> axisNames () {
      return axisNames_p;
    }
    //! Set the world axis names
    void setAxisNames (std::vector<std::string> const &axisNames) {
      if (axisNames.size() == nofAxes_p) {
	axisNames_p = axisNames;
      }
    }
    
    //! Get the world axis units
    std::vector<std::string> axisUnits () {
      return axisUnits_p;
    }
    //! Set the world axis units
    void setAxisUnits (std::vector<std::string> const &axisUnits) {
      if (axisUnits.size() == nofAxes_p) {
	axisUnits_p = axisUnits;
      }
    }

    //! Get the reference value
    std::vector<double> refValue () {
      return refValue_p;
    }
    //! Set the reference value
    void setRefValue (std::vector<double> const &refValue) {
      if (refValue.size() == nofAxes_p) {
	refValue_p = refValue;
      }
    }

    //! Get the reference pixel
    std::vector<double> refPixel () {
      return refPixel_p;
    }
    //! Set the reference pixel
    void setRefPixel (std::vector<double> const &refPixel) {
      if (refPixel.size() == nofAxes_p) {
	refPixel_p = refPixel;
      }
    }

    /*!
      \brief Get the coordinate axis increment
      \return increment -- The increment along the coordinate axes
    */
    std::vector<double> increment () {
      return increment_p;
    }
    /*!
      \brief Set the coordinate axis increment
      \param increment -- The increment along the coordinate axes
    */
    void setIncrement (std::vector<double> const &increment) {
      if (increment.size() == nofAxes_p) {
	increment_p = increment;
      }
    }

    /*!
      \brief Get the transformation matrix
      \return pc -- The transformation matrix, in row-wise ordering, e.g. [00,01,10,11]
    */
    std::vector<double> pc () {
      return pc_p;
    }
    /*!
      \brief Set the transformation matrix
      \param pc -- The transformation matrix, in row-wise ordering, e.g. [00,01,10,11]
    */
    void setPc (std::vector<double> const &pc) {
      unsigned int nelem = nofAxes_p*nofAxes_p;
      if (pc.size() == nelem) {
	pc_p = pc;
      }
    }

    //! Write the coordinate object to a HDF5 file
    void h5write (hid_t const &locationID) {
      DAL::h5set_attribute( locationID, "COORDINATE_TYPE", coordinateType_p );
      DAL::h5set_attribute( locationID, "NOF_AXES",        nofAxes_p );
      DAL::h5set_attribute( locationID, "AXIS_NAMES",      axisNames_p );
      DAL::h5set_attribute( locationID, "AXIS_UNITS",      axisUnits_p );
      DAL::h5set_attribute( locationID, "CRPIX",           refPixel_p );
      DAL::h5set_attribute( locationID, "CRVAL",           refValue_p );
      DAL::h5set_attribute( locationID, "CDELT",           increment_p );
      DAL::h5set_attribute( locationID, "PC",              pc_p );
    }

    //! Write the coordinate object to a HDF5 file
    void h5write (hid_t const &locationID,
		  std::string const &name) {
      hid_t groupID (0);
      // create HDF5 group
      groupID = H5Gcreate( locationID,
			   name.c_str(),
			   H5P_DEFAULT,
			   H5P_DEFAULT,
			   H5P_DEFAULT );
      // write coordinate attributes
      h5write (groupID);
      // close the group after write
      H5Gclose (groupID);
    }

    //! Read the coordinate object from a HDF5 file
    void h5read (hid_t const &locationID);

    //! Read the coordinate object from a HDF5 file
    void h5read (hid_t const &groupID,
		 std::string const &name);

  };
  
}

// ==============================================================================
//
//  Testing routines
//
// ==============================================================================

//_______________________________________________________________________________
//                                                    create_direction_coordinate

/*!
  \brief Create a group for storing a direction coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  -- 
  
  \return groupID -- Identifier of the created HDF5 group; required properly close
          the opened object later on.
 */
hid_t create_direction_coordinate (hid_t const &locationID,
				   std::string const &groupName,
				   std::string const &refname="J2000",
				   std::string const &projection="SIN")
{
  hid_t groupID (0);
  
  groupID = H5Gcreate( locationID,
		       groupName.c_str(),
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT );
  
  if (groupID) {
    std::string coordinateType ("Direction");
    unsigned int nofAxes (2);
    std::vector<std::string> axisNames (nofAxes);
    std::vector<std::string> axisUnits (nofAxes);
    std::vector<double> refPixel (nofAxes);
    std::vector<double> refValue (nofAxes);
    std::vector<double> increment (nofAxes);
    std::vector<double> pc (nofAxes*nofAxes);
    std::vector<double> projectionParam (1,0);
    double longpole (0);
    double latpole (90);
    //
    axisNames[0] = "Longitude";
    axisNames[1] = "Latitude";
    axisUnits[0] = "deg";
    axisUnits[1] = "deg";
    refPixel[0]  = 0;
    refPixel[1]  = 0;
    refValue[0]  = 0;
    refValue[1]  = 90;
    increment[0] = 1;
    increment[1] = 1;
    pc[0] = 1;
    pc[1] = 0;
    pc[2] = 0;
    pc[3] = 1;
    //
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE",  coordinateType );
    DAL::h5set_attribute( groupID, "NOF_AXES",         nofAxes );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",       axisNames );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",       axisUnits );
    DAL::h5set_attribute( groupID, "CRPIX",            refPixel );
    DAL::h5set_attribute( groupID, "CRVAL",            refValue );
    DAL::h5set_attribute( groupID, "CDELT",            increment );
    DAL::h5set_attribute( groupID, "PC",               pc );
    DAL::h5set_attribute( groupID, "SYSTEM",           refname );
    DAL::h5set_attribute( groupID, "PROJECTION",       projection );
    DAL::h5set_attribute( groupID, "PROJECTION_PARAM", projectionParam );
    DAL::h5set_attribute( groupID, "LONGPOLE",         longpole );
    DAL::h5set_attribute( groupID, "LATPOLE",          latpole );
  }

  return groupID;
}

//_______________________________________________________________________________
//                                                       create_radius_coordinate

/*!
  \brief Create a group for storing a radius coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  -- 
  
  \return groupID -- Identifier of the created HDF5 group; required properly close
          the opened object later on.
 */
hid_t create_radius_coordinate (hid_t const &locationID,
				std::string const &groupName)
{
  hid_t groupID (0);
  
  groupID = H5Gcreate( locationID,
		       groupName.c_str(),
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT );
  
  if (groupID) {
    std::string coordinateType ("Linear");
    unsigned int nofAxes (1);
    std::vector<std::string> axisNames (nofAxes);
    std::vector<std::string> axisUnits (nofAxes);
    std::vector<double> refPixel (nofAxes);
    std::vector<double> refValue (nofAxes);
    std::vector<double> increment (nofAxes);
    std::vector<double> pc (nofAxes*nofAxes);
    //
    axisNames[0] = "Radius";
    axisUnits[0] = "m";
    refPixel[0]  = 0;
    refValue[0]  = 100;
    increment[0] = 1;
    pc[0] = 1;
    //
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE", coordinateType );
    DAL::h5set_attribute( groupID, "NOF_AXES",        nofAxes );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",      axisNames );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",      axisUnits );
    DAL::h5set_attribute( groupID, "CRPIX",           refPixel );
    DAL::h5set_attribute( groupID, "CRVAL",           refValue );
    DAL::h5set_attribute( groupID, "CDELT",           increment );
    DAL::h5set_attribute( groupID, "PC",              pc );
  }

  return groupID;
}

//_______________________________________________________________________________
//                                                         create_time_coordinate

/*!
  \brief Create a group for storing a time coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  -- 
  
  \return groupID -- Identifier of the created HDF5 group; required properly close
          the opened object later on.
 */
hid_t create_time_coordinate (hid_t const &locationID,
			      std::string const &groupName)
{
  hid_t groupID (0);
  
  groupID = H5Gcreate( locationID,
		       groupName.c_str(),
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT );
  
  if (groupID) {
    std::string coordinateType ("Linear");
    unsigned int nofAxes (1);
    std::vector<std::string> axisNames (nofAxes);
    std::vector<std::string> axisUnits (nofAxes);
    std::vector<double> refPixel (nofAxes);
    std::vector<double> refValue (nofAxes);
    std::vector<double> increment (nofAxes);
    std::vector<double> pc (nofAxes*nofAxes);
    //
    axisNames[0] = "Time";
    axisUnits[0] = "sec";
    refPixel[0]  = 0;
    refValue[0]  = -1e02;
    increment[0] = 1e-06;
    pc[0] = 1;
    //
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE", coordinateType );
    DAL::h5set_attribute( groupID, "NOF_AXES",        nofAxes );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",      axisNames );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",      axisUnits );
    DAL::h5set_attribute( groupID, "CRPIX",           refPixel );
    DAL::h5set_attribute( groupID, "CRVAL",           refValue );
    DAL::h5set_attribute( groupID, "CDELT",           increment );
    DAL::h5set_attribute( groupID, "PC",              pc );
  }

  return groupID;
}

//_______________________________________________________________________________
//                                                    create_frequency_coordinate

/*!
  \brief Create a group for storing a frequency coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  -- 
  
  \return groupID -- Identifier of the created HDF5 group; required properly close
          the opened object later on.
 */
hid_t create_frequency_coordinate (hid_t const &locationID,
				   std::string const &groupName)
{
  hid_t groupID (0);
  
  groupID = H5Gcreate( locationID,
		       groupName.c_str(),
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT );
  
  if (groupID) {
    std::string coordinateType ("Frequency");
    unsigned int nofAxes (1);
    std::vector<std::string> axisNames (nofAxes);
    std::vector<std::string> axisUnits (nofAxes);
    std::vector<double> refPixel (nofAxes);
    std::vector<double> refValue (nofAxes);
    std::vector<double> increment (nofAxes);
    std::vector<double> pc (nofAxes*nofAxes);
    //
    axisNames[0] = "Frequency";
    axisUnits[0] = "Hz";
    refPixel[0]  = 0;
    refValue[0]  = 30;
    increment[0] = 1;
    pc[0] = 1;
    //
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE", coordinateType );
    DAL::h5set_attribute( groupID, "NOF_AXES",        nofAxes );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",      axisNames );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",      axisUnits );
    DAL::h5set_attribute( groupID, "CRPIX",           refPixel );
    DAL::h5set_attribute( groupID, "CRVAL",           refValue );
    DAL::h5set_attribute( groupID, "CDELT",           increment );
    DAL::h5set_attribute( groupID, "PC",              pc );
  }

  return groupID;
}

//_______________________________________________________________________________
//                                                       create_stokes_coordinate

/*!
  \brief Create a group for storing a stokes coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  -- 
  
  \return groupID -- Identifier of the created HDF5 group; required properly close
          the opened object later on.
 */
hid_t create_stokes_coordinate (hid_t const &locationID,
				std::string const &groupName)
{
  hid_t groupID (0);
  
  groupID = H5Gcreate( locationID,
		       groupName.c_str(),
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT );
  
  if (groupID) {
    std::string coordinateType ("Stokes");
    unsigned int nofAxes (4);
    std::vector<std::string> axisNames (nofAxes);
    std::vector<std::string> axisUnits (nofAxes);
    //
    axisNames[0] = "I";
    axisNames[1] = "Q";
    axisNames[2] = "U";
    axisNames[3] = "V";
    axisUnits[0] = "UNDEFINED";
    axisUnits[1] = "UNDEFINED";
    axisUnits[2] = "UNDEFINED";
    axisUnits[3] = "UNDEFINED";
    //
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE", coordinateType );
    DAL::h5set_attribute( groupID, "NOF_AXES",        nofAxes );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",      axisNames );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",      axisUnits );
  }

  return groupID;
}

//_______________________________________________________________________________
//                                                                create_cr_image

/*!
  \brief Create set of coordinate attributes for a CR near-field image

  \verbatim
  FILE_CONTENTS {
    group      /
    group      /COORDINATES
    group      /COORDINATES/COORDINATE0
    group      /COORDINATES/COORDINATE1
    group      /COORDINATES/COORDINATE2
    group      /COORDINATES/COORDINATE3
  }
  \endverbatim
  
  \param filename -- Name of the output HDF5 file
*/
void create_cr_image (std::string const &filename)
{
  hid_t fileID;
  hid_t groupID;

  /* Create HDF5 file to work with */
  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);

  /* Create coordinates group */
  groupID = H5Gcreate( fileID,
		       "COORDINATES",
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT );
  
  if (groupID) {
    unsigned int nofCoordinates (4);
    unsigned int nofAxes (5);
    std::vector<std::string> coordinateTypes (nofCoordinates);
    //
    coordinateTypes[0] = "Direction";
    coordinateTypes[1] = "Radius";
    coordinateTypes[2] = "Frequency";
    coordinateTypes[3] = "Time";
    //
    DAL::h5set_attribute( groupID, "NOF_COORDINATES", nofCoordinates );
    DAL::h5set_attribute( groupID, "NOF_AXES", nofAxes );
    DAL::h5set_attribute( groupID, "COORDINATE_TYPES", coordinateTypes );
  } else {
    std::cerr << "ERROR - failed to create coordinates group!" << std::endl;
  }

  /* Add coordinate sub-groups */
  if (groupID) {
    hid_t directionID;
    hid_t radiusID;
    hid_t frequencyID;
    hid_t timeID;
    //
    std::cout << "-- Adding direction coordinate ..." << std::endl;
    directionID = create_direction_coordinate (groupID,"COORDINATE0");
    std::cout << "-- Adding linear coordinate (Radius) ..." << std::endl;
    directionID = create_radius_coordinate (groupID,"COORDINATE1");
    std::cout << "-- Adding frequency coordinate ..." << std::endl;
    frequencyID = create_frequency_coordinate (groupID,"COORDINATE2");
    std::cout << "-- Adding time coordinate ..." << std::endl;
    timeID = create_time_coordinate (groupID,"COORDINATE3");
    //
    H5Gclose (directionID);
    H5Gclose (radiusID);
    H5Gclose (frequencyID);
    H5Gclose (timeID);
  }

  /* Close file handles */
  H5Gclose (groupID);
  H5Fclose (fileID);
}

//_______________________________________________________________________________
//                                                               create_sky_image

void create_sky_image (std::string const &filename)
{
  hid_t fileID;
  hid_t groupID;

  /* Create HDF5 file to work with */
  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);

  /* Create coordinates group */
  groupID = H5Gcreate( fileID,
		       "COORDINATES",
		       H5P_DEFAULT,
		       H5P_DEFAULT,
		       H5P_DEFAULT );

  if (groupID) {
    unsigned int nofCoordinates (3);
    unsigned int nofAxes (4);
    std::vector<std::string> coordinateTypes (nofCoordinates);
    //
    coordinateTypes[0] = "Direction";
    coordinateTypes[1] = "Frequency";
    coordinateTypes[2] = "Stokes";
    //
    DAL::h5set_attribute( groupID, "NOF_COORDINATES", nofCoordinates );
    DAL::h5set_attribute( groupID, "NOF_AXES", nofAxes );
    DAL::h5set_attribute( groupID, "COORDINATE_TYPES", coordinateTypes );
  } else {
    std::cerr << "ERROR - failed to create coordinates group!" << std::endl;
  }

  /* Add coordinate sub-groups */
  if (groupID) {
    hid_t directionID;
    hid_t frequencyID;
    hid_t stokesID;
    //
    std::cout << "-- Adding direction coordinate ..." << std::endl;
    directionID = create_direction_coordinate (groupID,"COORDINATE0");
    std::cout << "-- Adding frequency coordinate ..." << std::endl;
    frequencyID = create_frequency_coordinate (groupID,"COORDINATE1");
    std::cout << "-- Adding stokes coordinate ..." << std::endl;
    stokesID = create_stokes_coordinate (groupID,"COORDINATE2");
    //
    H5Gclose (directionID);
    H5Gclose (frequencyID);
    H5Gclose (stokesID);
  }
  
  /* Close file handles */
  H5Gclose (groupID);
  H5Fclose (fileID);
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  /* Create standard sky image */
  create_sky_image ("testimage-sky.h5");
  /* Create CR near-field image */
  create_cr_image ("testimage-crc.h5");

  return nofFailedTests;
}
