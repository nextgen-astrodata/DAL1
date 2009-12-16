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

  /*!
    Default constructor does not connect to a file, but simply sets up internal
    parameters.
  */
  TBB_StationGroup::TBB_StationGroup ()
  {
    init ();
  }

  //_____________________________________________________________________________

  /*!
    \param filename -- Name of the HDF5 file within which the group is
           contained.
    \param group    -- Name of the group, in this case the full path from
           the base of the hierarchical structure within the HDF5 file.
  */
  TBB_StationGroup::TBB_StationGroup (std::string const &filename,
                                      std::string const &group)
  {
    init (filename,
          group);
  }

  //_____________________________________________________________________________

  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the group is placed.    \param group    -- Name of the group.
  */
  TBB_StationGroup::TBB_StationGroup (hid_t const &location,
                                      std::string const &group)
  {
    // Initialize internal variables
    groupID_p = 0;

    init (location,
          group);
  }

  //_____________________________________________________________________________

  /*!
    \param groupID -- Object identifier for the group within the HDF5 dataset
  */
  TBB_StationGroup::TBB_StationGroup (hid_t const &groupID)
  {
    init (groupID);
  }

  //_____________________________________________________________________________

  /*!
    \param other -- Another TBB_StationGroup object from which to create
           this new one.
  */
  TBB_StationGroup::TBB_StationGroup (TBB_StationGroup const &other)
  {
    // Initialize internal variables
    groupID_p = 0;

    copy (other);
  }

  // ----------------------------------------------------------------------- init

  void TBB_StationGroup::init ()
  {
    groupID_p              = 0;
    nofTriggeredAntennas_p = -1;
  }

  // ----------------------------------------------------------------------- init

  /*!
    \param dataset_id -- Identifier for the group within the HDF5 file
  */
  void TBB_StationGroup::init (hid_t const &group_id)
  {
    bool status (true);
    std::string filename;
    std::string group;

    //________________________________________________________________
    // Get filename and group name from the group ID

    status = DAL::h5get_filename (filename,
                                  group_id);

    if (status) {
      status = DAL::h5get_name (group,
				group_id);
    }
    else {
      std::cerr << "[TBB_StationGroup::init] Failed to get filename from group ID!"
		<< std::endl;
    }
    
    //________________________________________________________________
    // Debugging messages

#ifdef DEBUGGING_MESSAGES
    std::cout << "[TBB_StationGroup::init]" << std::endl;
    std::cout << "-- Filename   = " << filename << std::endl;
    std::cout << "-- Group name = " << group    << std::endl;
    std::cout << "-- Group ID   = " << group_id << std::endl;
#endif

    //________________________________________________________________
    // Forward the reverse engineered information to the init() function
    // to set up a new object identifier for the group in question.
    
    if (status) {
      init (filename,
	    group);
    }
    else {
      std::cerr << "[TBB_StationGroup::init] "
		<< "Incomplete information to initialize new object!"
		<< std::endl;
    }
    
  }
  
  // ----------------------------------------------------------------------- init
  
  /*!
    \param filename -- HDF5 file within which the dataset in question is contained
    \param group    -- Name of the group which this object is to encapsulate.
  */
  void TBB_StationGroup::init (std::string const &filename,
                               std::string const &group)
  {
    hid_t file_id (0);
    herr_t h5error (0);

    //________________________________________________________________
    // Open the HDF5 file containing the station group

    try {
      file_id = H5Fopen (filename.c_str(),
			 H5F_ACC_RDONLY,
			 H5P_DEFAULT);
    }
    catch (std::string message) {
      std::cerr << "[TBB_StationGroup::init] Error while opening HDF5 file: "
		<< message << std::endl;
    }
    
    //________________________________________________________________
    // If the file was opened successfully, open the station group
    
    if (file_id > 0) {
      init (file_id,
	    group);
      // release the global file handle and clear the error stack
      h5error = H5Fclose (file_id);
      h5error = H5Eclear1 ();
    }
    else {
      std::cerr << "[TBB_StationGroup::init] Error opening HDF5 file "
		<< filename
		<< " !"
		<< std::endl;
    }
    
  }

  // ----------------------------------------------------------------------- init

  /*!
    \param location -- Location below which the group is found within the file.
    \param group    -- Name of the group which this object is to encapsulate.
  */
  void TBB_StationGroup::init (hid_t const &location,
                               std::string const &group)
  {
    bool status (true);
    hid_t group_id (0);

    /*
      Try to open the group within the HDF5 file; the group is expected
      to reside below the object identified by "location".
    */
    try {
      group_id = H5Gopen (location,
			  group.c_str(),
			  H5P_DEFAULT);
    }
    catch (std::string message) {
      std::cerr << "[TBB_StationGroup::init] " << message << std::endl;
      status = false;
    }
    
    /* Set internal variables */

    if (group_id > 0) {
      groupID_p              = group_id;
#ifdef HAVE_CASA
      nofTriggeredAntennas_p = triggered_antennas().nelements();
#else
      nofTriggeredAntennas_p = triggered_antennas().size();
#endif
    }
    else {
      groupID_p = 0;
    }
    
    /* Set up the list of dipole datasets contained within this group */
    status = setDipoleDatasets ();
  }
  
  // ---------------------------------------------------------- setDipoleDatasets
  
  /*!
    Essentially there are two ways by which to construct the list of dipole
    datasets contained within this station group:
    <ol>
       <li>Test if the possible names for the datasets belong to a valid
       object.
       <li>Scan the elements within the station group to separate datasets from
       attached attributes.
    </ol>

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool TBB_StationGroup::setDipoleDatasets ()
  {
    /* Check minimal condition for operations below. */
    if (groupID_p < 1) {
      std::cerr << "[TBB_StationGroup::setDipoleDatasets]"
		<< " Unable to set dipole datasets; not connected to HDF5 group!"
		<< endl;
      return false;
    }
    
    //________________________________________________________________
    // Local variables.
    
    bool status (true);
    std::string nameDataset;
    hsize_t nofObjects (0);
    herr_t h5error (0);

    //________________________________________________________________
    // Obtain the number of objects contained within the station group

    h5error = H5Gget_num_objs(groupID_p,
                              &nofObjects);

    if (h5error > 0) {
      std::cerr << "[TBB_StationGroup::setDipoleDatasets]"
		<< " Error retrieving the number of objects attached to"
		<< " the station group!"
		<< std::endl;
      return false;
    } else if (nofObjects == 0) {
      std::cerr << "[TBB_StationGroup::setDipoleDatasets]"
		<< " No dipole datasets found attached to station group!"
		<< std::endl;
      return false;
    }
    
    //________________________________________________________________
    // Iterate through the objects within the station group and create
    // a new object for each dipole dataset
    
    try {
      // Iterate through the list of objects
      for (hsize_t idx (0); idx<nofObjects; idx++)
	{
	  // get the type of the object
	  if (H5G_DATASET == H5Gget_objtype_by_idx (groupID_p,idx))
	    {
	      // get the name of the dataset
	      status = DAL::h5get_name (nameDataset,
					groupID_p,
					idx);
	      // if name retrieval was successful, create new TBB_DipoleDataset
	      if (status) {
		datasets_p.push_back(DAL::TBB_DipoleDataset (groupID_p,
							     nameDataset));
	      }
	      else {
		std::cerr << "[TBB_StationGroup::setDipoleDatasets]"
			  << " Failed to open dataset!"
			  << std::endl;
	      }
	    }
	}
    }
    catch (std::string message) {
      std::cerr << "[TBB_StationGroup::setDipoleDatasets] "
		<< message
		<< std::endl;
      return false;
    }
    
    return true;
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  TBB_StationGroup::~TBB_StationGroup ()
  {
    destroy();
  }

  void TBB_StationGroup::destroy ()
  {
    if (datasets_p.size() > 0)
      {
        datasets_p.clear();
      }

    if (groupID_p > 0)
      {
        herr_t h5error (0);

        h5error = H5Gclose (groupID_p);
        h5error = H5Eclear1 ();
      }
  }

  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  // ------------------------------------------------------------------ operator=

  /*!
    \param other -- Another TBB_StationGroup object from which to make a
           copy.
  */
  TBB_StationGroup& TBB_StationGroup::operator= (TBB_StationGroup const &other)
  {
    if (this != &other)
      {
        destroy ();
        copy (other);
      }
    return *this;
  }

  // ----------------------------------------------------------------------- copy

  /*!
    \param other -- Another TBB_StationGroup object from which to create
           this new one.
  */
  void TBB_StationGroup::copy (TBB_StationGroup const &other)
  {
    if (other.groupID_p > 0) {
      try {
	init (other.groupID_p);
      }
      catch (std::string message) {
	std::cerr << "[TBB_StationGroup::copy] " << message << std::endl;
      }
    }
    else {
      /*
       * if previous operation failed, at least ensure internal handling remains
       * consistent
       */
      groupID_p = 0;
    }
  }
  
  // ============================================================================
  //
  //  Parameter access - station group
  //
  // ============================================================================

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

    status = DAL::h5get_name (name,groupID_p);

    if (status)
      {
        if (stripPath)
          {
            return name.substr(1);
          }
      }
    else
      {
        std::cerr <<  "[TBB_StationGroup::group_name] "
                  << "Error retrieving name of HDF5 group!"
                  << std::endl;
      }

    return name;
  }

  // ----------------------------------------------------- station_position_value

