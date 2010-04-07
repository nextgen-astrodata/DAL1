/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

#include <TBB_Timeseries.h>

using std::cout;
using std::endl;

namespace DAL {  // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                               TBB_Timeseries

  /*!
    A minimal setup of the internal dataspace is performed, but since no
    data file is provided, no inspection of the data structure is carried out.
  */
  TBB_Timeseries::TBB_Timeseries ()
  {
    location_p = -1;
    stationGroups_p.clear();
  }
  
  //_____________________________________________________________________________
  //                                                               TBB_Timeseries

  /*!
    \param filename -- Name of the data file; if \e filename does not exist yet,
           it will be created.
  */
  TBB_Timeseries::TBB_Timeseries (std::string const &filename)
  {
    open (0,filename,true);
  }
  
  //_____________________________________________________________________________
  //                                                               TBB_Timeseries

  TBB_Timeseries::TBB_Timeseries (CommonAttributes const &attributes)
  {
    CommonAttributes attr = attributes;
    // open the new dataset
    open (0,attr.filename(),true);
    // write the LOFAR common attributes
    if (location_p>0 && H5Iis_valid(location_p)) {
      attr.h5write(location_p);
    }
  }
  
  //_____________________________________________________________________________
  //                                                               TBB_Timeseries

  /*!
    \param other -- Another TBB_Timeseries object from which to create this
           new one.
  */
  TBB_Timeseries::TBB_Timeseries (TBB_Timeseries const &other)
    : CommonInterface ()
  {
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
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  TBB_Timeseries& TBB_Timeseries::operator= (TBB_Timeseries const &other)
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
    \param other -- Another TBB_Timeseries object from which to create this new
           one.
  */
  void TBB_Timeseries::copy (TBB_Timeseries const &other)
  {
    location_p           = -1;
    std::string filename = other.filename_p;
    open (0,filename,false);
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                             commonAttributes
  
  CommonAttributes TBB_Timeseries::commonAttributes ()
  {
    CommonAttributes attr;

    if (location_p > 0) {
      attr.h5read(location_p);
    }

    return attr;
  }

  //_____________________________________________________________________________
  //                                                                      summary

  /*!
    \param os -- Output stream to which the summary is written.
  */
  void TBB_Timeseries::summary (std::ostream &os)
  {
    os << "[TBB_Timeseries] Summary of object properties"        << endl;
    os << "-- File name  ........  : " << filename_p             << endl;
    os << "-- Location ID .......  : " << locationID()           << endl;
    os << "-- nof. station groups  : " << stationGroups_p.size() << endl;

    if (location_p > 0) {
//       CommonAttributes attr = commonAttributes();
//       os << "-- Telescope            : " << attr.telescope()       << endl;
//       os << "-- Observer             : " << attr.observer()        << endl;
//       os << "-- Project              : " << attr.projectTitle()    << endl;
//       os << "-- Observation ID       : " << attr.observationID()   << endl;
      os << "-- nof. dipole datasets : " << nofDipoleDatasets()    << endl;
    }
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void TBB_Timeseries::setAttributes ()
  {
    CommonAttributes attr;
    
    attributes_p.clear();
    attributes_p = attr.attributes();
  }

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached; parameter inherited through CommonInterface,
	   but not evaluated here.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool TBB_Timeseries::open (hid_t const &location,
			     std::string const &name,
			     bool const &create)
  {
    bool status (true);

    /* Initialize private variables*/
    location_p = location;
    filename_p = name;
    setAttributes ();
    
    // Try to open the file ________________________________

    std::ifstream infile;
    infile.open (name.c_str(), std::ifstream::in);

    if (infile.is_open() && infile.good()) {
      // If the file already exists, close it ...
      infile.close();
      // ... and open it as HDF5 file
      location_p = H5Fopen (name.c_str(),
			    H5F_ACC_RDWR,
			    H5P_DEFAULT);
    } else {
      infile.close();
      location_p = 0;
    }
    
    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open file, check if we are supposed to create one */
      if (create) {
	location_p = H5Fcreate (name.c_str(),
				H5F_ACC_TRUNC,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* Write the common attributes attached to the root group */
	CommonAttributes attr;
	attr.h5write(location_p);
	//
	h5set_attribute (location_p, "FILENAME", name );
      } else {
	std::cerr << "[TBB_Timeseries::open] Failed to open file "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[TBB_Timeseries::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool TBB_Timeseries::openEmbedded (bool const &create)
  {
    bool status (true);

    // Open system-wide logs _____________________
    status *= openSysLog (create);

    // Open the station groups ___________________
    status *= openStationGroups();

    return status;
  }

  //_____________________________________________________________________________
  //                                                                   openSysLog
  
  bool TBB_Timeseries::openSysLog (bool const &create)
  {
    bool status (true);

    if (H5Lexists (location_p, "SysLog", H5P_DEFAULT)) {
      sysLog_p = SysLog (location_p,false);
    }
    else {
      if (create) {
	sysLog_p = SysLog (location_p,true);
      }
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                            openStationGroups
  
  /*!
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool TBB_Timeseries::openStationGroups ()
  {
    /* Check minimal condition for operations below. */
    if (location_p < 1){
      std::cerr << "[TBB_Timeseries::openStationGroups]"
		<< " Unable to set station groups; not connected to fileset."
		<< endl;
      return false;
    }
    
    //________________________________________________________________
    // Local variables.
    
    bool status (true);
    std::string nameGroup;
    std::set<std::string> groupnames;

    //________________________________________________________________
    // Obtain the number of objects attached to the root level of the file

    status = h5get_names (groupnames, location_p, H5G_GROUP);
    
    //________________________________________________________________
    // Iterate through the list of objects attached to the root group
    // of the file

    if (groupnames.size() > 0) {
      std::set<std::string>::iterator it;
      for (it=groupnames.begin(); it!=groupnames.end(); ++it) {
	stationGroups_p[*it] = DAL::TBB_StationGroup (location_p,
						      *it);
      }
    } else {
      std::cerr << "[TBB_Timeseries::openStationGroups]"
		<< " No station groups found!"
		<< std::endl;
    }
    
    return true;
  }

  //_____________________________________________________________________________
  //                                                            stationGroupNames
  
  /*!
    \return names -- A set with the names of the embedded station group objects.
  */
  std::set<std::string> TBB_Timeseries::stationGroupNames ()
  {
    std::map<std::string,TBB_StationGroup>::iterator it;
    std::set<std::string> names;

    for (it=stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      names.insert(it->first);
    }

    return names;
  }
  
  //_____________________________________________________________________________
  //                                                                 stationGroup

  /*!
    \param station -- Number of the TBB_StationGroup object embedded within this
           TBB_Timeseries object.

    return stationGroup -- The requested station group object; if the provided
           index is outside the valid range, an empty StationGroup object is
	   returned.
  */
  TBB_StationGroup TBB_Timeseries::stationGroup (uint const &station)
  {
    std::string name = TBB_StationGroup::getName (station);
    return stationGroup (name);
  }

  //_____________________________________________________________________________
  //                                                                 stationGroup

  /*!
    \param name -- Name of the TBB_StationGroup object embedded within this
           TBB_Timeseries object.

    return stationGroup -- The requested station group object; if the provided
           index is outside the valid range, an empty StationGroup object is
	   returned.
  */
  TBB_StationGroup TBB_Timeseries::stationGroup (std::string const &name)
  {
    std::map<std::string,TBB_StationGroup>::iterator it;

    it = stationGroups_p.find(name);

    if (it != stationGroups_p.end()) {
      return (*it).second;
    } else {
      std::cerr << "[TBB_Timeseries::stationGroup]"
		<< " No such station group " << name
		<< std::endl;
      return TBB_StationGroup();
    }

  }

  //_____________________________________________________________________________
  //                                                            nofDipoleDatasets

  uint TBB_Timeseries::nofDipoleDatasets ()
  {
    uint nofDatasets (0);
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it=stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      nofDatasets += it->second.nofDipoleDatasets();
    }

     return nofDatasets;
  }

  //_____________________________________________________________________________
  //                                                              selectedDipoles

  /*!
    \return selectedDipoles -- A set of the selected dipoles within the dataset
   */
  std::set<std::string> TBB_Timeseries::selectedDipoles ()
  {
    std::set<std::string> dipoles;
    std::set<std::string> tmp;
    std::set<std::string>::iterator it;
    std::map<std::string,TBB_StationGroup>::iterator itMap;

    for (itMap = stationGroups_p.begin(); itMap!=stationGroups_p.end(); ++itMap) {
      // get the selected dipoles for this group
      tmp = itMap->second.selectedDipoles();
      // append the selected dipoles
      for (it=tmp.begin(); it!=tmp.end(); ++it) {
	dipoles.insert(*it);
      }
    }

    return dipoles;
  }

  //_____________________________________________________________________________
  //                                                           setSelectedDipoles

  bool TBB_Timeseries::setSelectedDipoles (std::set<std::string> const &selection)
  {
    bool status (true);
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      status *= it->second.setSelectedDipoles(selection);
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                             selectAllDipoles

  bool TBB_Timeseries::selectAllDipoles ()
  {
    bool status (true);
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      status *= it->second.selectAllDipoles();
    }
    
    return status;
  }
  
  // ============================================================================
  //
  //  Access to attributes attached to the station groups
  //
  // ============================================================================

#ifdef HAVE_CASA

  //_____________________________________________________________________________
  //                                                                 trigger_type

  casa::Vector<casa::String> TBB_Timeseries::trigger_type ()
  {
    uint nofStations = stationGroups_p.size();
    casa::Vector<casa::String> trigger (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("TRIGGER_TYPE",trigger(n));
      ++n;
    }

    return trigger;
  }

  //_____________________________________________________________________________
  //                                                               trigger_offset

  casa::Vector<double> TBB_Timeseries::trigger_offset ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<double> trigger (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("TRIGGER_OFFSET",trigger(n));
      ++n;
    }

    return trigger;
  }

  //_____________________________________________________________________________
  //                                                       station_position_value

  casa::Matrix<double> TBB_Timeseries::station_position_value ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<double> tmp;
    casa::Matrix<double> values (nofStations,3);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("STATION_POSITION_VALUE",tmp);
      values.row(n) = tmp;
      ++n;
    }

    return values;
  }

  //_____________________________________________________________________________
  //                                                        station_position_unit

  casa::Matrix<casa::String> TBB_Timeseries::station_position_unit ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::String> tmp;
    casa::Matrix<casa::String> units (nofStations,3);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("STATION_POSITION_UNIT",tmp);
      units.row(n) = tmp;
      ++n;
    }
    
    return units;
  }

  //_____________________________________________________________________________
  //                                                       station_position_frame

  casa::Vector<casa::String> TBB_Timeseries::station_position_frame ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::String> frame (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    std::string tmp;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("STATION_POSITION_FRAME",tmp);
      frame(n) = tmp;
      ++n;
    }

    return frame;
  }

  //_____________________________________________________________________________
  //                                                             station_position

  casa::Vector<casa::MPosition> TBB_Timeseries::station_position ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::MPosition> positions (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      positions(n) = (*it).second.station_position();
      ++n;
    }

    return positions;
  }

