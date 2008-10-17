/*-------------------------------------------------------------------------*
 | $Id::                                                               $ |
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

#include <casa/Arrays/ArrayIO.h>

#include <Enumerations.h>
#include <TBB_StationGroup.h>

using std::cout;
using std::endl;

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  // ----------------------------------------------------------- TBB_StationGroup
  
  TBB_StationGroup::TBB_StationGroup ()
  {
    init ();
  }
  
  // ----------------------------------------------------------- TBB_StationGroup
  
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
  
  // ----------------------------------------------------------- TBB_StationGroup
  
  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the group is placed.
    \param group    -- Name of the group.
  */
  TBB_StationGroup::TBB_StationGroup (hid_t const &location,
				      std::string const &group)
  {
    // Initialize internal variables
    groupID_p = 0;
    
    init (location,
	  group);
  }
  
  // ----------------------------------------------------------- TBB_StationGroup
  
  TBB_StationGroup::TBB_StationGroup (hid_t const &group_id)
  {
    init (group_id);
  }
  
  // ----------------------------------------------------------- TBB_StationGroup
  
  TBB_StationGroup::TBB_StationGroup (TBB_StationGroup const &other)
  {
    // Initialize internal variables
    groupID_p = 0;
    
    copy (other);
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
    herr_t h5error (0);
    
    if (groupID_p > 0) {
      h5error = H5Gclose (groupID_p);
    }
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  // ------------------------------------------------------------------ operator=
  
  TBB_StationGroup& TBB_StationGroup::operator= (TBB_StationGroup const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  // ----------------------------------------------------------------------- copy
  
  void TBB_StationGroup::copy (TBB_StationGroup const &other)
  {
    if (other.groupID_p > 0) {
      init (other.groupID_p);
    } else {
      /*
       * if previous operation failed, at least ensure internal handling remains
       * consistent
       */
      groupID_p = 0;
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  std::string TBB_StationGroup::group_name (bool const &stripPath)
  {
    bool status (true);
    std::string name ("UNDEFINED");

    status = DAL::h5get_name (name,groupID_p);

    if (status) {
      if (stripPath) {
	return name.substr(1);
      }
    } else {
      std::cerr <<  "[TBB_StationGroup::group_name] "
		<< "Error retrieving name of HDF5 group!"
		<< std::endl;
    }

    return name;
  }
  
  // ----------------------------------------------------- station_position_value
  
  casa::Vector<double> TBB_StationGroup::station_position_value ()
  {
    casa::Vector<double> val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::STATION_POSITION_VALUE),
			     groupID_p)) {
      return val;
    } else {
      return casa::Vector<double> (1);
    }
  }
  
  // ------------------------------------------------------ station_position_unit
  
  std::string TBB_StationGroup::station_position_unit ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     DAL::attribute_name(DAL::STATION_POSITION_UNIT),
			     groupID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------- station_position_frame
  
  std::string TBB_StationGroup::station_position_frame ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::STATION_POSITION_FRAME),
			     groupID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------------- station_position
  
  casa::MPosition TBB_StationGroup::station_position ()
  {
    return DAL::h5get_position (DAL::STATION_POSITION_VALUE,
				DAL::STATION_POSITION_UNIT,
				DAL::STATION_POSITION_FRAME,
				groupID_p);
  }
  
  // ------------------------------------------------------- beam_direction_value
  
  casa::Vector<double> TBB_StationGroup::beam_direction_value ()
  {
    casa::Vector<double> val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::BEAM_DIRECTION_VALUE),
			     groupID_p)) {
      return val;
    } else {
      return casa::Vector<double> (1);
    }
  }
  
  
  // -------------------------------------------------------- beam_direction_unit
  
  std::string TBB_StationGroup::beam_direction_unit ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::BEAM_DIRECTION_UNIT),
			     groupID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ------------------------------------------------------- beam_direction_frame
  
  std::string TBB_StationGroup::beam_direction_frame ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::BEAM_DIRECTION_FRAME),
			     groupID_p)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ------------------------------------------------------------- beam_direction
  
#ifdef HAVE_CASA
  casa::MDirection TBB_StationGroup::beam_direction ()
  {
    return DAL::h5get_direction (DAL::BEAM_DIRECTION_VALUE,
				 DAL::BEAM_DIRECTION_UNIT,
				 DAL::BEAM_DIRECTION_FRAME,
				 groupID_p);
  }
#endif
  
  // --------------------------------------------------------------- trigger_type
  
  std::string TBB_StationGroup::trigger_type ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::TRIGGER_TYPE),
			     groupID_p)) {
      return val;
    } else {
      return std::string ("");
    }
  }
  
  // ------------------------------------------------------------- trigger_offset
  
  double TBB_StationGroup::trigger_offset ()
  {
    double val (0);
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::TRIGGER_OFFSET),
			     groupID_p)) {
      return val;
    } else {
      return 0.0;
    }
  }
  
  // --------------------------------------------------------- triggered_antennas
  
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::triggered_antennas ()
  {
    casa::Vector<uint> val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::TRIGGERED_ANTENNAS),
			     groupID_p)) {
      return val;
    } else {
      return casa::Vector<uint> (1,0);
    }
  }
