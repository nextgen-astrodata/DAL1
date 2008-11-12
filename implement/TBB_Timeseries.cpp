/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#include <TBB_Timeseries.h>

using std::cout;
using std::endl;

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  // ------------------------------------------------------------- TBB_Timeseries
  
  /*!
    A minimal setup of the internal dataspace is performed, but since no
    data file is provided, no inspection of the data structure is carried out.
  */
  TBB_Timeseries::TBB_Timeseries ()
  {
    init ();
  }

  // ------------------------------------------------------------- TBB_Timeseries
  
  /*!
    \param filename -- Name of the data file
  */
  TBB_Timeseries::TBB_Timeseries (std::string const &filename)
  {
    init ();
    init (filename);
  }
  
  // ------------------------------------------------------------- TBB_Timeseries
  
  /*!
    \param other -- Another TBB_Timeseries object from which to create this
           new one.
  */
  TBB_Timeseries::TBB_Timeseries (TBB_Timeseries const &other)
  {
    // Initialize internal variables
    init ();
    // copy the internal parameters of the other object
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  TBB_Timeseries::~TBB_Timeseries ()
  {
    destroy();
  }
  
  void TBB_Timeseries::destroy ()
  {
    if (fileID_p > 0) {
      herr_t h5error (0);
      // Close the HDF5 file
      h5error = H5Fclose (fileID_p);
    }
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  // ------------------------------------------------------------------ operator=
  
  TBB_Timeseries& TBB_Timeseries::operator= (TBB_Timeseries const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  // ----------------------------------------------------------------------- copy

  void TBB_Timeseries::copy (TBB_Timeseries const &other)
  {
    if (other.fileID_p < 0) {
      init ();
    } else {
      filename_p = other.filename_p;
      init (other.filename_p);
    }
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  // ------------------------------------------------------------------ telescope

  /*!
    \return telescope -- The name of the telescope with which the data were
            recorded; returns an empty string in case no keyword value could be
	    extracted.
  */
  std::string TBB_Timeseries::telescope ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(fileID_p,
			     attribute_name(DAL::TELESCOPE),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ------------------------------------------------------------------- observer
  
  std::string TBB_Timeseries::observer ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(fileID_p,
 			     attribute_name(DAL::OBSERVER),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // -------------------------------------------------------------------- project
  
  std::string TBB_Timeseries::project ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(fileID_p,
			     attribute_name(DAL::PROJECT),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }

  // ------------------------------------------------------------- observation_id
  
  std::string TBB_Timeseries::observation_id ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(fileID_p,
			     attribute_name(DAL::OBSERVATION_ID),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------------- observation_mode
  
  std::string TBB_Timeseries::observation_mode ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(fileID_p,
			     attribute_name(DAL::OBSERVATION_MODE),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // -------------------------------------------------------------------- summary
  
  void TBB_Timeseries::summary (std::ostream &os)
  {
    os << "[TBB_Timeseries] Summary of object properties"       << endl;
    os << "-- File name  ........  : " << filename_p            << endl;
    os << "-- File ID      ......  : " << fileID_p              << endl;

    /*
      Further output is conditional - only makes sense if successfully connected
      to a file
    */
    if (fileID_p > 0) {
      os << "-- Telescope            : " << telescope()         << endl;
      os << "-- Observer             : " << observer()          << endl;
      os << "-- Project              : " << project()           << endl;
      os << "-- Observation ID       : " << observation_id()    << endl;
      os << "-- Observation mode     : " << observation_mode()  << endl;
      os << "-- nof. station groups  : " << groups_p.size()     << endl;
      os << "-- nof. dipole datasets : " << nofDipoleDatasets() << endl;
    }
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  // ----------------------------------------------------------------------- init

  void TBB_Timeseries::init ()
  {
    filename_p = "UNDEFINED";
    fileID_p   = -1;
  }
  
  // ----------------------------------------------------------------------- init
  
  void TBB_Timeseries::init (std::string const &filename)
  {
    // try to open the HDF5 file
    fileID_p = H5Fopen (filename.c_str(),
			H5F_ACC_RDONLY,
			H5P_DEFAULT);
    assert (fileID_p);
    
    if (fileID_p > 0) {
      bool status (true);
      // Store the filename
      filename_p = filename;
      // locate and register the station groups
      try {
	status = setStationGroups ();
      } catch (std::string message) {
	std::cerr << "[TBB_Timeseries::init] Error running setStationGroups ()! "
		  << message 
		  << std::endl;
      }
      // feedback
      if (!status) {
	std::cerr << "[TBB_Timeseries::init] Error setting up the station groups!"
		  << std::endl;
      }
    } else {
      std::cerr << "[TBB_Timeseries::init] Error opening file "
		<< filename
		<< std::endl;
    }
  }

  // ----------------------------------------------------------- setStationGroups
  
  bool TBB_Timeseries::setStationGroups ()
  {
    /* Check minimal condition for operations below. */
    if (fileID_p < 1) {
      std::cerr << "[TBB_Timeseries::setStationGroups]"
		<< " Unable to set station groups; not connected to fileset."
		<< endl;
      return false;
    }
    
    //________________________________________________________________
    // Local variables.
    
    bool status (true);
    hsize_t nofObjects (0);
    herr_t h5error (0);
    std::string nameGroup;
    
    //________________________________________________________________
    // Obtain the number of objects attached to the root level of the file
    
    h5error = H5Gget_num_objs(fileID_p,
			      &nofObjects);
    
    if (h5error > 0) {
      std::cerr << "[TBB_Timeseries::setStationGroups]"
		<< " Error retrieving the number of objects attached to"
		<< " the root level of the file!"
		<< std::endl;
      return false;
    } else if (nofObjects == 0) {
      std::cerr << "[TBB_Timeseries::setStationGroups]"
		<< " No station groups found attached to root group!!"
		<< std::endl;
      return false;
    }

    //________________________________________________________________
    // Iterate through the list of objects attached to the root group
    // of the file
    
    try {
      // Iterate through the list of objects
      for (hsize_t idx (0); idx<nofObjects; idx++) {
	// get the type of the object
	if (H5G_GROUP == H5Gget_objtype_by_idx (fileID_p,idx)) {
	  // get the name of the dataset
	  status = DAL::h5get_name (nameGroup,
				    fileID_p,
				    idx);
	  // if name retrieval was successful, create new TBB_DipoleDataset
	  if (status) {
	    groups_p.push_back(DAL::TBB_StationGroup (fileID_p,
						      nameGroup));
	  } else {
	    std::cerr << "[TBB_Timeseries::setStationGroups]"
		      << " Failed to open station group!"
		      << std::endl;
	  }
	}
      }
    } catch (std::string message) {
      std::cerr << "[TBB_Timeseries::setStationGroups] " << message
		<< std::endl;
      return false;
    }
    
    return true;
  }
  
  //_____________________________________________________________________________
  // Get one of the embedded station group objects

  /*!
    \param station -- Number of the TBB_StationGroup object embedded within this
           TBB_Timeseries object.

    return stationGroup -- The requested station group object; if the provided
           index is outside the valid range, an empty StationGroup object is 
	   returned.
  */
  TBB_StationGroup TBB_Timeseries::stationGroup (uint const &station)
  {
    if (station > 0 && station < groups_p.size()) {
      return groups_p[station];
    } else {
      std::cout << "[TBB_Timeseries::stationGroup] Index for station group"
		<< " out of range!" << std::endl;
      return TBB_StationGroup();
    }
  }

  // ============================================================================
  //
  //  Access to attributes attached to the station groups
  //
  // ============================================================================

  // --------------------------------------------------------------- trigger_type

#ifdef HAVE_CASA
  casa::Vector<casa::String> TBB_Timeseries::trigger_type ()
  {
    uint nofStations = groups_p.size();
    casa::Vector<casa::String> trigger (nofStations);

    for (uint n(0); n<nofStations; n++) {
      trigger(n) = groups_p[n].trigger_type();
    }
    
    return trigger;
  }
#else
  std::vector<std::string> TBB_Timeseries::trigger_type ()
  {
    uint nofStations = groups_p.size();
    std::vector<std::string> trigger (nofStations);
    
    for (uint n(0); n<nofStations; n++) {
      trigger[n] = groups_p[n].trigger_type();
    }
    
    return trigger;
  }
#endif
  
  // ------------------------------------------------------------- trigger_offset
  
#ifdef HAVE_CASA
  casa::Vector<double> TBB_Timeseries::trigger_offset ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<double> trigger (nofStations);
    
    for (uint n(0); n<nofStations; n++) {
      trigger(n) = groups_p[n].trigger_offset();
    }
    
    return trigger;
  }
#else
  std::vector<double> TBB_Timeseries::trigger_offset ()
  {
    uint nofStations = nofStationGroups();
    std::vector<double> trigger (nofStations);
    
    for (uint n(0); n<nofStations; n++) {
      trigger[n] = groups_p[n].trigger_offset();
    }
    
    return trigger;
  }
#endif
  
#ifdef HAVE_CASA
  
  casa::Vector<casa::MPosition> TBB_Timeseries::station_position ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::MPosition> positions (nofStations);
    
    for (uint n(0); n<nofStations; n++) {
      positions(n) = groups_p[n].station_position();
    }
    
    return positions;
  }
  
  casa::Vector<casa::MDirection> TBB_Timeseries::beam_direction ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::MDirection> directions (nofStations);
    
    for (uint n(0); n<nofStations; n++) {
      directions(n) = groups_p[n].beam_direction();
    }
    
    return directions;
  }
  
#endif
  
  // ============================================================================
  //
  //  Access to attributes attached to the dipole datasets
  //
  // ============================================================================
  
  // -------------------------------------------------------------- channelNames
  
#ifdef HAVE_CASA
  casa::Vector<casa::String> TBB_Timeseries::channelNames ()
  {
    casa::Vector<casa::String> names;
    
    return names;
  }
#else
  std::vector<std::string> TBB_Timeseries::channelNames ()
  {
    std::vector<std::string> names;
    
    return names;
  }
#endif
  
  // ---------------------------------------------------------------- channelID
  
#ifdef HAVE_CASA
  casa::Vector<int> TBB_Timeseries::channelID ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    casa::Vector<int> channelIDvalues (nofDipoles);
    casa::Vector<int> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].channelID();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	channelIDvalues(n) = tmp(dipole);
	n++;
      }
    }
    
    return channelIDvalues;
  }
#else
  std::vector<uint> TBB_Timeseries::channelID ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    std::vector<int> channelIDvalues (nofDipoles);
    std::vector<int> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].channelID();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	channelIDvalues[n] = tmp[dipole];
	n++;
      }
    }
    
    return channelIDvalues;
  }