  //_____________________________________________________________________________
  //                                                         beam_direction_value

  casa::Matrix<double> TBB_Timeseries::beam_direction_value ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<double> tmp;
    casa::Matrix<double> values (nofStations,2);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("BEAM_DIRECTION_VALUE",tmp);
      values.row(n) = tmp;
      ++n;
    }

    return values;
  }
  
  // -------------------------------------------------------- beam_direction_unit

  casa::Matrix<casa::String> TBB_Timeseries::beam_direction_unit ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::String> tmp;
    casa::Matrix<casa::String> units (nofStations,2);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("BEAM_DIRECTION_UNIT",tmp);
      units.row(n) = tmp;
      ++n;
    }
    
    return units;
  }
  
  // ------------------------------------------------------- beam_direction_frame
  
  casa::Vector<casa::String> TBB_Timeseries::beam_direction_frame ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::String> frame (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    std::string tmp;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("BEAM_DIRECTION_FRAME",tmp);
      frame(n) = tmp;
      ++n;
    }
    
    return frame;
  }
  
  //_____________________________________________________________________________
  //                                                               beam_direction

  casa::Vector<casa::MDirection> TBB_Timeseries::beam_direction ()
  {
    uint nofStations = nofStationGroups();
    casa::Vector<casa::MDirection> directions (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      directions(n) = (*it).second.beam_direction();
      ++n;
    }

    return directions;
  }
  