#ifdef HAVE_CASA
  casa::Vector<double> TBB_StationGroup::station_position_value ()
  {
    casa::Vector<double> val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::STATION_POSITION_VALUE),
                             val)) {
      return val;
    }
    else {
      return casa::Vector<double> (1);
    }
  }
#else
  std::vector<double> TBB_StationGroup::station_position_value ()
  {
    std::vector<double> val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::STATION_POSITION_VALUE),
                             val)) {
      return val;
    }
    else {
      return std::vector<double> (1);
    }
  }
#endif

  // ------------------------------------------------------ station_position_unit

#ifdef HAVE_CASA
  casa::Vector<casa::String> TBB_StationGroup::station_position_unit ()
  {
    casa::Vector<casa::String> val;

    if (DAL::h5get_attribute(groupID_p,
                             DAL::attribute_name(DAL::STATION_POSITION_UNIT),
                             val)) {
      return val;
    }
    else {
      return casa::Vector<casa::String> (1);
    }
  }
#else
  std::vector<std::string> TBB_StationGroup::station_position_unit ()
  {
    std::vector<std::string> val;

    if (DAL::h5get_attribute(groupID_p,
                             DAL::attribute_name(DAL::STATION_POSITION_UNIT),
                             val))
      {
        return val;
      }
    else
      {
        return std::vector<std::string> (1);
      }
  }
