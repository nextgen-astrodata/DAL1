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
//  Helper functions
//
// ==============================================================================

namespace DAL {

  class Coordinate {
    
    std::string coordinateType_p;
    unsigned int nofAxes_p;
    std::vector<std::string> axisNames_p;
    std::vector<std::string> axisUnits_p;
    std::vector<double> refValue_p;
    std::vector<double> refPixel_p;
    std::vector<double> increment_p;
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

    //! Set the world axis names
    void setAxisNames (std::vector<std::string> const &axisNames) {
      if (axisNames.size() == nofAxes_p) {
	axisNames_p = axisNames;
      }
    }

    //! Set the world axis units
    void setAxisUnits (std::vector<std::string> const &axisUnits) {
      if (axisUnits.size() == nofAxes_p) {
	axisUnits_p = axisUnits;
      }
    }

    //! Set the reference value
    void setRefValue (std::vector<double> const &refValue) {
      if (refValue.size() == nofAxes_p) {
	refValue_p = refValue;
      }
    }

    //! Set the reference pixel
    void setRefPixel (std::vector<double> const &refPixel) {
      if (refPixel.size() == nofAxes_p) {
	refPixel_p = refPixel;
      }
    }

    //! Set the coordinate axis increment
    void setIncrement (std::vector<double> const &increment) {
      if (increment.size() == nofAxes_p) {
	increment_p = increment;
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
		  std::string const &name);

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
				   std::string const &groupName)
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
// create_sky_image

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

int main (int argc,
	  char *argv[])
{
  int nofFailedTests (0);
  std::string outfile;

  if (argc < 2) {
    outfile = std::string("testimage.h5");
  } else {
    outfile = std::string(argv[1]);
  }

  create_sky_image (outfile);

  return nofFailedTests;
}