#else
  
  // --------------------------------------------------------------- trigger_type

  std::vector<std::string> TBB_Timeseries::trigger_type ()
  {
    uint nofStations = groups_p.size();
    std::vector<std::string> trigger (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      trigger[n] = (*it).second.trigger_type();
      ++n;
    }

    return trigger;
  }

  // ------------------------------------------------------------- trigger_offset

  std::vector<double> TBB_Timeseries::trigger_offset ()
  {
    uint nofStations = nofStationGroups();
    std::vector<double> trigger (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      trigger[n] = (*it).second.trigger_offset();
      ++n;
    }

    return trigger;
  }

  // ----------------------------------------------------- station_position_frame

  std::vector<std::string> TBB_Timeseries::station_position_frame ()
  {
    uint nofStations = nofStationGroups();
    std::vector<std::string> frame (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      frame[n] = (*it).second.station_position_frame();
      ++n;
    }

    return frame;
  }

#endif


  // ============================================================================
  //
  //  Access to attributes attached to the dipole datasets
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    channelID

  std::vector<int> TBB_Timeseries::channelID ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<int> channelIDvalues (nofDipoles);
    std::vector<int> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      (*it).second.dipoleNumbers(tmp);
      nofDipoles = (*it).second.nofDipoleDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	channelIDvalues[n] = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }
    
    return channelIDvalues;
  }
  
  //_____________________________________________________________________________
  //                                                                         time

  std::vector<uint> TBB_Timeseries::time ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<uint> UnixTimes (nofDipoles);
    std::vector<uint> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttributes("TIME",tmp);
      nofDipoles = (*it).second.nofDipoleDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	UnixTimes[n] = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }

    return UnixTimes;
  }

  //_____________________________________________________________________________
  //                                                                sample_number

  std::vector<uint> TBB_Timeseries::sample_number ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<uint> samples (nofDipoles);
    std::vector<uint> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttributes("SAMPLE_NUMBER",tmp);
      nofDipoles = (*it).second.nofDipoleDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	samples[n] = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }

    return samples;
  }

  //_____________________________________________________________________________
  //                                                                 nyquist_zone

  std::vector<uint> TBB_Timeseries::nyquist_zone ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<uint> values (nofDipoles);
    std::vector<uint> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttributes("NYQUIST_ZONE",tmp);
      nofDipoles = (*it).second.nofDipoleDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	values[n] = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }

    return values;
  }

  //_____________________________________________________________________________
  //                                                       sample_frequency_value

  std::vector<double> TBB_Timeseries::sample_frequency_value ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<double> values (nofDipoles);
    std::vector<double> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttributes("SAMPLE_FREQUENCY_VALUE",tmp);
      nofDipoles = (*it).second.nofDipoleDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	values[n] = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }

    return values;
  }

  //_____________________________________________________________________________
  //                                                        sample_frequency_unit

  std::vector<std::string> TBB_Timeseries::sample_frequency_unit ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<std::string> units (nofDipoles);
    std::vector<std::string> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttributes("SAMPLE_FREQUENCY_UNIT",tmp);
      nofDipoles = (*it).second.nofDipoleDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	units[n] = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }
    
    return units;
  }
  
  //_____________________________________________________________________________
  //                                                                  data_length

  std::vector<uint> TBB_Timeseries::data_length ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    std::vector<uint> dataLengths (nofDipoles);
    std::vector <uint> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttributes("DATA_LENGTH",tmp);
      nofDipoles = (*it).second.nofDipoleDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	dataLengths[n] = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }

    return dataLengths;
  }

  // -------------------------------------------------------------- sample_offset

  std::vector<int> TBB_Timeseries::sample_offset (uint const &refAntenna)
  {
    uint nofDipoles              = nofDipoleDatasets();
    std::vector<uint> valTime   = time();
    std::vector<uint> valSample = sample_number();
    std::vector<int> offset (nofDipoles);

    for (uint n(0); n<nofDipoles; n++) {
      offset[n] = valTime[n]-valTime[refAntenna] + valSample[n]-valSample[refAntenna];
    }
    
    return offset;
  }
  

  // ============================================================================
  //
  //  High-level access to data and attributes
  //
  // ============================================================================