#endif

  // ---------------------------------------------------------------------- times

#ifdef HAVE_CASA
  casa::Vector<uint> TBB_Timeseries::time ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    casa::Vector<uint> UnixTimes (nofDipoles);
    casa::Vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].time();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	UnixTimes(n) = tmp(dipole);
	n++;
      }
    }
    
    return UnixTimes;
  }
#else
  std::vector<uint> TBB_Timeseries::time (std::string const &units)
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    std::vector<uint> UnixTimes (nofDipoles);
    std::vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].time();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	UnixTimes[n] = tmp[dipole];
	n++;
      }
    }
    
    return UnixTimes;
  }
#endif

  // --------------------------------------------------------- sample_frequency

#ifdef HAVE_CASA
  casa::Vector<double> TBB_Timeseries::sample_frequency ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    casa::Vector<double> sampleFrequency (nofDipoles);
    casa::Vector<double> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp = groups_p[station].sample_frequency_value();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	sampleFrequency(n) = tmp(dipole);
	n++;
      }
    }
    
    return sampleFrequency;
  }
#else
  std::vector<double> TBB_Timeseries::sample_frequency ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    std::vector<double> sampleFrequency (nofDipoles);
    std::vector<double> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp = groups_p[station].sample_frequency_value();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	sampleFrequency[n] = tmp[dipole];
	n++;
      }
    }
    
    return sampleFrequency;
  }