#endif

  // ----------------------------------------------------- station_position_frame

  std::string TBB_StationGroup::station_position_frame ()
  {
    std::string val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::STATION_POSITION_FRAME),
                             val)) {
      return val;
    }
    else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------------- station_position
  
#ifdef HAVE_CASA
  casa::MPosition TBB_StationGroup::station_position ()
  {
    return DAL::h5get_position (groupID_p,
                                DAL::STATION_POSITION_VALUE,
                                DAL::STATION_POSITION_UNIT,
                                DAL::STATION_POSITION_FRAME);
  }
#endif

  // --------------------------------------------------------------- trigger_type

  /*!
    \return trigger_type -- The trigger type which cause recording this data
  */
  std::string TBB_StationGroup::trigger_type ()
  {
    std::string val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::TRIGGER_TYPE),
                             val))
      {
        return val;
      }
    else
      {
        return std::string ("");
      }
  }

  // ------------------------------------------------------------- trigger_offset

  /*!
    \return trigger_offset -- The trigger offset.
  */
  double TBB_StationGroup::trigger_offset ()
  {
    double val (0);

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::TRIGGER_OFFSET),
                             val)) {
      return val;
    }
    else {
      return 0.0;
    }
  }
  
  // --------------------------------------------------------- triggered_antennas

  /*!
    \return triggered_antennas -- List of antennas, for which the trigger
            condition was fullfilled.
  */
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::triggered_antennas ()
  {
    casa::Vector<uint> val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::TRIGGERED_ANTENNAS),
                             val))
      {
        return val;
      }
    else
      {
        return casa::Vector<uint> (1,0);
      }
  }
