/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007                                                    *
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

#include <Enumerations.h>
#include <TBB_StationGroup.h>

using std::cout;
using std::endl;

namespace DAL {  // Namespace DAL -- begin

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
  /*!
    Default constructor does not connect to a file, but simply sets up internal
    parameters.
  */
  TBB_StationGroup::TBB_StationGroup ()
  {
    location_p             = 0;
    nofTriggeredAntennas_p = 0;
    selectedDipoles_p.clear();
  }

  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
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
  TBB_StationGroup::TBB_StationGroup (hid_t const &location,
                                      std::string const &group)
  {
    open (location, group, false);
  }

  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
  TBB_StationGroup::TBB_StationGroup (hid_t const &location,
				      unsigned int const &stationID,
				      bool const &create)
  {
    std::string name = getName (stationID);
    open (location, name, create);
  }
  
  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
  /*!
    \param groupID -- Object identifier for the group within the HDF5 dataset
  */
  TBB_StationGroup::TBB_StationGroup (hid_t const &groupID)
  {
    open (groupID);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                            ~TBB_StationGroup
  
  TBB_StationGroup::~TBB_StationGroup ()
  {
    destroy();
  }

  //_____________________________________________________________________________
  //                                                                      destroy
  
  void TBB_StationGroup::destroy ()
  {
    herr_t h5error;
    H5I_type_t object_type = H5Iget_type(location_p);
    // release HDF5 object
    if (object_type == H5I_GROUP) {
      h5error = H5Gclose(location_p);
      location_p = 0;
    }
    // clear standard containers
    selectedDipoles_p.clear();
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   operator=
  
  /*!
    \param other -- Another TBB_StationGroup object from which to make a
           copy.
  */
  TBB_StationGroup& TBB_StationGroup::operator= (TBB_StationGroup const &other)
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
    \param other -- Another TBB_StationGroup object from which to create
           this new one.
  */
  void TBB_StationGroup::copy (TBB_StationGroup const &other)
  {
    location_p  = other.location_p;

    open (other.location_p);
  }
  
  // ============================================================================
  //
  //  Parameter access - station group
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void TBB_StationGroup::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("STATION_POSITION_VALUE");
    attributes_p.insert("STATION_POSITION_UNIT");
    attributes_p.insert("STATION_POSITION_FRAME");
    attributes_p.insert("BEAM_DIRECTION_VALUE");
    attributes_p.insert("BEAM_DIRECTION_UNIT");
    attributes_p.insert("BEAM_DIRECTION_FRAME");
    attributes_p.insert("TRIGGER_TYPE");
    attributes_p.insert("TRIGGER_OFFSET");
    attributes_p.insert("TRIGGERED_ANTENNAS");
    attributes_p.insert("NOF_DIPOLES");
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  bool TBB_StationGroup::open (hid_t const &location)
  {
    bool status (true);
    bool absolutePath (false);
    herr_t h5error;
    std::string filename;
    std::string dataset;
    
    // Get name of file and dataset ________________________
    
    status  = DAL::h5get_filename (filename, location);
    status *= DAL::h5get_name (dataset, location,absolutePath);

    if (status) {
      // open the file
      hid_t fileID = H5Fopen (filename.c_str(),
			      H5F_ACC_RDWR,
			      H5P_DEFAULT);
      // open the dataset
      status = open (fileID,dataset,false);
      // release file handler
      h5error = H5Fclose (fileID);
    }
    
    return status;
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
  bool TBB_StationGroup::open (hid_t const &location,
			       std::string const &name,
			       bool const &create)
  {
    bool status (true);

    /* Internal setup */
    setAttributes();
    nofTriggeredAntennas_p = 0;

    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      location_p = H5Gopen (location,
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
	  std::string grouptype ("StationGroup");
	  std::string undefined ("UNDEFINED");
	  std::vector<uint> triggered (1,0);
	  std::vector<double> vectD (3,0.0);
	  std::vector<std::string> vectUnits (3,undefined);
	  // write the attributes
	  h5set_attribute (location_p,"GROUPTYPE",                grouptype   );
	  h5set_attribute (location_p,"STATION_POSITION_VALUE",   vectD       );
	  h5set_attribute (location_p,"STATION_POSITION_UNIT",    vectUnits   );
	  h5set_attribute (location_p,"STATION_POSITION_FRAME",   undefined   );
	  h5set_attribute (location_p,"BEAM_DIRECTION_VALUE",     vectD       );
	  h5set_attribute (location_p,"BEAM_DIRECTION_UNIT",      vectUnits   );
	  h5set_attribute (location_p,"BEAM_DIRECTION_FRAME",     undefined   );
 	  h5set_attribute (location_p,"TRIGGER_TYPE",             undefined   );
 	  h5set_attribute (location_p,"TRIGGER_OFFSET",           double(0.0) );
 	  h5set_attribute (location_p,"TRIGGERED_ANTENNAS",       triggered   );
	  h5set_attribute (location_p,"NOF_DIPOLES",              uint(0)     );
	} else {
	  std::cerr << "[TBB_StationGroup::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[TBB_StationGroup::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[TBB_StationGroup::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool TBB_StationGroup::openEmbedded (bool const &create)
  {
    bool status = create;

    // Open dipole datasets ________________________________

    if (location_p > 0) {
      std::set<std::string> names;
      std::set<std::string>::iterator it;

      h5get_names (names,location_p,H5G_DATASET);

      if (names.size() > 0) {
	datasets_p.clear();
	selectedDipoles_p.clear();
	for (it=names.begin(); it!=names.end(); ++it) {
	  datasets_p[*it] = TBB_DipoleDataset(location_p,*it);
	  selectedDipoles_p.insert(*it);
	}
      } else {
	status = false;
      }
    } else {
      std::cerr << "[TBB_StationGroup::setDipoleDatasets]"
		<< " Unable to open dipole dataset - no connect to group!"
		<< std::endl;
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- The index identifying the station
  */
  std::string TBB_StationGroup::getName (unsigned int const &index)
  {
    char uid[10];
    sprintf(uid,
            "%03d",
	    index);
    
    std::string name (uid);
    
    name = "Station" + name;
    
    return name;
  }
  
  //_____________________________________________________________________________
  //                                                                   group_name
  
  /*!
    \param stripPath -- Strip away the path and just return the name of the
           group? By default the full path is returned, so make sure to set
	   <tt>stripPath=true</tt> if you want the name only.
    
    \return name -- The name for this group within the HDF5 file
  */
  std::string TBB_StationGroup::group_name (bool const &stripPath)
  {
    bool status (true);
    std::string name ("UNDEFINED");

    status = DAL::h5get_name (name,location_p);

    if (status) {
      if (stripPath) {
	return name.substr(1);
      }
    }
    else {
      std::cerr <<  "[TBB_StationGroup::group_name] "
		<< "Error retrieving name of HDF5 group!"
		<< std::endl;
    }
    
    return name;
  }
  
  //_____________________________________________________________________________
  //                                                             station_position
  
#ifdef HAVE_CASA
  casa::MPosition TBB_StationGroup::station_position ()
  {
    return DAL::h5get_position (location_p,
                                DAL::STATION_POSITION_VALUE,
                                DAL::STATION_POSITION_UNIT,
                                DAL::STATION_POSITION_FRAME);
  }
#endif
  
  //_____________________________________________________________________________
  //                                                           setSelectedDipoles
  
  /*!
    \param selection -- std::set with the names of dipoles to be selected; if the 
           provided set is empty no changes to the internal settings are made.

    \return status -- Status of the operation; returns \e true in case the new
            values successfully have been stored.
  */
  bool TBB_StationGroup::setSelectedDipoles (std::set<std::string> const &selection)
  {
    bool status (true);

    if (selection.empty()) {
      status = false;
    } else {
      std::set<std::string> tmp;
      std::set<std::string>::iterator it = selection.begin();
      std::map<std::string,TBB_DipoleDataset>::iterator dat;
      /* Got through the provided selection set */
      for (; it!=selection.end(); ++it) {
	/* If the dipole in the selection is ok, accept it */
	dat = datasets_p.find(*it);
	if (dat != datasets_p.end()) {
	  tmp.insert(*it);
	}
      }
      /* Store the temporary selection set */
      if (tmp.size() > 0) {
	selectedDipoles_p.clear();
	selectedDipoles_p = tmp;
      } else {
	status = false;
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void TBB_StationGroup::summary (std::ostream &os)
  {
    os << "[TBB_StationGroup] Summary of object properties"     << endl;

    os << "-- Group ID  .............. : " << location_p           << endl;

    if (location_p > 0) {
      std::string triggerType;
      double triggerOffset;
      std::vector<uint> triggeredAntennas;
      std::vector<double> stationPositionValue;
      std::vector<std::string> stationPositionUnit;
      std::string stationPositionFrame;
      std::vector<double> beamDirectionValue;
      std::vector<std::string> beamDirectionUnit;
      std::string beamDirectionFrame;
      //
      getAttribute ("TRIGGER_TYPE",           triggerType);
      getAttribute ("TRIGGER_OFFSET",         triggerOffset);
      getAttribute ("TRIGGERED_ANTENNAS",     triggeredAntennas);
      getAttribute ("STATION_POSITION_VALUE", stationPositionValue);
      getAttribute ("STATION_POSITION_UNIT",  stationPositionUnit);
      getAttribute ("STATION_POSITION_FRAME", stationPositionFrame);
      getAttribute ("BEAM_DIRECTION_VALUE",   beamDirectionValue);
      getAttribute ("BEAM_DIRECTION_UNIT",    beamDirectionUnit);
      getAttribute ("BEAM_DIRECTION_FRAME",   beamDirectionFrame);
      //
      os << "-- Group name ............. : " << group_name(true)        << endl;
      os << "-- nof. dipole datasets ... : " << nofDipoleDatasets()     << endl;
      os << "-- Station position (Value) : " << stationPositionValue    << endl;
      os << "-- Station position (Unit)  : " << stationPositionUnit     << endl;
      os << "-- Station position (Frame) : " << stationPositionFrame    << endl;
      os << "-- Beam direction (Value) . : " << beamDirectionValue      << endl;
      os << "-- Beam direction (Unit) .. : " << beamDirectionUnit       << endl;
      os << "-- Beam direction (Frame).. : " << beamDirectionFrame      << endl;
      os << "-- Trigger type ........... : " << triggerType             << endl;
      os << "-- Trigger offset ......... : " << triggerOffset           << endl;
      os << "-- nof. triggered antennas  : " << nofTriggeredAntennas()  << endl;
      os << "-- Triggered antennas ..... : " << triggeredAntennas       << endl;
      os << "-- Selected dipoles ....... : " << selectedDipoles_p       << endl;
    }
    
  }
  
  // ============================================================================
  //
  //  Parameter access - dipole dataset
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                sample_offset
  
#ifdef HAVE_CASA
  casa::Vector<int> TBB_StationGroup::sample_offset (uint const &refAntenna)
  {
    uint nofDipoles              = nofDipoleDatasets();
    casa::Vector<uint> valTime;
    casa::Vector<uint> valSample;
    casa::Vector<int> offset (nofDipoles);

    getAttributes ("TIME",          valTime);
    getAttributes ("SAMPLE_NUMBER", valSample);

    for (uint n(0); n<nofDipoles; n++) {
      offset(n) = valTime(n)-valTime(refAntenna) + valSample(n)-valSample(refAntenna);
    }
    
    return offset;
  }
#else
  std::vector<int> TBB_StationGroup::sample_offset (uint const &refAntenna)
  {
    uint nofDipoles             = nofDipoleDatasets();
    std::vector<uint> valTime;
    std::vector<uint> valSample;
    std::vector<int> offset (nofDipoles);

    getAttributes ("TIME",          valTime);
    getAttributes ("SAMPLE_NUMBER", valSample);

    for (uint n(0); n<nofDipoles; n++) {
      offset[n] = valTime[n]-valTime[refAntenna] + valSample[n]-valSample[refAntenna];
    }
    
    return offset;
  }
#endif
  
  //_____________________________________________________________________________
  //                                                             antenna_position
  
#ifdef HAVE_CASA
  casa::Vector<casa::MPosition> TBB_StationGroup::antenna_position ()
  {
    uint n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    casa::Vector<casa::MPosition> position (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      position(n) = it->second.antenna_position();
      ++n;
    }
    
    return position;
  }
#endif
  
  //_____________________________________________________________________________
  //                                                                  dipoleNames
  
  void TBB_StationGroup::dipoleNames (std::vector<std::string> &names)
  {
    std::map<std::string,TBB_DipoleDataset>::iterator it;

    names.clear();
    
    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      names.push_back(it->second.getName());
    }
  }

#ifdef HAVE_CASA
  void TBB_StationGroup::dipoleNames (casa::Vector<casa::String> &names)
  {
    int n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;

    names.resize (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      names(n) = it->second.getName();
      ++n;
    }
  }
#endif

  //_____________________________________________________________________________
  //                                                                dipoleNumbers
  
  void TBB_StationGroup::dipoleNumbers (std::vector<int> &ids)
  {
    std::map<std::string,TBB_DipoleDataset>::iterator it;

    ids.clear();

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      ids.push_back(it->second.dipoleNumber());
    }
  }

#ifdef HAVE_CASA
  void TBB_StationGroup::dipoleNumbers (casa::Vector<int> &names)
  {
    int n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;

    names.resize (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      names(n) = it->second.dipoleNumber();
      ++n;
    }
  }
#endif

  //_____________________________________________________________________________
  //                                                                   datasetIDs
  
  /*!
    \return dataset_ids -- Vector with a list of the identifiers to the
            HDF5 dataset objects within this station group.
  */
#ifdef HAVE_CASA
  casa::Vector<hid_t> TBB_StationGroup::datasetIDs ()
  {
    uint n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    casa::Vector<hid_t> id (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      id(n) = it->second.locationID();
      ++n;
    }
    
    return id;
  }
#else
  std::vector<hid_t> TBB_StationGroup::datasetIDs ()
  {
    uint n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    std::vector<hid_t> id (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      id[n] = it->second.locationID();
      ++n;
    }
    
    return id;
  }
#endif

  // ============================================================================
  //
  //  Methods using casacore
  //
  // ============================================================================
  
#ifdef HAVE_CASA
  
  //_____________________________________________________________________________
  //                                                                           fx
  
  /*!
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
    \param dipoles    -- identifiers of the dipoles, for which to retrieve the
           data.

    \return fx -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
  */
  bool TBB_StationGroup::fx (casa::Matrix<double> &data,
			     int const &start,
			     int const &nofSamples,
			     casa::Vector<casa::String> const &dipoles)
  {
    bool status (true);
    uint nofDipoles (dipoles.nelements());
    casa::IPosition shape (2,nofSamples,nofDipoles);
    casa::Vector<double> tmp (nofSamples);
    std::map<std::string,TBB_DipoleDataset>::iterator it;

    if (data.shape() != shape) {
      data.resize(shape);
    }
    
    for (uint n(0); n<nofDipoles; ++n) {
      it = datasets_p.find(dipoles(n));
      if (it != datasets_p.end()) {
	tmp = it->second.fx(start,nofSamples);
      } else {
	tmp = 0.0;
      }
      data.column(n) = tmp;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                           fx
  
  /*!
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.

    \return fx -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
  */
  casa::Matrix<double> TBB_StationGroup::fx (int const &start,
					     int const &nofSamples)
  {
    bool status (true);
    casa::Matrix<double> data;
    casa::Vector<casa::String> dipoles (datasets_p.size());
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    uint n (0);
    
    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      dipoles(n) = it->first;
      ++n;
    }

    status = fx (data,
		 start,
		 nofSamples,
		 dipoles);
    return data;
  }

  //_____________________________________________________________________________
  //                                                               beam_direction
  
  /*!
    \return direction -- The pointing direction of the station beam as a
            casa::Measure
  */
  casa::MDirection TBB_StationGroup::beam_direction ()
  {
    return DAL::h5get_direction (location_p,
                                 DAL::BEAM_DIRECTION_VALUE,
                                 DAL::BEAM_DIRECTION_UNIT,
                                 DAL::BEAM_DIRECTION_FRAME);
  }

  //_____________________________________________________________________________
  //                                                       antenna_position_value

  /*!
    \return values -- [antenna,coord] Matrix containing the position values
            for all the antennas in the station
  */
  casa::Matrix<double> TBB_StationGroup::antenna_position_value ()
  {
    unsigned int nofDipoles = datasets_p.size();
    casa::Matrix<double> positionValues (nofDipoles,3);
    casa::Vector<double> tmp;
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    uint n (0);

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      it->second.getAttribute("ANTENNA_POSITION_VALUE",tmp);
      positionValues.row(n) = tmp;
      ++n;
    }
    
    return positionValues;
  }
  
  //_____________________________________________________________________________
  //                                                        antenna_position_unit

  /*!
    \return units -- [antenna,coord] Matrix containing the physical units for
            the positions of the antennas in the station.
  */
  casa::Matrix<casa::String> TBB_StationGroup::antenna_position_unit ()
  {
    unsigned int nofDipoles = datasets_p.size();
    casa::Matrix<casa::String> antennaPositionUnits (nofDipoles,3);
    casa::Vector<casa::String> tmp;
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    uint n (0);

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      it->second.getAttribute("ANTENNA_POSITION_UNIT",tmp);
      antennaPositionUnits.row(n) = tmp;
      ++n;
    }
    
    return antennaPositionUnits;
  }
  
  //_____________________________________________________________________________
  //                                                             sample_frequency
  
  bool TBB_StationGroup::sample_frequency (casa::Vector<casa::MFrequency> &freq)
  {
    bool status (true);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    uint n (0);
    
    freq.resize (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      status *= it->second.sample_frequency(freq(n));
      ++n;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                            attributes2record

  /*!
    \param recursive -- Recursively add information from embedded HDF5
           objects, such as groups and datasets?

    \return record -- A casa::Record container holding the values of the
            attributes attached to the dataset for this dipole
  */
  casa::Record TBB_StationGroup::attributes2record (bool const &recursive)
  {
    bool status (true);
    casa::Record rec;
    
    status = attributes2record (rec,recursive);
    
    return rec;
  }
  
  //_____________________________________________________________________________
  //                                                            attributes2record
  
  bool TBB_StationGroup::attributes2record (casa::Record &rec,
					    bool const &recursive)
  {
    bool status (true);
    
    //________________________________________________________________
    // Retrieve the attributes attached to the station group
    
    std::string triggerType;
    double triggerOffset;
    casa::Vector<uint> triggeredAntennas;
    casa::Vector<double> stationPositionValue;
    casa::Vector<casa::String> stationPositionUnit;
    std::string stationPositionFrame;
    casa::Vector<double> beamDirectionValue;
    casa::Vector<casa::String> beamDirectionUnit;
    std::string beamDirectionFrame;
    //
    getAttribute ("TRIGGER_TYPE",           triggerType);
    getAttribute ("TRIGGER_OFFSET",         triggerOffset);
    getAttribute ("TRIGGERED_ANTENNAS",     triggeredAntennas);
    getAttribute ("STATION_POSITION_FRAME", stationPositionFrame);
    getAttribute ("BEAM_DIRECTION_VALUE",   beamDirectionValue);
    getAttribute ("BEAM_DIRECTION_UNIT",    beamDirectionUnit);
    getAttribute ("BEAM_DIRECTION_FRAME",   beamDirectionFrame);
    
    try {
      rec.define(casa::RecordFieldId("STATION_POSITION_VALUE"),
		 stationPositionValue);
      rec.define(casa::RecordFieldId("STATION_POSITION_UNIT"),
		 stationPositionUnit);
      rec.define(casa::RecordFieldId("STATION_POSITION_FRAME"),
		 stationPositionFrame);
      rec.define(casa::RecordFieldId("BEAM_DIRECTION_VALUE"),
		 beamDirectionValue);
      rec.define(casa::RecordFieldId("BEAM_DIRECTION_UNIT"),
		 beamDirectionUnit);
      rec.define(casa::RecordFieldId("BEAM_DIRECTION_FRAME"),
		 beamDirectionFrame);
      rec.define("TRIGGER_TYPE",         triggerType            );
      rec.define("TRIGGER_OFFSET",       triggerOffset          );
      rec.define("TRIGGERED_ANTENNAS",   triggeredAntennas      );
    }
    catch (std::string message) {
      std::cerr << "[TBB_StationGroup::attributes2record] "
		<< "Error filling the record with attribute values!\n"
		<< message
		<< std::endl;
      status=false;
    }
    
    //________________________________________________________________
    // Recursive retrieval of the attributes attached to the dipole
    // datasets.
    
    if (recursive) {
      std::string name;
      casa::Record recordDipole;
      std::map<std::string,TBB_DipoleDataset>::iterator it;
      uint n (0);
      
      for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
	name = it->second.getName();
	// retrieve the attributes for the dipole data-set as record
	recordDipole = it->second.attributes2record();
	// ... and add it to the existing record
	rec.defineRecord (name,recordDipole);
	// increment counter
	++n;
      }
    }
    
    return status;
  }

#endif
  
} // Namespace DAL -- end