#endif

  // ---------------------------------------------------------------- data_length

#ifdef HAVE_CASA
  casa::Vector<uint> TBB_Timeseries::data_length ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    casa::Vector<uint> dataLengths (nofDipoles);
    casa::Vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].data_length();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	dataLengths(n) = tmp(dipole);
	n++;
      }
    }
    
    return dataLengths;
  }
#else
  std::vector<uint> TBB_Timeseries::data_length ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    std::vector<uint> dataLengths (nofDipoles);
    std::vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].data_length();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	dataLengths[n] = tmp[dipole];
	n++;
      }
    }
    
    return dataLengths;
  }
#endif

  // ---------------------------------------------------------- attributes2record

  /*!
    \param recursive    -- Recursively add information from the station groups
           and dipole datasets? If set to \e true the file structure will be 
	   transverse recursively, collecting the attributes attached to the 
	   embedded objects.
	   
    \return rec -- casa::Record to which the values of the attributes will be
            added as new entries.
  */
#ifdef HAVE_CASA
  casa::Record TBB_Timeseries::attributes2record (bool const &recursive)
  {
    casa::Record rec;
    
    try {
      rec.define(casa::RecordFieldId(attribute_name(DAL::TELESCOPE)),
		 telescope());
      rec.define(casa::RecordFieldId(attribute_name(DAL::OBSERVER)),
		 observer());
      rec.define(casa::RecordFieldId(attribute_name(DAL::PROJECT)),
		 project());
      rec.define(casa::RecordFieldId(attribute_name(DAL::OBSERVATION_ID)),
		 observation_id());
      rec.define(casa::RecordFieldId(attribute_name(DAL::OBSERVATION_MODE)),
		 observation_mode());
    } catch (std::string message) {
      std::cerr << "[TBB_Timeseries::attributes2record]" << message << std::endl;
    }
    
    //________________________________________________________________
    // Recursive retrieval of the attributes attached to the station
    // groups and dipole datasets
    
    if (recursive) {
      std::string group;
      casa::Record recordStation;
      for (uint n(0); n<groups_p.size(); n++) {
	group = groups_p[n].group_name(true);
	// retrieve the attributes for the dipole data-set as record
	recordStation = groups_p[n].attributes2record(recursive);
	// ... and add it to the existing record
	rec.defineRecord (group,recordStation);
      }
    }
    
    return rec;
  }