#else
  std::vector<uint> TBB_StationGroup::triggered_antennas ()
  {
    std::vector<uint> val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::TRIGGERED_ANTENNAS),
                             val))
      {
        return val;
      }
    else
      {
        return std::vector<uint> (1,0);
      }
  }
#endif

  // ------------------------------------------------------- beam_direction_value

  /*!
    \return value -- The numerical value for the direction of the station
            beam.
  */
#ifdef HAVE_CASA
  casa::Vector<double> TBB_StationGroup::beam_direction_value ()
  {
    casa::Vector<double> val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::BEAM_DIRECTION_VALUE),
                             val)) {
      return val;
    }
    else {
      return casa::Vector<double> (1);
    }
  }
#else
  std::vector<double> TBB_StationGroup::beam_direction_value ()
  {
    std::vector<double> val;
    
    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::BEAM_DIRECTION_VALUE),
                             val)) {
      return val;
    }
    else {
      return std::vector<double> (1);
    }
  }
#endif
  
  
  // -------------------------------------------------------- beam_direction_unit
  
  /*!
    \return unit -- The physical units within which the beam direction
            is given; this might be <i>radian</i> or <i>degree</i>
  */
#ifdef HAVE_CASA
  casa::Vector<casa::String> TBB_StationGroup::beam_direction_unit ()
  {
    casa::Vector<casa::String> val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::BEAM_DIRECTION_UNIT),
                             val))
      {
        return val;
      }
    else
      {
        return casa::Vector<casa::String> (1);
      }
  }
#else
  std::vector<std::string> TBB_StationGroup::beam_direction_unit ()
  {
    std::vector<std::string> val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::BEAM_DIRECTION_UNIT),
                             val))
      {
        return val;
      }
    else
      {
        return std::vector<std::string> (1);
      }
  }
#endif

  // ------------------------------------------------------- beam_direction_frame

  std::string TBB_StationGroup::beam_direction_frame ()
  {
    std::string val;

    if (DAL::h5get_attribute(groupID_p,
                             attribute_name(DAL::BEAM_DIRECTION_FRAME),
                             val))
      {
        return val;
      }
    else
      {
        return std::string ("UNDEFINED");
      }
  }

  // -------------------------------------------------------------------- summary

  void TBB_StationGroup::summary (std::ostream &os)
  {
    os << "[TBB_StationGroup] Summary of object properties"     << endl;

    os << "-- Group ID  .............. : " << groupID_p           << endl;

    if (groupID_p > 0)
      {
        os << "-- Group name ............. : " << group_name(true)         << endl;
        os << "-- nof. dipole datasets ... : " << nofDipoleDatasets()      << endl;
        os << "-- Station position (Value) : " << station_position_value() << endl;
        os << "-- Station position (Unit)  : " << station_position_unit()  << endl;
        os << "-- Station position (Frame) : " << station_position_frame() << endl;
        os << "-- Beam direction (Value) . : " << beam_direction_value()   << endl;
        os << "-- Beam direction (Unit) .. : " << beam_direction_unit()    << endl;
        os << "-- Beam direction (Frame).. : " << beam_direction_frame()   << endl;
        os << "-- Trigger type ........... : " << trigger_type()           << endl;
        os << "-- Trigger offset ......... : " << trigger_offset()         << endl;
        os << "-- nof. triggered antennas  : " << nofTriggeredAntennas()   << endl;
        os << "-- Triggered antennas ..... : " << triggered_antennas()     << endl;
      }

  }

  // ============================================================================
  //
  //  Parameter access - dipole dataset
  //
  // ============================================================================

  // ----------------------------------------------------------------- station_id