#else 
  std::vector<uint> TBB_StationGroup::triggered_antennas ()
  {
    std::vector<uint> val;
    
    if (DAL::h5get_attribute(val,
			     attribute_name(DAL::TRIGGERED_ANTENNAS),
			     groupID_p)) {
      return val;
    } else {
      return std::vector<uint> (1,0);
    }
  }
#endif
  
  // -------------------------------------------------------------------- summary
  
  void TBB_StationGroup::summary (std::ostream &os)
  {
    os << "[TBB_StationGroup] Summary of object properties"     << endl;
    
    os << "-- Group ID  .............. : " << groupID_p           << endl;

    if (groupID_p > 0) {
      os << "-- Group name ............. : " << group_name(true)         << endl;
      os << "-- nof. dipole datasets ... : " << nofDipoleDatasets()      << endl;
      os << "-- Station position (Unit)  : " << station_position_unit()  << endl;
      os << "-- Station position (Frame) : " << station_position_frame() << endl;
      os << "-- Beam direction (Unit) .. : " << beam_direction_unit()    << endl;
      os << "-- Beam direction (Frame).. : " << beam_direction_frame()   << endl;
      os << "-- Trigger type ........... : " << trigger_type()           << endl;
      os << "-- Trigger offset ......... : " << trigger_offset()         << endl;
    }
    
#ifdef HAVE_CASA
    if (groupID_p > 0) {
      os << "-- Triggered antennas  : " << triggered_antennas()     << endl;
    }
#endif
  }

  // ============================================================================
  //
  //  Parameters from dipole datasets
  //
  // ============================================================================
  
  // ----------------------------------------------------------------- station_id

#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::station_id ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<uint> station_id (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      station_id(n) = datasets_p[n].station_id();
    }
    
    return station_id;
  }
#else
  std::vector<uint> TBB_StationGroup::station_id ()
  {
    std::vector<double> station_id;
    
    for (uint n(0); n<datasets_p.size(); n++) {
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
    casa::Vector<uint> rsp_id (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      rsp_id(n) = datasets_p[n].rsp_id();
    }
    
    return rsp_id;
  }
#else
  std::vector<uint> TBB_StationGroup::rsp_id ()
  {
    std::vector<double> rsp_id;
    
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
    casa::Vector<uint> rcu_id (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      rcu_id(n) = datasets_p[n].rcu_id();
    }
    
    return rcu_id;
  }
#else
  std::vector<uint> TBB_StationGroup::rcu_id ()
  {
    std::vector<double> rcu_id;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      rcu_id.push_back(datasets_p[n].rcu_id());
    }
    
    return rcu_id;
  }
#endif
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  // ----------------------------------------------------------------------- init
  
  void TBB_StationGroup::init ()
  {
    groupID_p = 0;
  }
  
  // ----------------------------------------------------------------------- init
  
  void TBB_StationGroup::init (hid_t const &group_id)
  {
    bool status (true);
    std::string filename;
    std::string group;
    
    //  group ID -> file name
    status = DAL::h5get_filename (filename,
				  group_id);
    //  group ID -> group name
    if (status) {
      status = DAL::h5get_name (group,
				group_id);
    }
    /*
     * Forward the reverse engineered information to the init() function to 
     * set up a new object identifier for the group in question.
     */
    if (status) {
      init (filename,
	    group);
    }
  }
  
  // ----------------------------------------------------------------------- init
  
  void TBB_StationGroup::init (std::string const &filename,
				 std::string const &group)
  {
    hid_t file_id (0);
    herr_t h5error (0);
    
    // Initialize internal variables
    groupID_p = 0;
    
    // open the file
    file_id = H5Fopen (filename.c_str(),
		       H5F_ACC_RDONLY,
		       H5P_DEFAULT);
    
    // if opening of file was successfull, try to open group
    if (file_id > 0) {
      init (file_id,
	    group);
    } else {
      std::cerr << "[TBB_StationGroup::init] Error opening HDF5 file "
		<< filename 
		<< " !"
		<< std::endl;
    }
    
    // release the global file handle and clear the error stack
    if (file_id > 0) {
      h5error = H5Fclose (file_id);
      h5error = H5Eclear1 ();
    }
  }
  
  // ----------------------------------------------------------------------- init
  
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
      group_id = H5Gopen1 (location,
			   group.c_str());
    } catch (std::string message) {
      std::cerr << "[TBB_StationGroup::init] " << message << std::endl;
      status = false;
    }
    
    if (group_id > 0) {
      groupID_p = group_id;
    } else {
      groupID_p = 0;
    }
    
    /* Set up the list of dipole datasets contained within this group */
    status = setDipoleDatasets ();    
  }
  
  // ---------------------------------------------------------- setDipoleDatasets
  
  bool TBB_StationGroup::setDipoleDatasets ()
  {
    /* Check minimal condition for operations below. */
    if (groupID_p < 1) {
      return false;
    }
    
    /* Local variables. */
    
    bool status (true);
    std::string datasetName;
    hsize_t nofObjects (0);
    herr_t h5error (0);
    
    try {
      // Number of objects in the group specified by its identifier
      h5error = H5Gget_num_objs(groupID_p,
				&nofObjects);
      // go throught the list of objects to identify the datasets
      for (hsize_t idx (0); idx<nofObjects; idx++) {
	// get the type of the object
	if (H5G_DATASET == H5Gget_objtype_by_idx (groupID_p,idx)) {
	  // get the name of the dataset
	  status = DAL::h5get_name (datasetName,
				    groupID_p,
				    idx);
	  // if name retrieval was successful, create new TBB_DipoleDataset
	  if (status) {
	    datasets_p.push_back(DAL::TBB_DipoleDataset (groupID_p,
							   datasetName));
	  }
	}
      }
    } catch (std::string message) {
      std::cerr << "[TBB_StationGroup::setDipoleDatasets] "
		<< message
		<< std::endl;
      return false;
    }
    
    return true;
  }
  
  // ---------------------------------------------------------------- channelID
  
  std::vector<int> TBB_StationGroup::channelID ()
  {
    std::vector<int> channel_ids;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      channel_ids.push_back(datasets_p[n].channelID());
    }
    
    return channel_ids;
  }

  // ------------------------------------------------------ antennaPositionValues