#ifdef HAVE_CASA

  //_____________________________________________________________________________
  //                                                                           fx
  
  /*!
    \retval data -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
    \param start      -- Number of the sample at which to start reading.
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
  */
  void TBB_Timeseries::fx (casa::Matrix<double> &data,
			   int const &start,
			   int const &nofSamples)
  {
    uint nofDipoles = selectedDipoles().size();
    casa::Vector<int> startPositions (nofDipoles,start);

    return fx (data,
	       startPositions,
	       nofSamples);
  }
  
  //_____________________________________________________________________________
  //                                                                           fx
  
  /*!
    \retval data -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
    \param start      -- Number of the sample at which to start reading; w.r.t.
           to the variant where just a single value is provided, this function
           allows to set the starting point for each dipole individually
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
  */
  void TBB_Timeseries::fx (casa::Matrix<double> &data,
			   casa::Vector<int> const &start,
			   int const &nofSamples)
  {
    uint nelem      = start.nelements();
    uint nofDipoles = 0;
    std::map<std::string,TBB_StationGroup>::iterator it;

    // Check input parameters ______________________________
    
    if (location_p > 0) {
      
      // Check the size of the 'start' vector
      for (it=stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
	nofDipoles += (it->second.selectedDipoles().size());
      }
      if (nelem != nofDipoles) {
	std::cerr << "[TBB_Timeseries::fx]"
		  << " Wrong length of vector with start positions!"
		  << std::endl;
	std::cerr << " -- nof. selected dipoles   = " << nofDipoles << std::endl;
	std::cerr << " -- nof. elements in vector = " << nelem      << std::endl;
	//
	data = casa::Matrix<double> (1,1,0);
      }
      
    }  //  end -- (location_p>0)
    else {
      std::cerr << "[TBB_Timeseries::fx] Object not connected to dataset"
		<< std::endl;
      data = casa::Matrix<double> (1,1,0);
    }

    // Retrieve data from file _____________________________
    
    casa::Matrix<double> tmpData;
    casa::Vector<int> tmpStart;
    uint nDipole (0);
    uint offsetStart (0);
    bool status (true);

    data.resize (nofSamples,nofDipoles);

    for (it=stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      // get the number of dipoles for this station
      nofDipoles = it->second.selectedDipoles().size();
      // set up start position values
      tmpStart.resize(nofDipoles);
      for (uint dipole(0); dipole<nofDipoles; ++dipole) {
	tmpStart(dipole) = start(offsetStart+dipole);
      }
      offsetStart += nofDipoles;
      // get the data for the dipoles of that station
      status = it->second.fx(tmpData,tmpStart,nofSamples);
      // copy the data
      for (uint dipole(0); dipole<nofDipoles; ++dipole) {
	data.column(nDipole) = tmpData.column(dipole);
	++nDipole;
      }
    }
    
  }
  
#endif
  
  // ============================================================================
  //
  //  Methods using casacore
  //
  // ============================================================================

#ifdef HAVE_CASA

  //_____________________________________________________________________________
  //                                                             sample_frequency

  casa::Vector<casa::MFrequency> TBB_Timeseries::sample_frequency ()
  {
    bool status (true);
    uint n           = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofDipoleDatasets();
    casa::Vector<casa::MFrequency> freq (nofDipoles);
    casa::Vector<casa::MFrequency> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      /* Retrieve sample frequencies per station */
      status = (*it).second.sample_frequency(tmp);
      /* Determine number of dipoles within this station */
      nofDipoles = (*it).second.nofDipoleDatasets();
      /* Copy the data to the returned array */
      for (dipole=0; dipole<nofDipoles; dipole++) {
	freq(n) = tmp(dipole);
	n++;
      }
    }

    return freq;
  }
#endif 

} // Namespace DAL -- end