#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::station_id ()
  {
    uint nofDatasets (datasets_p.size());
    uint id;
    casa::Vector<uint> station_id (nofDatasets);

    for (uint n(0); n<nofDatasets; n++) {
      datasets_p[n].getAttribute("STATION_ID",id);
      station_id(n) = id;
    }
    
    return station_id;
  }
#else
  std::vector<uint> TBB_StationGroup::station_id ()
  {
    std::vector<uint> station_id;
    
    for (unsigned int n(0); n<datasets_p.size(); n++) {
      station_id.push_back(datasets_p[n].station_id());
    }
    
    return station_id;
  }
#endif
  
  // --------------------------------------------------------------------- rsp_id
  
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::rsp_id ()
  {
    uint nofDatasets (datasets_p.size());
    uint id;
    casa::Vector<uint> rsp_id (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      datasets_p[n].getAttribute("RSP_ID",id);
      rsp_id(n) = id;
    }
    
    return rsp_id;
  }
#else
  std::vector<uint> TBB_StationGroup::rsp_id ()
  {
    std::vector<uint> rsp_id;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      rsp_id.push_back(datasets_p[n].rsp_id());
    }
    
    return rsp_id;
  }
#endif
  
  // --------------------------------------------------------------------- rcu_id
  
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::rcu_id ()
  {
    uint nofDatasets (datasets_p.size());
    uint id;
    casa::Vector<uint> rcu_id (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      datasets_p[n].getAttribute("RCU_ID",id);
      rcu_id(n) = id;
    }
    
    return rcu_id;
  }
#else
  std::vector<uint> TBB_StationGroup::rcu_id ()
  {
    std::vector<uint> rcu_id;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      rcu_id.push_back(datasets_p[n].rcu_id());
    }
    
    return rcu_id;
  }
#endif
  
  // ----------------------------------------------------- sample_frequency_value
  
#ifdef HAVE_CASA
  casa::Vector<double>
  TBB_StationGroup::sample_frequency_value ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<double> sample_frequency (nofDatasets);

    for (uint n(0); n<nofDatasets; n++)
      {
        sample_frequency(n) = datasets_p[n].sample_frequency_value();
      }

    return sample_frequency;
  }
#else
  std::vector<double> TBB_StationGroup::sample_frequency_value ()
  {
    std::vector<double> sample_frequency;

    for (uint n(0); n<datasets_p.size(); n++) {
      sample_frequency.push_back(datasets_p[n].sample_frequency_value());
    }
    
    return sample_frequency;
  }
#endif
  
  // ------------------------------------------------------ sample_frequency_unit
  
#ifdef HAVE_CASA
  casa::Vector<casa::String>
  TBB_StationGroup::sample_frequency_unit ()
  {
    std::string unit;
    uint nofDatasets (datasets_p.size());
    casa::Vector<casa::String> units (nofDatasets);

    for (uint n(0); n<nofDatasets; n++) {
      datasets_p[n].getAttribute("SAMPLE_FREQUENCY_UNIT",unit);
      units(n) = unit;
    }
    
    return units;
  }
#else
  std::vector<std::string> TBB_StationGroup::sample_frequency_unit ()
  {
    std::string unit;
    std::vector<std::string> units

    for (uint n(0); n<datasets_p.size(); n++) {
      datasets_p[n].getAttribute("SAMPLE_FREQUENCY_UNIT",unit);
      units.push_back(unit);
    }
    
    return units;
  }
#endif
  
    // --------------------------------------------------------------- nyquist_zone
    
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::nyquist_zone ()
  {
    uint nofDatasets (datasets_p.size());
    uint val;
    casa::Vector<uint> zone (nofDatasets);
    
    for (uint n(0); n<nofDatasets; ++n) {
      datasets_p[n].getAttribute("NYQUIST_ZONE",val);
      zone(n) = val;
    }
    
    return zone;
  }
