/***************************************************************************
 *   Copyright (C) 2007                                                    *
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

#include <data_hl/TBB_StationGroup.h>

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
    datasets_p.clear();    
    selectedDatasets_p.clear();
  }

  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
  */
  TBB_StationGroup::TBB_StationGroup (hid_t const &location,
                                      std::string const &group,
                                      IO_Mode const &flags)
  {
    open (location, group, flags);
  }

  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
  TBB_StationGroup::TBB_StationGroup (hid_t const &location,
				      unsigned int const &stationID,
              IO_Mode const &flags)
  {
    std::string name = getName (stationID);
    open (location, name, flags);
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
    if (object_type == H5I_GROUP && H5Iis_valid(location_p)) {
      h5error = H5Gclose(location_p);
      location_p = 0;
    }
    // clear standard containers
    selectedDatasets_p.clear();
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
    hid_t fileID;
    IO_Mode flags = IO_Mode();
    
    // Get name of file and dataset ________________________
    
    status  = DAL::h5get_filename (filename, location);
    status *= DAL::h5get_name (dataset, location, absolutePath);

    if (status) {

      h5get_flags(flags, location);

      if (flags.flags() & IO_Mode::ReadOnly) {
        fileID = H5Fopen (filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
      }
      else {
        fileID = H5Fopen (filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

        /* If opening the the file failed, this might have been due to wrong
           access permissions; check if the file can be opened as read-only. */
        if (fileID<0) { fileID = H5Fopen (filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
        }
      }
        // open the dataset
      status = open (fileID,dataset,flags);
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
    \param flags  -- I/O mode flags.
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool TBB_StationGroup::open (hid_t const &location,
			       std::string const &name,
			       IO_Mode const &flags)
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
      if ( (flags.flags() & IO_Mode::Create) ||
	   (flags.flags() & IO_Mode::CreateNew) ) {
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
	  HDF5Attribute::write (location_p,"GROUPTYPE",                grouptype   );
	  HDF5Attribute::write (location_p,"STATION_POSITION_VALUE",   vectD       );
	  HDF5Attribute::write (location_p,"STATION_POSITION_UNIT",    vectUnits   );
	  HDF5Attribute::write (location_p,"STATION_POSITION_FRAME",   undefined   );
	  HDF5Attribute::write (location_p,"BEAM_DIRECTION_VALUE",     vectD       );
	  HDF5Attribute::write (location_p,"BEAM_DIRECTION_UNIT",      vectUnits   );
	  HDF5Attribute::write (location_p,"BEAM_DIRECTION_FRAME",     undefined   );
 	  HDF5Attribute::write (location_p,"TRIGGER_TYPE",             undefined   );
 	  HDF5Attribute::write (location_p,"TRIGGER_OFFSET",           double(0.0) );
 	  HDF5Attribute::write (location_p,"TRIGGERED_ANTENNAS",       triggered   );
	  HDF5Attribute::write (location_p,"NOF_DIPOLES",              uint(0)     );
	} else {
#ifdef DAL_DEBUGGING_MESSAGES
	  std::cerr << "[TBB_StationGroup::open] Failed to create group "
		    << name
		    << std::endl;
#endif
	  status = false;
	}
      } else {
#ifdef DAL_DEBUGGING_MESSAGES
	std::cerr << "[TBB_StationGroup::open] Failed to open group "
		  << name
		  << std::endl;
#endif
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (flags);
    }
#ifdef DAL_DEBUGGING_MESSAGES
    else {
      std::cerr << "[TBB_StationGroup::open] Skip opening embedded groups!"
		<< std::endl;
    }
#endif
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool TBB_StationGroup::openEmbedded (IO_Mode const &flags)
  {
    bool status = true;

    if (H5Iis_valid(location_p)) {
      std::set<std::string> groups;
      std::set<std::string> datasets;
      std::set<std::string>::iterator it;

      h5get_names (groups,location_p,H5G_GROUP);
      h5get_names (datasets,location_p,H5G_DATASET);

      // Open station calibration group ____________________

      // Open station trigger group ________________________

      it = groups.find(TBB_StationTrigger::getName());

      stationTrigger_p = TBB_StationTrigger (location_p,
					     TBB_StationTrigger::getName(),
					     flags);

      // Open dipole datasets ______________________________

      if (datasets.size() > 0) {
	datasets_p.clear();
	for (it=datasets.begin(); it!=datasets.end(); ++it) {
	  datasets_p[*it] = TBB_DipoleDataset(location_p,*it,flags);
	}
      } else {
	status = false;
      }

      status *= selectAllDipoles ();

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
  
#ifdef DAL_WITH_CASA
  casa::MPosition TBB_StationGroup::station_position ()
  {
    return DAL::h5get_position (location_p,
                                "STATION_POSITION_VALUE",
                                "STATION_POSITION_UNIT",
                                "STATION_POSITION_FRAME");
  }
#endif

  //_____________________________________________________________________________
  //                                                              selectedDipoles
  
  /*!
    \return selection -- Names of the selected dipole datasets.
  */
  std::set<std::string> TBB_StationGroup::selectedDipoles ()
  {
    std::set<std::string> selection;
    std::map<std::string,iterDipoleDataset>::iterator it;
    
    for (it=selectedDatasets_p.begin(); it!=selectedDatasets_p.end(); ++it) {
      selection.insert(it->first);
    }    

    return selection;
  }
  
  //_____________________________________________________________________________
  //                                                                selectDipoles
  
  /*!
    \param selection -- std::set with the names of dipoles to be selected; if the 
           provided set is empty no changes to the internal settings are made.

    \return status -- Status of the operation; returns \e true in case the new
            values successfully have been stored.
  */
  bool TBB_StationGroup::selectDipoles (std::set<std::string> const &selection)
  {
    /* If the input is empty, abort ... */
    if (selection.empty()) {
      return false;
    }

    /* ... else continue making the selection. */
    
    bool status (true);
    std::set<std::string>::iterator iterInput;
    std::map<std::string,iterDipoleDataset> tmpSelection;
    iterDipoleDataset iterSelection;
    
    for (iterInput=selection.begin(); iterInput!=selection.end(); ++iterInput) {
      /* Get pointer to the selected dataset. */
      iterSelection = datasets_p.find(*iterInput);
      /* If the selection is valid, accept it. */
      if (iterSelection!=datasets_p.end()) {
	tmpSelection[*iterInput] = iterSelection;
      }
    }

    /* If selection is non-empty, store the result. */
    
    if (tmpSelection.empty()) {
      std::cerr << "[TBB_StationGroup::selectDipoles]"
		<< " No valid selection of dipoles!"
		<< std::endl;
      status = false;
    } else {
      selectedDatasets_p.clear();
      selectedDatasets_p = tmpSelection;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                             selectAllDipoles
  
  /*!
    This method can be used to undo a previous selection of individual dipoles.

    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool TBB_StationGroup::selectAllDipoles ()
  {
    bool status (true);
    std::map<std::string,iterDipoleDataset> selection;
    iterDipoleDataset it;

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      selection[it->first] = it;
    }

    if (!selection.empty()) {
      selectedDatasets_p.clear();
      selectedDatasets_p = selection;
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
      os << "-- Selected dipoles ....... : " << selectedDipoles()       << endl;
    }
    
  }
  
  // ============================================================================
  //
  //  Parameter access - dipole dataset
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                sample_offset
  
#ifdef DAL_WITH_CASA
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
  
#ifdef DAL_WITH_CASA
  casa::Vector<casa::MPosition> TBB_StationGroup::antenna_position ()
  {
    uint n (0);
    std::map<std::string,iterDipoleDataset>::iterator it;
    casa::Vector<casa::MPosition> position (selectedDatasets_p.size());

    for (it=selectedDatasets_p.begin(); it!=selectedDatasets_p.end(); ++it) {
      position(n) = (it->second)->second.antenna_position();
      ++n;
    }
    
    return position;
  }
#endif
  
  //_____________________________________________________________________________
  //                                                                  dipoleNames
  
  std::vector<std::string> TBB_StationGroup::dipoleNames ()
  {
    std::vector<std::string> names (datasets_p.size());
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    unsigned int n (0);

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      names[n] = it->second.dipoleName();
      ++n;
    }

    return names;
  }

  //_____________________________________________________________________________
  //                                                                dipoleNumbers
  
  std::vector<int> TBB_StationGroup::dipoleNumbers ()
  {
    std::vector<int> numbers;
    std::map<std::string,iterDipoleDataset>::iterator it;

    for (it=selectedDatasets_p.begin(); it!=selectedDatasets_p.end(); ++it) {
      numbers.push_back((it->second)->second.dipoleNumber());
    }

    return numbers;
  }

  //_____________________________________________________________________________
  //                                                                   datasetIDs
  
  /*!
    \return dataset_ids -- Vector with a list of the identifiers to the
            HDF5 dataset objects within this station group.
  */
#ifdef DAL_WITH_CASA
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
  
#ifdef DAL_WITH_CASA
  
  //_____________________________________________________________________________
  //                                                                     readData
  
  /*!
    \retval data -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
  */
  bool TBB_StationGroup::readData (casa::Matrix<double> &data,
				   casa::Vector<int> const &start,
				   int const &nofSamples)
  {
    uint nofDipoles = selectedDatasets_p.size();
    uint nelem      = start.nelements();
    casa::IPosition shape (2,nofSamples,nofDipoles);

    // Check input parameters ______________________________

    if (nelem != nofDipoles) {
      std::cerr << "[TBB_StationGroup::readData]"
		<< " Wrong length of vector with start positions!"
		<< std::endl;
      std::cerr << " -- nof. selected dipoles   = " << nofDipoles << std::endl;
      std::cerr << " -- nof. elements in vector = " << nelem      << std::endl;
      //
      data.resize(shape);
      data = 0.0;
      return false;
    }

    // Check the shape of the matrix returning the values
    if (data.shape() != shape) {
      data.resize(shape);
    }

    // Retrieve data from file _____________________________
    
    bool status (true);
    uint n (0);
    casa::Vector<double> tmp (nofSamples);
    std::map<std::string,iterDipoleDataset>::iterator it;
    
    /* Iterate over the selected dipoles */
    for (it=selectedDatasets_p.begin(); it!=selectedDatasets_p.end(); ++it) {
      /* Retrieve dipole data */
      tmp = (it->second)->second.readData(start(n),nofSamples);
      /* Copy the data to the returned array */
      data.column(n) = tmp;
      /* Increment data array column counter */
      ++n;
    }

    // Feedback ____________________________________________

#ifdef DAL_DEBUGGING_MESSAGES
    std::cout << "[TBB_StationGroup::readData]" << std::endl;
    std::cout << " -- start            = " << start             << std::endl;
    std::cout << " -- selected dipoles = " << selectedDipoles() << std::endl;
    std::cout << " -- shape(data)      = " << data.shape()      << std::endl;
#endif

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     readData
  
  /*!
    \retval data -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
  */
  bool TBB_StationGroup::readData (casa::Matrix<double> &data,
				   int const &start,
				   int const &nofSamples)
  {
    uint nofDipoles (selectedDatasets_p.size());
    casa::Vector<int> startVect (nofDipoles,start);

    return readData (data,
	       startVect,
	       nofSamples);
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
                                 "BEAM_DIRECTION_VALUE",
                                 "BEAM_DIRECTION_UNIT",
                                 "BEAM_DIRECTION_FRAME");
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
	name = it->second.dipoleName();
	// retrieve the attributes for the dipole data-set as record
	it->second.getAttributes(recordDipole);
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