#endif
  
  // ---------------------------------------------------- attributes2headerRecord

  /*!  
    \return rec -- Create record entries conforming to the information expected
            in the header record of the CR::DataReader class.
  */
#ifdef HAVE_CASA
  casa::Record TBB_Timeseries::attributes2headerRecord ()
  {
    casa::Record rec;
    
    rec.define("Date",min(time()));
    rec.define("AntennaIDs",channelID());
    rec.define("Observatory",telescope());
    rec.define("Filesize",min(data_length()));
    
    return rec;
  }
#endif
  
  // ------------------------------------------------------------------------- fx
  
  casa::Matrix<double> TBB_Timeseries::fx (int const &start,
					   int const &nofSamples)
  {
    /* Check minimal condition for operations below. */
    if (fileID_p < 1) {
      return casa::Matrix<double> (1,1,0);
    }
    
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofStations = groups_p.size();
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<uint> selection (1);
    casa::Matrix<double> data (nofSamples,nofDipoles);
    casa::Vector<double> tmp (nofSamples);
    
#ifdef DEBUGGING_MESSAGES
    std::cout << "[TBB_Timeseries::fx]" << std::endl;
    std::cout << "-- nof. dipoles  = " << nofDipoles   << std::endl;
    std::cout << "-- nof. stations = " << nofStations  << std::endl;
    std::cout << "-- shape(data)   = " << data.shape() << std::endl;
    std::cout << "-- shape(tmp)    = " << tmp.shape()  << std::endl;
#endif

    for (station=0; station<nofStations; station++) {
      // get the number dipoles 
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	selection[0] = dipole;
	// get the channel data ...
	tmp = groups_p[station].fx(start,
				   nofSamples,
				   selection);
	// ... and add them to the returned array
	data.column(n) = tmp;
	// increment the counter for the number of dipoles
	n++;
      }
    }
    
    return data;
    
  }
  
} // Namespace DAL -- end