#else
  std::vector<uint> TBB_StationGroup::nyquist_zone ()
  {
    uint nofDatasets (datasets_p.size());
    uint val;
    std::vector<uint> result;
    
    for (uint n(0); n<nofDatasets; n++) {
      datasets_p[n].getAttribute("NYQUIST_ZONE",val);
      result.push_back(val);
    }
    
    return result;
  }
#endif
  
  // ----------------------------------------------------------------------- time
  
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::time ()
  {
    uint nofDatasets (datasets_p.size());
    uint val;
    casa::Vector<uint> time (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      datasets_p[n].getAttribute("TIME",val);
      time(n) = val;
    }
    
    return time;
  }
#else
  std::vector<uint> TBB_StationGroup::time ()
  {
    std::vector<uint> time;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      time.push_back(datasets_p[n].time());
    }
    
    return time;
  }
#endif
  
  // -------------------------------------------------------------- sample_number

#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::sample_number ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<uint> sample_number (nofDatasets);

    for (uint n(0); n<nofDatasets; n++) {
      sample_number(n) = datasets_p[n].sample_number();
    }
    
    return sample_number;
  }
#else
  std::vector<uint> TBB_StationGroup::sample_number ()
  {
    std::vector<uint> sample_number;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      sample_number.push_back(datasets_p[n].sample_number());
    }
    
    return sample_number;
  }
#endif
  
  // ---------------------------------------------------------- samples_per_frame
  
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::samples_per_frame ()
  {
    uint nofDatasets (datasets_p.size());
    uint samples;
    casa::Vector<uint> samples_per_frame (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      datasets_p[n].getAttribute("SAMPLES_PER_FRAME",samples);
      samples_per_frame(n) = samples;
    }
    
    return samples_per_frame;
  }
#else
  std::vector<uint> TBB_StationGroup::samples_per_frame ()
  {
    std::vector<uint> samples_per_frame;

    for (uint n(0); n<datasets_p.size(); n++) {
      samples_per_frame.push_back(datasets_p[n].samples_per_frame());
    }
    
    return samples_per_frame;
  }
#endif
  
#ifdef HAVE_CASA
  casa::Vector<int> TBB_StationGroup::sample_offset (uint const &refAntenna)
  {
    uint nofDipoles              = nofDipoleDatasets();
    casa::Vector<uint> valTime   = time();
    casa::Vector<uint> valSample = sample_number();
    casa::Vector<int> offset (nofDipoles);

    for (uint n(0); n<nofDipoles; n++) {
      offset(n) = valTime(n)-valTime(refAntenna) + valSample(n)-valSample(refAntenna);
    }
    
    return offset;
  }
#else
  std::vector<int> TBB_StationGroup::sample_offset (uint const &refAntenna)
  {
    uint nofDipoles             = nofDipoleDatasets();
    std::vector<uint> valTime   = time();
    std::vector<uint> valSample = sample_number();
    std::vector<int> offset (nofDipoles);

    for (uint n(0); n<nofDipoles; n++) {
      offset[n] = valTime[n]-valTime[refAntenna] + valSample[n]-valSample[refAntenna];
    }
    
    return offset;
  }
#endif
  
  // --------------------------------------------------------------- data_length
  
  /*!
    \return data_lengths -- Values of the DATA_LENGTH attribute for all
            datasets present in this station group
  */
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::data_length ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<uint> data_length (nofDatasets);

    for (uint n(0); n<nofDatasets; n++) {
      data_length(n) = datasets_p[n].data_length();
    }
    
    return data_length;
  }
#else
  std::vector<uint> TBB_StationGroup::data_length ()
  {
    std::vector<uint> data_length;

    for (uint n(0); n<datasets_p.size(); n++) {
      data_length.push_back(datasets_p[n].data_length());
    }
    
    return data_length;
  }
#endif
  
  // ----------------------------------------------------------------------- feed

