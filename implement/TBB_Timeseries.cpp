/*-------------------------------------------------------------------------*
 | $Id::                                                               $ |
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
  
  TBB_Timeseries::TBB_Timeseries ()
  {
    init ();
  }

  // ------------------------------------------------------------- TBB_Timeseries
  
  TBB_Timeseries::TBB_Timeseries (std::string const &filename)
  {
    init ();
    init (filename);
  }
  
  // ------------------------------------------------------------- TBB_Timeseries
  
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
  { ; }
  
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
      init (other.filename_p);
    }
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  // ------------------------------------------------------------------ telescope

  std::string TBB_Timeseries::telescope ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::TELESCOPE),
			     fileID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }

  // ------------------------------------------------------------------- observer
  
  std::string TBB_Timeseries::observer ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::OBSERVER),
			     fileID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }

  // -------------------------------------------------------------------- project

  std::string TBB_Timeseries::project ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::PROJECT),
			     fileID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }

  // ------------------------------------------------------------- observation_id
  
  std::string TBB_Timeseries::observation_id ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::OBSERVATION_ID),
			     fileID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------------- observation_mode
  
  std::string TBB_Timeseries::observation_mode ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::OBSERVATION_MODE),
			     fileID_p)) {
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
    
    if (fileID_p > 0) {
      /*
       * Store the filename
       */
      filename_p = filename;
      // locate and register the station groups
      bool status = setStationGroups ();
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
    if (fileID_p < 0) {
      std::cerr << "[TBB_Timeseries::setStationGroups]"
		<< " Unable to set station groups; not connected to fileset."
		<< endl;
      return false;
    }
    
    /* Local variables. */
    
    bool status        = true;
    herr_t h5error     = 0;
    hsize_t nofObjects = 0;
    std::string name;

    try {
      // get the number of objects attached to the root level of the file
      h5error = H5Gget_num_objs(fileID_p,
				&nofObjects);
      if (nofObjects > 0) {
	// go through the list of objects to identify the station groups
	for (hsize_t n(0); n<nofObjects; n++) {
	  // check type of object
	  if (H5G_GROUP == H5Gget_objtype_by_idx (fileID_p,n)) {
	    // get the name of the group
	    status = DAL::h5get_name (name,
				      fileID_p,
				      n);
	    /*
	     * If retrieval of group name was successful, create new
	     * TBB_StationGroup object to be added to the internal list.
	     */
	    if (status) {
	      groups_p.push_back(DAL::TBB_StationGroup(filename_p,
							 name));
	    } else {
	      std::cerr << "[TBB_Timeseries::setStationGroups]"
			<< " Error retrieving group name!"
			<< std::endl;
	    }
	  }
	}
      } else {
	std::cout << "[TBB_Timeseries::setStationGroups]"
		  << " No objects found at root level of the file!" << std::endl;
      }
    } catch (std::string message) {
      std::cerr << "[TBB_Timeseries::setStationGroups] " << message
		<< std::endl;
      return false;
    }
    
    return true;
  }
  
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
  casa::Vector<uint> TBB_Timeseries::times ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    casa::Vector<uint> UnixTimes (nofDipoles);
    casa::Vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].times();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	UnixTimes(n) = tmp(dipole);
	n++;
      }
    }
    
    return UnixTimes;
  }
#else
  std::vector<uint> TBB_Timeseries::times (std::string const &units)
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    std::vector<uint> UnixTimes (nofDipoles);
    std::vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].times();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	UnixTimes[n] = tmp[dipole];
	n++;
      }
    }
    
    return UnixTimes;
  }
#endif

  // --------------------------------------------------------- sample_frequencies

#ifdef HAVE_CASA
  casa::Vector<double> TBB_Timeseries::sample_frequencies (std::string const &units)
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    casa::Vector<double> sampleFrequencies (nofDipoles);
    casa::Vector<double> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp = groups_p[station].sample_frequencies(units);
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	sampleFrequencies(n) = tmp(dipole);
	n++;
      }
    }
    
    return sampleFrequencies;
  }
#else
  std::vector<double> TBB_Timeseries::sample_frequencies (std::string const &units)
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    std::vector<double> sampleFrequencies (nofDipoles);
    std::vector<double> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp = groups_p[station].sample_frequencies(units);
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	sampleFrequencies[n] = tmp[dipole];
	n++;
      }
    }
    
    return sampleFrequencies;
  }
#endif

  // ----------------------------------------------------------------- data_lengths

#ifdef HAVE_CASA
  casa::Vector<uint> TBB_Timeseries::data_lengths ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    casa::Vector<uint> dataLengths (nofDipoles);
    casa::Vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].data_lengths();
      nofDipoles = groups_p[station].nofDipoleDatasets();
      for (dipole=0; dipole<nofDipoles; dipole++) {
	dataLengths(n) = tmp(dipole);
	n++;
      }
    }
    
    return dataLengths;
  }
#else
  std::vector<uint> TBB_Timeseries::data_lengths ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    uint nofStations = nofStationGroups();
    std::vector<uint> dataLengths (nofDipoles);
    std::vector<uint> tmp;
    
    for (station=0; station<nofStations; station++) {
      tmp        = groups_p[station].data_lengths();
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

  casa::Record TBB_Timeseries::attributes2record (bool const &addRecursive)
  {
    casa::Record rec;
    
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

    /* Recursive adding of embbedded data? */

    if (addRecursive) {
      casa::Record recordStation;
      for (uint n(0); n<groups_p.size(); n++) {
	// retrieve the attributes for the dipole data-set as record
	recordStation = groups_p[n].attributes2record(addRecursive);
	// ... and add it to the existing record
	rec.defineRecord (groups_p[n].group_name(true),recordStation);
      }
    }
    
    return rec;
  }

  // ---------------------------------------------------- attributes2headerRecord

  casa::Record TBB_Timeseries::attributes2headerRecord ()
  {
    casa::Record rec;
    
    attributes2headerRecord (rec);
    
    return rec;
  }

  // ---------------------------------------------------- attributes2headerRecord
  
  void TBB_Timeseries::attributes2headerRecord (casa::Record &rec)
  {
    try {
      rec.define("Date",min(times()));
      rec.define("AntennaIDs",channelID());
      rec.define("Observatory",telescope());
      rec.define("Filesize",min(data_lengths()));
    } catch (std::string message) {
      std::cerr << "[TBB_Timeseries::attributes2headerRecord] " << message
		<< std::endl;      
    }
  }

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
