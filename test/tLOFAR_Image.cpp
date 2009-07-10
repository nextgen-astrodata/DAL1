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
#include <DirectionCoordinate.h>
#include <LinearCoordinate.h>
#include <TabularCoordinate.h>

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

//_______________________________________________________________________________
//                                                    create_direction_coordinate

/*!
  \brief Create a group for storing a direction coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  --
*/
void create_direction_coordinate (hid_t const &locationID,
                                  std::string const &groupName,
                                  std::string const &refname="J2000",
                                  std::string const &projection="SIN")
{
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

  DAL::DirectionCoordinate coord (axisNames,
                                  axisUnits,
                                  refValue,
                                  refPixel,
                                  increment,
                                  pc,
                                  refname,
                                  projection);
  coord.setLongpole (longpole);
  coord.setLatpole (latpole);
  coord.setProjectionParameters (projectionParam);

  coord.h5write (locationID,
                 groupName);

}

//_______________________________________________________________________________
//                                                       create_radius_coordinate

/*!
  \brief Create a group for storing a radius coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  --
 */
void create_radius_coordinate (hid_t const &locationID,
                               std::string const &groupName)
{
  unsigned int nofAxes (1);
  std::vector<std::string> axisNames (nofAxes);
  std::vector<std::string> axisUnits (nofAxes);
  std::vector<double> refValue (nofAxes);
  std::vector<double> refPixel (nofAxes);
  std::vector<double> increment (nofAxes);
  std::vector<double> pc (nofAxes*nofAxes);

  axisNames[0] = "Radius";
  axisUnits[0] = "m";
  refValue[0]  = 100;
  refPixel[0]  = 0;
  increment[0] = 1;
  pc[0] = 1;

  DAL::LinearCoordinate coord (nofAxes,
                               axisNames,
                               axisUnits,
                               refValue,
                               refPixel,
                               increment,
                               pc);

  coord.h5write (locationID,
                 groupName);
}

//_______________________________________________________________________________
//                                                         create_time_coordinate

/*!
  \brief Create a group for storing a time coordinate

  \param locationID -- HDF5 identifier for the location at which the coordinate
         group is created.
  \param groupName  --
 */
void create_time_coordinate (hid_t const &locationID,
                             std::string const &groupName)
{
  unsigned int nofAxes (1);
  std::vector<std::string> axisNames (nofAxes);
  std::vector<std::string> axisUnits (nofAxes);
  std::vector<double> refPixel (nofAxes);
  std::vector<double> refValue (nofAxes);
  std::vector<double> increment (nofAxes);
  std::vector<double> pc (nofAxes*nofAxes);

  axisNames[0] = "Time";
  axisUnits[0] = "s";
  refValue[0]  = -1e02;
  refPixel[0]  = 0;
  increment[0] = 1e-06;
  pc[0] = 1;

  DAL::LinearCoordinate coord (nofAxes,
                               axisNames,
                               axisUnits,
                               refValue,
                               refPixel,
                               increment,
                               pc);

  coord.h5write (locationID,
                 groupName);
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

  if (groupID)
    {
      std::string coordinateType ("Frequency");
      unsigned int nofAxes (1);
      std::vector<std::string> axisNames (nofAxes);
      std::vector<std::string> axisUnits (nofAxes);
      std::vector<double> refPixel (nofAxes);
      std::vector<double> refValue (nofAxes);
      std::vector<double> increment (nofAxes);
      std::vector<double> pc (nofAxes*nofAxes);
      std::string system ("TOPO");
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
      DAL::h5set_attribute( groupID, "SYSTEM",          system );
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

  if (groupID)
    {
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

  if (groupID)
    {
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
    }
  else
    {
      std::cerr << "ERROR - failed to create coordinates group!" << std::endl;
    }

  /* Add coordinate sub-groups */
  if (groupID)
    {
      hid_t frequencyID;
      //
      std::cout << "-- Adding direction coordinate ..." << std::endl;
      create_direction_coordinate (groupID,"COORDINATE0");
      std::cout << "-- Adding linear coordinate (Radius) ..." << std::endl;
      create_radius_coordinate (groupID,"COORDINATE1");
      std::cout << "-- Adding frequency coordinate ..." << std::endl;
      frequencyID = create_frequency_coordinate (groupID,"COORDINATE2");
      std::cout << "-- Adding time coordinate ..." << std::endl;
      create_time_coordinate (groupID,"COORDINATE3");
      //
      H5Gclose (frequencyID);
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

  if (groupID)
    {
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
    }
  else
    {
      std::cerr << "ERROR - failed to create coordinates group!" << std::endl;
    }

  /* Add coordinate sub-groups */
  if (groupID)
    {
      hid_t frequencyID;
      hid_t stokesID;
      //
      std::cout << "-- Adding direction coordinate ..." << std::endl;
      create_direction_coordinate (groupID,"COORDINATE0");
      std::cout << "-- Adding frequency coordinate ..." << std::endl;
      frequencyID = create_frequency_coordinate (groupID,"COORDINATE1");
      std::cout << "-- Adding stokes coordinate ..." << std::endl;
      stokesID = create_stokes_coordinate (groupID,"COORDINATE2");
      //
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