#ifdef HAVE_CASA
  casa::Vector<casa::String> TBB_StationGroup::feed ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<casa::String> feed (nofDatasets);

    for (uint n(0); n<nofDatasets; n++) {
      feed(n) = datasets_p[n].feed();
    }
    
    return feed;
  }
#else
  std::vector<std::string> TBB_StationGroup::feed ()
  {
    std::vector<std::string> feed;

    for (uint n(0); n<datasets_p.size(); n++) {
      feed.push_back(datasets_p[n].feed());
    }
    
    return feed;
  }
#endif
  
  // ----------------------------------------------------------- antenna_position
  
#ifdef HAVE_CASA
  casa::Vector<casa::MPosition> TBB_StationGroup::antenna_position ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<casa::MPosition> position (nofDatasets);

    for (uint n(0); n<nofDatasets; n++) {
      position(n) = datasets_p[n].antenna_position();
    }
    
    return position;
  }
#endif
  
  // ------------------------------------------------------------------ channelID

  /*!
    \return channelID -- A list of the channel IDs for all the dipoles within
            this LOFAR station.
  */
#ifdef HAVE_CASA
  casa::Vector<int> TBB_StationGroup::channelID ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<int> id (nofDatasets);

    for (uint n(0); n<nofDatasets; n++) {
      id(n) = datasets_p[n].channelID();
    }
    
    return id;
  }
#else
  std::vector<int> TBB_StationGroup::channelID ()
  {
    std::vector<int> id;

    for (uint n(0); n<datasets_p.size(); n++) {
        id.push_back(datasets_p[n].channelID());
    }
    
    return id;
  }
#endif
  
  // --------------------------------------------------------------- channelNames
  
  /*!
    \return channelNames -- A list of the channel names for all the dipoles
            within this LOFAR station.
  */
#ifdef HAVE_CASA
  casa::Vector<casa::String> TBB_StationGroup::channelNames ()
  {
    casa::Vector<casa::String> names (datasets_p.size());
    
    for (uint n(0); n<datasets_p.size(); n++) {
      names(n) = datasets_p[n].channelName();
    }
    
    return names;
  }
#else
  std::vector<std::string> TBB_StationGroup::channelNames ()
  {
    std::vector<std::string> names (datasets_p.size());
    
    for (uint n(0); n<datasets_p.size(); n++) {
      names[n] = datasets_p[n].channelName();
    }
    
    return names;
  }
#endif
  
  // ----------------------------------------------------------------- datasetIDs
  
  /*!
    \return dataset_ids -- Vector with a list of the identifiers to the
            HDF5 dataset objects within this station group.
  */
#ifdef HAVE_CASA
  casa::Vector<hid_t> TBB_StationGroup::datasetIDs ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<hid_t> dataset_ids (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      dataset_ids(n) = datasets_p[n].locationID();
    }
    
    return dataset_ids;
  }
#else
  std::vector<hid_t> TBB_StationGroup::datasetIDs ()
  {
    std::vector<hid_t> dataset_ids;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      dataset_ids.push_back(datasets_p[n].locationID());
    }
    
    return dataset_ids;
  }
#endif
  
  // ------------------------------------------------------------------------- fx
  
  /*!
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.

    \return fx -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
  */
#ifdef HAVE_CASA
  casa::Matrix<double> TBB_StationGroup::fx (int const &start,
					     int const &nofSamples)
  {
    /* Check minimal condition for operations below. */
    if (groupID_p < 1) {
      return casa::Matrix<double> (1,1,0);
    }
    
    casa::Matrix<double> data (nofSamples,datasets_p.size());
    casa::Vector<double> tmp (nofSamples);
    
#ifdef DEBUGGING_MESSAGES
    std::cout << "[TBB_StationGroup::fx]"             << std::endl;
    std::cout << "-- nof. samples = " << nofSamples   << std::endl;
    std::cout << "-- shape(data)  = " << data.shape() << std::endl;
    std::cout << "-- shape(tmp)   = " << tmp.shape()  << std::endl;
#endif
    
    for (uint n(0); n<datasets_p.size(); n++) {
      // get the channel data ...
      tmp = datasets_p[n].fx(start,nofSamples);
      // ... and add them to the returned array
      data.column(n) = tmp;
    }
    
    return data;
  }
