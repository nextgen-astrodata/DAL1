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
    init ();
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
                                      std::string const &group,
				      bool const &create)
  {
    open (location, group, create);
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

  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
  /*!
    \param other -- Another TBB_StationGroup object from which to create
           this new one.
  */
  TBB_StationGroup::TBB_StationGroup (TBB_StationGroup const &other)
    : CommonInterface()
  {
    // Initialize internal variables
    location_p = 0;

    copy (other);
  }

  //_____________________________________________________________________________
  //                                                                         init
  
  void TBB_StationGroup::init ()
  {
    location_p              = 0;
    nofTriggeredAntennas_p = -1;
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
    H5I_type_t objectType = H5Iget_type(location);
    std::set<std::string> groups;

    /* Set up the list of attributes attached to the root group */
    setAttributes();
    /* Get the list of groups attached to this group */
    h5get_names (groups,location,H5G_GROUP);

    if (static_cast<bool>(groups.count(name))) {
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
	for (it=names.begin(); it!=names.end(); ++it) {
	  datasets_p[*it] = TBB_DipoleDataset(location_p,*it);
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
  
  // ----------------------------------------------------- station_position_value
  
#ifdef HAVE_CASA
  casa::Vector<double> TBB_StationGroup::station_position_value ()
  {
    casa::Vector<double> val;

    if (DAL::h5get_attribute(location_p,
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

    if (DAL::h5get_attribute(location_p,
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

    if (DAL::h5get_attribute(location_p,
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

    if (DAL::h5get_attribute(location_p,
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

  // ----------------------------------------------------------- station_position
  
#ifdef HAVE_CASA
  casa::MPosition TBB_StationGroup::station_position ()
  {
    return DAL::h5get_position (location_p,
                                DAL::STATION_POSITION_VALUE,
                                DAL::STATION_POSITION_UNIT,
                                DAL::STATION_POSITION_FRAME);
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

    if (DAL::h5get_attribute(location_p,
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
    
    if (DAL::h5get_attribute(location_p,
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

    if (DAL::h5get_attribute(location_p,
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

    if (DAL::h5get_attribute(location_p,
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

  // -------------------------------------------------------------------- summary

  void TBB_StationGroup::summary (std::ostream &os)
  {
    os << "[TBB_StationGroup] Summary of object properties"     << endl;

    os << "-- Group ID  .............. : " << location_p           << endl;

    if (location_p > 0) {
      std::string triggerType;
      double triggerOffset;
      std::vector<uint> triggeredAntennas;
      std::string stationPositionFrame;
      std::string beamDirectionFrame;
      //
      getAttribute ("TRIGGER_TYPE",           triggerType);
      getAttribute ("TRIGGER_OFFSET",         triggerOffset);
      getAttribute ("TRIGGERED_ANTENNAS",     triggeredAntennas);
      getAttribute ("STATION_POSITION_FRAME", stationPositionFrame);
      getAttribute ("BEAM_DIRECTION_FRAME",   beamDirectionFrame);
      //
      os << "-- Group name ............. : " << group_name(true)         << endl;
      os << "-- nof. dipole datasets ... : " << nofDipoleDatasets()      << endl;
      os << "-- Station position (Value) : " << station_position_value() << endl;
      os << "-- Station position (Unit)  : " << station_position_unit()  << endl;
      os << "-- Station position (Frame) : " << stationPositionFrame     << endl;
      os << "-- Beam direction (Value) . : " << beam_direction_value()   << endl;
      os << "-- Beam direction (Unit) .. : " << beam_direction_unit()    << endl;
      os << "-- Beam direction (Frame).. : " << beamDirectionFrame       << endl;
      os << "-- Trigger type ........... : " << triggerType              << endl;
      os << "-- Trigger offset ......... : " << triggerOffset            << endl;
      os << "-- nof. triggered antennas  : " << nofTriggeredAntennas()   << endl;
      os << "-- Triggered antennas ..... : " << triggeredAntennas        << endl;
    }
    
  }
  
  // ============================================================================
  //
  //  Parameter access - dipole dataset
  //
  // ============================================================================

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
  
  // ----------------------------------------------------------- antenna_position
  
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
  
  // ------------------------------------------------------------------ channelID

  /*!
    \return channelID -- A list of the channel IDs for all the dipoles within
            this LOFAR station.
  */
#ifdef HAVE_CASA
  casa::Vector<int> TBB_StationGroup::channelID ()
  {
    uint n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    casa::Vector<int> id (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      id(n) = it->second.channelID();
      ++n;
    }
    
    return id;
  }
#else
  std::vector<int> TBB_StationGroup::channelID ()
  {
    uint n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    std::vector<int> id (datasets_p.size());

    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      id[n] = it->second.channelID();
      ++n;
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
    uint n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    casa::Vector<casa::String> names (datasets_p.size());
    
    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      names(n) = it->second.channelName();
      ++n;
    }
    
    return names;
  }
#else
  std::vector<std::string> TBB_StationGroup::channelNames ()
  {
    uint n (0);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    std::vector<std::string> names (datasets_p.size());
    
    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      names[n] = it->second.channelName();
      ++n;
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

    \return fx -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
  */
  casa::Matrix<double> TBB_StationGroup::fx (int const &start,
					     int const &nofSamples)
  {
    casa::Vector<casa::String> dipoles (datasets_p.size());
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    uint n (0);
    
    for (it=datasets_p.begin(); it!=datasets_p.end(); ++it) {
      dipoles(n) = it->first;
      ++n;
    }

    return fx (start,
	       nofSamples,
	       dipoles);
  }

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
  casa::Matrix<double> TBB_StationGroup::fx (int const &start,
					     int const &nofSamples,
					     casa::Vector<casa::String> const &dipoles)
  {
    uint nofDipoles (dipoles.nelements());
    casa::Matrix<double> data (nofSamples,nofDipoles);
    casa::Vector<double> tmp (nofSamples);
    std::map<std::string,TBB_DipoleDataset>::iterator it;
    
    for (uint n(0); n<nofDipoles; ++n) {
      it = datasets_p.find(dipoles(n));
      if (it != datasets_p.end()) {
	tmp = it->second.fx(start,nofSamples);
      } else {
	tmp = 0.0;
      }
      data.column(n) = tmp;
    }
    
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
      it->second.antenna_position_value(tmp);
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
      it->second.antenna_position_unit(tmp);
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
    std::string beamDirectionFrame;
    std::string stationPositionFrame;
    //
    getAttribute ("TRIGGER_TYPE",           triggerType);
    getAttribute ("TRIGGER_OFFSET",         triggerOffset);
    getAttribute ("TRIGGERED_ANTENNAS",     triggeredAntennas);
    getAttribute ("BEAM_DIRECTION_FRAME",   beamDirectionFrame);
    getAttribute ("STATION_POSITION_FRAME", stationPositionFrame);
    
    try {
      rec.define(casa::RecordFieldId(attribute_name(DAL::STATION_POSITION_VALUE)),
		 station_position_value());
      rec.define(casa::RecordFieldId(attribute_name(DAL::STATION_POSITION_UNIT)),
		 station_position_unit());
      rec.define(casa::RecordFieldId("STATION_POSITION_FRAME"),
		 stationPositionFrame );
      rec.define(casa::RecordFieldId(attribute_name(DAL::BEAM_DIRECTION_VALUE)),
		 beam_direction_value());
      rec.define(casa::RecordFieldId("BEAM_DIRECTION_UNIT"),
		 beam_direction_unit());
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
	name = it->second.channelName();
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