#ifdef HAVE_CASA
  casa::Matrix<double> TBB_StationGroup::antennaPositionValues ()
  {
    unsigned int nofDipoles = datasets_p.size();
    casa::Matrix<double> positionValues (nofDipoles,3);

    for (unsigned int n(0); n<nofDipoles; n++) {
      positionValues.row(n) = datasets_p[n].antenna_position_value();
    }

    return positionValues;
  }
#endif
  
  // --------------------------------------------------------------- channelNames
  
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
  
#ifdef HAVE_CASA
  casa::Vector<hid_t> TBB_StationGroup::datasetIDs ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<hid_t> dataset_ids (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      dataset_ids(n) = datasets_p[n].dataset_id();
    }
    
    return dataset_ids;
  }
#else
  std::vector<hid_t> TBB_StationGroup::datasetIDs ()
  {
    std::vector<hid_t> dataset_ids;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      dataset_ids.push_back(datasets_p[n].dataset_id());
    }
    
    return dataset_ids;
  }
#endif
  
  // ---------------------------------------------------------------------- times
  
#ifdef HAVE_CASA
  casa::Vector<uint> TBB_StationGroup::times ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<uint> time (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      time(n) = datasets_p[n].time();
    }
    
    return time;
  }
#else
  std::vector<uint> TBB_StationGroup::times ()
  {
    std::vector<uint> time;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      time.push_back(datasets_p[n].time());
    }
    
    return time;
  }
#endif
  
  // ----------------------------------------------------------- sample_frequency
  
#ifdef HAVE_CASA
  casa::Vector<double>
  TBB_StationGroup::sample_frequency ()
  {
    uint nofDatasets (datasets_p.size());
    casa::Vector<double> sample_frequency (nofDatasets);
    
    for (uint n(0); n<nofDatasets; n++) {
      sample_frequency(n) = datasets_p[n].sample_frequency_value();
    }
    
    return sample_frequency;
  }
#else
  std::vector<double> TBB_StationGroup::sample_frequency ()
  {
    std::vector<double> sample_frequency;
    
    for (uint n(0); n<datasets_p.size(); n++) {
      sample_frequency.push_back(datasets_p[n].sample_frequency());
    }
    
    return sample_frequency;
  }
#endif
  
  // --------------------------------------------------------------- data_length
  
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
  
  // ------------------------------------------------------------------------- fx
  
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
  
  // ------------------------------------------------------------------------- fx
  
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
  
  // ---------------------------------------------------------- attributes2record
  
  /*!
    \param addRecursive -- Recursively add information from embedded HDF5
           objects, such as groups and datasets? 
    
    \return record -- A casa::Record container holding the values of the 
            attributes attached to the dataset for this dipole
  */
  casa::Record TBB_StationGroup::attributes2record (bool const &addRecursive)
  {
    casa::Record rec;
    
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
    } catch (std::string message) {
      std::cerr << "[TBB_StationGroup::attributes2record] "
		<< "Error filling the record with attribute values!\n"
		<< message
		<< std::endl;
    }

    /* Recursive adding of embbedded data? */

    if (addRecursive) {
      casa::Record recordDipole;
      for (uint n(0); n<datasets_p.size(); n++) {
	// retrieve the attributes for the dipole data-set as record
	recordDipole = datasets_p[n].attributes2record();
	// ... and add it to the existing record
	rec.defineRecord (datasets_p[n].channelName(),recordDipole);
      }
    }
    
    return rec;
  }

} // Namespace DAL -- end
