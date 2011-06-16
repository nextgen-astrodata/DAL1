/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren <lbaehren@gmail.com>                                     *
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

/*!
  \file core_test.h

  \ingroup DAL
  \ingroup core

  \brief Collection of functions to be commonly usd by the test programs
 
  \author Lars B&auml;hren
 
  \date 2011/06/10
*/

#ifndef CORE_TEST_H
#define CORE_TEST_H

#include <iostream>
#include <sstream>
#include <string>
#include <core/dalCommon.h>
#include <core/HDF5Attribute.h>
#include <core/HDF5Dataset.h>

//_______________________________________________________________________________
//                                                                   datasetNames

/*!
  \brief Generate names for datasets of available datatypes
  
  The name of a dataset is generated from the input parameters as follows:
  \verbatim
  <nameBase><rank>D_<type>
  \endverbatim

  \param rank     -- Rank of the dataset to be created.
  \param chunking -- Is chunking of the dataset going to be enabled? If set
         \e true the additional string \c _chunk will be appended to the name.
  \param nameBase -- Base string of the dataset name
  \return names   -- Map with the combination of dataset names and their
          corresponding HDF5 datatypes.
*/
std::map<std::string,hid_t> datasetNames (unsigned int const &rank=1,
					  bool const &chunking=false,
					  std::string const &nameBase="Dataset_")
{
  /*
    Establish the base set of combinations
   */

  std::map<hid_t,std::string> types;
  std::map<hid_t,std::string>::iterator it;
  
  types[H5T_NATIVE_SHORT]   = "short";
  types[H5T_NATIVE_USHORT]  = "ushort";
  types[H5T_NATIVE_INT]     = "int";
  types[H5T_NATIVE_UINT]    = "uint";
  types[H5T_NATIVE_LONG]    = "long";
  types[H5T_NATIVE_ULONG]   = "ulong";
  types[H5T_NATIVE_LLONG]   = "llong";
  types[H5T_NATIVE_ULLONG]  = "ullong";
  types[H5T_NATIVE_FLOAT]   = "float";
  types[H5T_NATIVE_DOUBLE]  = "double";
  types[H5T_NATIVE_LDOUBLE] = "ldouble";
  types[H5T_NATIVE_INT8]    = "int8";
  types[H5T_NATIVE_INT16]   = "int16";
  types[H5T_NATIVE_INT32]   = "int32";
  types[H5T_NATIVE_INT64]   = "int64";

  /*
    Generate set of names base on the input parameters
  */
  
  std::map<std::string,hid_t> names;
  std::ostringstream os;
  
  // Loop through the array datatypes
  for (it=types.begin(); it!=types.end(); ++it) {
    // Assemble name of the dataset from the individual components
    os.str("");
    os << nameBase << rank << "D_" << it->second;
    if (chunking) {
      os << "_chunk";
    }
    // Set entry 
    names[os.str()] = it-> first;
  }
  
  return names;
}

//_______________________________________________________________________________
//                                                                    findDataset

/*!
  \brief Find a dataset within the HDF5 file \e filename

  \param filename -- Name of the HDF5 file.
  \retval dataset -- Name of the dataset.
  \return status  -- Returns \e true if a dataset could be located.
*/
bool findDataset (std::string const &filename,
		  std::string &dataset)
{
  bool haveDataset (false);
  hid_t location;
  std::string name;
  std::vector<hid_t> locations;
  std::vector<hid_t>::reverse_iterator loc;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  dataset="";

  //______________________________________________
  // Open the HDF5 file to inspect
  
  location = H5Fopen (filename.c_str(),
		      H5F_ACC_RDWR,
		      H5P_DEFAULT);
  if (H5Iis_valid(location)) {
    locations.push_back(location);
  } else {
    return false;
  }
  
  //______________________________________________
  // Go through the data structure

  while (!haveDataset) {
    // Check for dataset attached to the current location
    cout << "-- Checking for datasets location " << location << " ..." << endl;
    names.clear();
    DAL::h5get_names (names,location,H5G_DATASET);
    // Did we find a dataset?
    if (names.empty()) {
      // Check for groups attached to the current location
      cout << "-- Checking for groups location " << location << " ..." << endl;
      names.clear();
      DAL::h5get_names (names,location,H5G_GROUP);
      // Did we find a group? If not, that's it.
      if (names.empty()) {
	return false;
      }
      else {
	it    = names.begin();
	name += "/" + *it;
	// Open HDF5 group
	cout << "-- Opening group " << name << " ..." << endl;
	location = H5Gopen (locations[0],
			    name.c_str(),
			    H5P_DEFAULT);
	// Store object identifier
	if (H5Iis_valid(location)) {
	  locations.push_back(location);
	}
      }
    }
    else {
      // Store information
      it          = names.begin();
      dataset     = name + "/" + *it;
      haveDataset = true;
      // Feedback
      std::cout << "-- Dataset       = " << dataset      << std::endl;
      std::cout << "-- nof. datasets = " << names.size() << std::endl;
    }
  }  //  END -- while (!haveDataset)
  
  //______________________________________________
  // Close HDF5 object identifiers

  for (loc=locations.rbegin(); loc!=locations.rend(); ++loc) {
    if (H5Iis_valid(*loc)) {
      switch (H5Iget_type(*loc)) {
      case H5I_FILE:
	cout << "-- Closing HDF5 file " << *loc << " ..." << endl;
	H5Fclose(*loc);
	break;
      case H5I_GROUP:
	cout << "-- Closing HDF5 group " << *loc << " ..." << endl;
	H5Gclose(*loc);
	break;
      default:
	H5Oclose(*loc);
      break;
      }
    }
  }

  return haveDataset;
}

#endif