#endif
  
  // ------------------------------------------------------------------------- fx

  /*!
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
    \param dipoleSelection -- Selection of dipoles, for which to retrieve the
           data.

    \return fx -- [nofSamples,dipole] Vector of raw ADC samples representing
            the electric field strength as function of time.
  */
#ifdef HAVE_CASA
  casa::Matrix<double> TBB_StationGroup::fx (int const &start,
      int const &nofSamples,
      std::vector<uint> const &dipoleSelection)
  {
    /* Check minimal condition for operations below. */
    if (groupID_p < 1) {
      return casa::Matrix<double> (1,1,0);
    }
    
    uint nofDipoles = dipoleSelection.size();
    casa::Matrix<double> data (nofSamples,nofDipoles);
    
    /* Go through the set of dipole datasets and retrieve the data */
    
    casa::Vector<double> tmp (nofSamples);
    
    for (uint n(0); n<nofDipoles; n++) {
      // get the channel data ...
      tmp = datasets_p[dipoleSelection[n]].fx(start,nofSamples);
      // ... and add them to the returned array
      data.column(n) = tmp;
    }
    
    return data;
  }
#endif
  
  // ============================================================================
  //
  //  Methods using casacore
  //
  // ============================================================================
  
#ifdef HAVE_CASA
  
  //_____________________________________________________________________________
  //                                                               beam_direction

  /*!
    \return direction -- The pointing direction of the station beam as a
            casa::Measure
  */
  casa::MDirection TBB_StationGroup::beam_direction ()
  {
    return DAL::h5get_direction (groupID_p,
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

    for (unsigned int n(0); n<nofDipoles; n++) {
      datasets_p[n].antenna_position_value(tmp);
      positionValues.row(n) = tmp;
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

    for (unsigned int n(0); n<nofDipoles; n++) {
      datasets_p[n].antenna_position_unit(tmp);
      antennaPositionUnits.row(n) = tmp;
    }
    
    return antennaPositionUnits;
  }
  
  //_____________________________________________________________________________
  //                                                             sample_frequency
  
  bool TBB_StationGroup::sample_frequency (casa::Vector<casa::MFrequency> &freq)
  {
    bool status (true);
    uint nofDatasets (datasets_p.size());
    
    freq.resize (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      status *= datasets_p[n].sample_frequency(freq(n));
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

    try {
      rec.define(casa::RecordFieldId(attribute_name(DAL::STATION_POSITION_VALUE)),
		 station_position_value());
      rec.define(casa::RecordFieldId(attribute_name(DAL::STATION_POSITION_UNIT)),
		 station_position_unit());
      rec.define(casa::RecordFieldId(attribute_name(DAL::STATION_POSITION_FRAME)),
		 station_position_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::BEAM_DIRECTION_VALUE)),
		 beam_direction_value());
      rec.define(casa::RecordFieldId(attribute_name(DAL::BEAM_DIRECTION_UNIT)),
		 beam_direction_unit());
      rec.define(casa::RecordFieldId(attribute_name(DAL::BEAM_DIRECTION_FRAME)),
		 beam_direction_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::TRIGGER_TYPE)),
		 trigger_type());
      rec.define(casa::RecordFieldId(attribute_name(DAL::TRIGGER_OFFSET)),
		 trigger_offset());
      rec.define(casa::RecordFieldId(attribute_name(DAL::TRIGGERED_ANTENNAS)),
		 triggered_antennas());
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
      for (uint n(0); n<datasets_p.size(); n++) {
	name = datasets_p[n].channelName();
	// retrieve the attributes for the dipole data-set as record
	recordDipole = datasets_p[n].attributes2record();
	// ... and add it to the existing record
	rec.defineRecord (name,recordDipole);
      }
    }
    
    return status;
  }

#endif
  
} // Namespace DAL -- end
