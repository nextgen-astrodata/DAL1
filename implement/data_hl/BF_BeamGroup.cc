/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
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

#include <BF_BeamGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                BF_BeamGroup
  
  BF_BeamGroup::BF_BeamGroup ()
  {
    location_p = 0;
    itsProcessingHistory.clear();
    itsCoordinates.clear();
  }
  
  //_____________________________________________________________________________
  //                                                                 BF_BeamGroup
  
  BF_BeamGroup::BF_BeamGroup (hid_t const &location,
			      std::string const &name)
  {
    open (location,name,false);
  }
  
  //_____________________________________________________________________________
  //                                                                 BF_BeamGroup
  
  BF_BeamGroup::BF_BeamGroup (hid_t const &location,
			      unsigned int const &index,
			      bool const &create)
  {
    open (location,getName(index),create);
  }
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                     ~BF_BeamGroup

  BF_BeamGroup::~BF_BeamGroup ()
  {
    if (location_p > 0) {
      herr_t h5error;
      H5I_type_t object_type = H5Iget_type(location_p);
      // clear maps with embedded objects
      itsProcessingHistory.clear();
      itsCoordinates.clear();
      // release HDF5 object
      if (object_type == H5I_GROUP) {
	h5error = H5Gclose(location_p);
	location_p = 0;
      }
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void BF_BeamGroup::summary (std::ostream &os)
  {
    std::map<std::string,BF_ProcessingHistory>::iterator hist;
    std::map<std::string,CoordinatesGroup>::iterator coord;

    hist  = itsProcessingHistory.begin();
    coord = itsCoordinates.begin();

    os << "[BF_BeamGroup] Summary of internal parameters." << std::endl;
    os << "-- Location ID                   = " << location_p
       << std::endl;

    if (itsProcessingHistory.size() > 0) {
    os << "-- Location ID ProcessingHistory = " << hist->second.locationID()
       << std::endl;
    }

    if (itsCoordinates.size() > 0) {
    os << "-- Location ID CoordinatesGroup  = " << coord->second.locationID()
       << std::endl;
    }
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_BeamGroup::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("TARGET");
    attributes_p.insert("NOF_STATIONS");
    attributes_p.insert("STATIONS_LIST");
    attributes_p.insert("POINT_RA");
    attributes_p.insert("POINT_DEC");
    attributes_p.insert("POSITION_OFFSET_RA");
    attributes_p.insert("POSITION_OFFSET_DEC");
    attributes_p.insert("PB_DIAMETER_RA");
    attributes_p.insert("PB_DIAMETER_DEC");
    attributes_p.insert("PB_CENTER_FREQUENCY");
    attributes_p.insert("PB_CENTER_FREQUENCY_UNIT");
    attributes_p.insert("FOLDED_DATA");
    attributes_p.insert("FOLD_PERIOD");
    attributes_p.insert("FOLD_PERIOD_UNIT");
    attributes_p.insert("DEDISPERSION");
    attributes_p.insert("DISPERSION_MEASURE");
    attributes_p.insert("DISPERSION_MEASURE_UNIT");
    attributes_p.insert("BARYCENTER");
    attributes_p.insert("STOKES_COMPONENTS");
    attributes_p.insert("COMPLEX_VOLTAGE");
    attributes_p.insert("SIGNAL_SUM");
  }

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_BeamGroup::open (hid_t const &location,
			   std::string const &name,
			   bool const &create)
  {
    bool status (true);

    /* Set up the list of attributes attached to the Beam group */
    setAttributes();

    /* Try to open the group: get list of groups attached to 'location' and
       check if 'name' is part of it.
    */
    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      location_p = H5Gopen2 (location,
			     name.c_str(),
			     H5P_DEFAULT);
    } else {
      location_p = 0;
    }
    
    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if (create) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::string grouptype ("Beam");
	  std::string undefined ("UNDEFINED");
	  bool ok (false);
	  std::vector<std::string> stations (1,undefined);
	  std::vector<std::string> stokes (1,undefined);
	  // write the attributes
	  h5set_attribute (location_p,"GROUPTYPE",                grouptype   );
	  h5set_attribute (location_p,"TARGET",                   undefined   );
	  h5set_attribute (location_p,"NOF_STATIONS",             int(0)      );
	  h5set_attribute (location_p,"STATIONS_LIST",            stations    );
	  h5set_attribute (location_p,"POINT_RA",                 double(0.0) );
	  h5set_attribute (location_p,"POINT_DEC",                double(0.0) );
	  h5set_attribute (location_p,"POSITION_OFFSET_RA",       double(0.0) );
	  h5set_attribute (location_p,"POSITION_OFFSET_DEC",      double(0.0) );
	  h5set_attribute (location_p,"PB_DIAMETER_RA",           double(0.0) );
	  h5set_attribute (location_p,"PB_DIAMETER_DEC",          double(0.0) );
	  h5set_attribute (location_p,"PB_CENTER_FREQUENCY",      double(0.0) );
	  h5set_attribute (location_p,"PB_CENTER_FREQUENCY_UNIT", undefined   );
	  h5set_attribute (location_p,"FOLDED_DATA",              ok          );
	  h5set_attribute (location_p,"FOLD_PERIOD",              float(0.0)  );
	  h5set_attribute (location_p,"FOLD_PERIOD_UNIT",         undefined   );
	  h5set_attribute (location_p,"DEDISPERSION",             undefined   );
	  h5set_attribute (location_p,"DISPERSION_MEASURE",       float(0.0)  );
 	  h5set_attribute (location_p,"DISPERSION_MEASURE_UNIT",  undefined   );
 	  h5set_attribute (location_p,"BARYCENTER",               ok          );
 	  h5set_attribute (location_p,"STOKES_COMPONENTS",        stokes      );
 	  h5set_attribute (location_p,"COMPLEX_VOLTAGE",          ok          );
	  h5set_attribute (location_p,"SIGNAL_SUM",               undefined   );
	} else {
	  std::cerr << "[BF_BeamGroup::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_BeamGroup::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[BF_StationBeam::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_BeamGroup::openEmbedded (bool const &create)
  {
    bool status (true);
    std::set<std::string> groupnames;
    
    /* Retrieve the names of the groups attached to the StationBeam group */
    status = h5get_names (groupnames,
			  location_p,
			  H5G_GROUP);

    /* Open the processing history group */
    status = openProcessingHistory (create);
    /* Open coordinates group */
    status = openCoordinatesGroup (create);

    return status;
  }

  //_____________________________________________________________________________
  //                                                        openProcessingHistory
  
  bool BF_BeamGroup::openProcessingHistory (bool const &create)
  {
    bool status (true);

    if (itsProcessingHistory.size() == 0 && location_p > 0) {
      itsProcessingHistory["ProcessingHistory"] = BF_ProcessingHistory (location_p,create);
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                         openCoordinatesGroup
  
  bool BF_BeamGroup::openCoordinatesGroup (bool const &create)
  {
    bool status (true);

    if (itsCoordinates.size() == 0 && location_p > 0) {
      itsCoordinates["CoordinatesGroup"] = CoordinatesGroup (location_p,create);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                         openCoordinatesGroup

  /*!
    \param name    -- Name of the Stokes dataset to be opened.
    \return status -- Satus of the operation; returns \e false in case the
            dataset does not exist and/or cannot be opened.
   */
  bool BF_BeamGroup::openStokesDataset (std::string const &name)
  {
    bool status (true);
    std::map<std::string,BF_StokesDataset>::iterator it;

    /* Check if the requested dataset is opened already. */

    it = itsStokesDatasets.find(name);

    if (it == itsStokesDatasets.end()) {
      /* If the iterator points towards the end of the map, the dataset has not
	 been opened yet. Check if a dataset of given name does exit. */
      if (H5Iis_valid(location_p)) {
	/* Try opening Stokes dataset */
	BF_StokesDataset stokes (location_p, name);
	/* Check if opening the dataset was successful */
	if (stokes.hasValidID()) {
	  itsStokesDatasets[name] = stokes;
	} else {
	  std::cerr << "[BF_BeamGroup::openStokesDataset]"
		    << " No such dataset " << name << "!"
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_BeamGroup::openStokesDataset]"
		  << " Object not connect to valid HDF5 group!"
		  << std::endl;
	status = false;
      }
    }


    return status;
  }

  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the beam.

    \return name -- The name of the beam group, <tt>Beam<index></tt>
  */
  std::string BF_BeamGroup::getName (unsigned int const &index)
  {
    char uid[10];
    sprintf(uid,
            "%03d",
	    index);
    
    std::string name (uid);
    
    name = "Beam" + name;
    
    return name;
  }
  
} // Namespace DAL -- end
