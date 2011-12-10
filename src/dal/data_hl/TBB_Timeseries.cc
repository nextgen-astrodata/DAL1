/***************************************************************************
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

#include "TBB_Timeseries.h"

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

  /*!
    \param filename -- Name of the data file;
    \param flags  -- I/O mode flags.
  */
  TBB_Timeseries::TBB_Timeseries (std::string const &filename, IO_Mode const &flags)
  {
    open (0,filename,flags);
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
    : HDF5GroupBase ()
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
  //                                                                       sysLog

  /*!
    \return sysLog -- Container for system-wide logs.
   */
  SysLog TBB_Timeseries::sysLog ()
  {
    std::map<std::string,SysLog>::iterator it;

    if (sysLog_p.empty()) {
      std::cerr << "[TBB_Timeseries::sysLog] No system log group." << std::endl;
      return SysLog ();
    } else {
      sysLog_p.begin();
      return (*it).second;
    }
  }

  //_____________________________________________________________________________
  //                                                                      summary

  /*!
    \param os -- Output stream to which the summary is written.
  */
  void TBB_Timeseries::summary (std::ostream &os)
  {
    os << "[TBB_Timeseries] Summary of object properties"             << endl;
    os << "-- File name  ........... : " << filename_p                << endl;
    os << "-- Location ID .......... : " << locationID()              << endl;
    os << "-- nof. station groups .. : " << stationGroups_p.size()    << endl;
    os << "-- nof. selected datasets : " << selectedDatasets_p.size() << endl;

    if (location_p > 0) {
//       CommonAttributes attr = commonAttributes();
//       os << "-- Telescope            : " << attr.telescope()       << endl;
//       os << "-- Observer             : " << attr.observer()        << endl;
//       os << "-- Project              : " << attr.projectTitle()    << endl;
//       os << "-- Observation ID       : " << attr.observationID()   << endl;
      os << "-- nof. dipole datasets . : " << nofDipoleDatasets()       << endl;
      os << "-- nof. selected datasets : " << selectedDatasets_p.size() << endl;
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
           structure is attached; parameter inherited through HDF5GroupBase,
	   but not evaluated here.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param flags  -- I/O mode flags.
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool TBB_Timeseries::open (hid_t const &location,
			     std::string const &name,
			     IO_Mode const &flags)
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

      // and open as HDF5 file
      if ( (flags.flags() & IO_Mode::ReadOnly) ) {
        // Open read-only
        location_p = H5Fopen (name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
      }
      else {
        // Open read-write
        location_p = H5Fopen (name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
      }
    } else {
      infile.close();
      location_p = 0;
    }

    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open file, check if we are supposed to create one */
      if ( (flags.flags() & IO_Mode::Create) ||
          (flags.flags() & IO_Mode::OpenOrCreate) ) {
        location_p = H5Fcreate (name.c_str(),
            H5F_ACC_TRUNC,
            H5P_DEFAULT,
            H5P_DEFAULT);
        /* Write the common attributes attached to the root group */
        CommonAttributes attr;
        attr.h5write(location_p);
        //
        HDF5Attribute::write (location_p, "FILENAME", name );
      } else {
        throw IOError();
      }
    }

    // Open embedded groups
    if (status) {
      status = openEmbedded (flags);
    } else {
      std::cerr << "[TBB_Timeseries::open] Skip opening embedded groups!"
        << std::endl;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool TBB_Timeseries::openEmbedded (IO_Mode const &flags)
  {
    bool status (true);

    // Open system-wide logs _____________________
    status *= openSysLog (flags);

    // Open the station groups ___________________
    status *= openStationGroups (flags);

    return status;
  }

  //_____________________________________________________________________________
  //                                                                   openSysLog
  
  bool TBB_Timeseries::openSysLog (IO_Mode const &flags)
  {
    bool status           = flags.flags();
    std::string groupName = SysLog::getName();

    sysLog_p[groupName] = SysLog (location_p,flags);
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                            openStationGroups
  
  /*!
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool TBB_Timeseries::openStationGroups (IO_Mode const &flags)
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
						      *it, flags);
      }
    } else {
      throw IOError();
    }

    status *= setSelectedDatasets ();
    
    return status;
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
  //                                                            nofSelectedDatasets

  uint TBB_Timeseries::nofSelectedDatasets ()
  {
    uint nofDatasets (0);
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it=stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      nofDatasets += it->second.nofSelectedDatasets();
    }

    return nofDatasets;
  }

  //_____________________________________________________________________________
  //                                                                dipoleNumbers

  std::vector<int> TBB_Timeseries::dipoleNumbers ()
  {
    std::vector<int> numbers;
    std::vector<int> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      tmp.clear();
      tmp = it->second.dipoleNumbers();
      for (unsigned int n(0); n<tmp.size(); ++n) {
	numbers.push_back(tmp[n]);
      }
    }

    return numbers;
  }
  
  //_____________________________________________________________________________
  //                                                                  dipoleNames

  std::vector<std::string> TBB_Timeseries::dipoleNames ()
  {
    std::vector<std::string> names;
    std::vector<std::string> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      tmp.clear();
      tmp = it->second.dipoleNames();
      for (unsigned int n(0); n<tmp.size(); ++n) {
	names.push_back(tmp[n]);
      }
    }

    return names;
  }
  
  //_____________________________________________________________________________
  //                                                              selectedDipoles

  /*!
    \return selection -- Names of the selected dipole datasets.
  */
  std::set<std::string> TBB_Timeseries::selectedDipoles ()
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
    \return selection -- Names of the dipole datasets to be selected.
  */
  bool TBB_Timeseries::selectDipoles (std::set<std::string> const &selection)
  {
    bool status (true);
    std::map<std::string,TBB_StationGroup>::iterator it;
    
    /* Forward selection instruction to the underlying station groups */
    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      status *= it->second.selectDipoles(selection);
    }
    
    /* Local book-keeping */
    status *= setSelectedDatasets ();
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                             selectAllDipoles
  
  bool TBB_Timeseries::selectAllDipoles ()
  {
    bool status (true);
    std::map<std::string,TBB_StationGroup>::iterator it;

    /* Forward selection instruction to the underlying station groups */
    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      status *= it->second.selectAllDipoles();
    }
    
    /* Local book-keeping */
    status *= setSelectedDatasets ();

    return status;
  }

  //_____________________________________________________________________________
  //                                                          setSelectedDatasets

  bool TBB_Timeseries::setSelectedDatasets ()
  {
    bool status (true);
    std::map<std::string,TBB_StationGroup>::iterator iterStation;
    std::map<std::string,iterDipoleDataset> tmp;
    std::map<std::string,iterDipoleDataset>::iterator it;

    selectedDatasets_p.clear();

    for (iterStation=stationGroups_p.begin();
	 iterStation!=stationGroups_p.end();
	 ++iterStation) {
      /* Clear temporary map */
      tmp.clear();
      /* Get selection from the current station group */
      tmp = iterStation->second.dipoleSelection();
      /* Add the elements from the selection for the station */
      for (it=tmp.begin(); it!=tmp.end(); ++it) {
	selectedDatasets_p[it->first] = it->second;
      }
    }

    return status;
  }
  
  // ============================================================================
  //
  //  Access to attributes attached to the station groups
  //
  // ============================================================================


  //_____________________________________________________________________________
  //                                                                 trigger_type

#ifdef DAL_WITH_CASA
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
#else 
  std::vector<std::string> TBB_Timeseries::trigger_type ()
  {
    uint nofStations = stationGroups_p.size();
    std::vector<std::string> trigger (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("TRIGGER_TYPE",trigger[n]);
      ++n;
    }

    return trigger;
  }
#endif
  
  //_____________________________________________________________________________
  //                                                               trigger_offset

#ifdef DAL_WITH_CASA
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
#else
  std::vector<double> TBB_Timeseries::trigger_offset ()
  {
    uint nofStations = nofStationGroups();
    std::vector<double> trigger (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("TRIGGER_OFFSET",trigger[n]);
      ++n;
    }

    return trigger;
  }
#endif

#ifdef DAL_WITH_CASA

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
#endif

  //_____________________________________________________________________________
  //                                                       station_position_frame

#ifdef DAL_WITH_CASA
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
#else
  std::vector<std::string> TBB_Timeseries::station_position_frame ()
  {
    uint nofStations = nofStationGroups();
    std::vector<std::string> frame (nofStations);
    std::map<std::string,TBB_StationGroup>::iterator it;
    std::string tmp;
    int n (0);

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      it->second.getAttribute("STATION_POSITION_FRAME",tmp);
      frame[n] = tmp;
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

#ifdef DAL_WITH_CASA

  //_____________________________________________________________________________
  //                                                                    antenna_position

  casa::Vector<casa::MPosition> TBB_Timeseries::antenna_position ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofSelectedDatasets();
    casa::Vector<casa::MPosition> positions (nofDipoles);
    casa::Vector<casa::MPosition> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      tmp        = (*it).second.antenna_position();
      nofDipoles = (*it).second.nofSelectedDatasets();
      // go through the dipoles from an individual station
      for (dipole=0; dipole<nofDipoles; dipole++) {
	positions(n) = tmp[dipole];
	n++;
      }
      // increment station counter
      ++station;
    }
    
    return positions;
  }

  //_____________________________________________________________________________
  //                                                              set_antenna_position

  /*!
    \param pos -- Map with (dipole name, MPosition pairs)
  */
  bool TBB_Timeseries::set_antenna_position (std::map<std::string, casa::MPosition> &pos)
  {
    std::map<std::string, casa::MPosition>::iterator pos_it;
    std::map<std::string, iterDipoleDataset>::iterator ds_it;

    bool status = true;

    for (pos_it=pos.begin(); pos_it!=pos.end(); ++pos_it) {
      ds_it = selectedDatasets_p.find(pos_it->first);
      if (ds_it != selectedDatasets_p.end())
      {
        if ((ds_it->second)->second.set_antenna_position(pos_it->second) == false)
        {
          status = false;
        }
      }
      else
      {
        status = false;
      }
    }

    return status;
  }

#endif

  //_____________________________________________________________________________
  //                                                                    channelID

  std::vector<int> TBB_Timeseries::channelID ()
  {
    uint n           = 0;
    uint station     = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofSelectedDatasets();
    std::vector<int> channelIDvalues (nofDipoles);
    std::vector<int> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      tmp        = (*it).second.dipoleNumbers();
      nofDipoles = (*it).second.nofSelectedDatasets();
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
    std::vector<uint> out;

    getAttributes("TIME", out);

    return out;
  }

  //_____________________________________________________________________________
  //                                                                sample_number

  std::vector<uint> TBB_Timeseries::sample_number ()
  {
    std::vector<uint> out;

    getAttributes("SAMPLE_NUMBER", out);

    return out;
  }

  //_____________________________________________________________________________
  //                                                                 nyquist_zone

  std::vector<uint> TBB_Timeseries::nyquist_zone ()
  {
    std::vector<uint> out;

    getAttributes("NYQUIST_ZONE", out);

    return out;
  }

  //_____________________________________________________________________________
  //                                                       sample_frequency_value

  std::vector<double> TBB_Timeseries::sample_frequency_value ()
  {
    std::vector<double> out;

    getAttributes("SAMPLE_FREQUENCY_VALUE", out);

    return out;
  }

  //_____________________________________________________________________________
  //                                                        sample_frequency_unit

  std::vector<std::string> TBB_Timeseries::sample_frequency_unit ()
  {
    std::vector<std::string> out;

    getAttributes("SAMPLE_FREQUENCY_UNIT", out);

    return out;
  }
  
  //_____________________________________________________________________________
  //                                                                  data_length

  std::vector<uint> TBB_Timeseries::data_length ()
  {
    std::vector<uint> out;

    getAttributes("DATA_LENGTH", out);

    return out;
  }

  //_____________________________________________________________________________
  //                                                                  cable_delay

  std::vector<double> TBB_Timeseries::cable_delay ()
  {
    std::vector<double> out;

    getAttributes("CABLE_DELAY", out);

    return out;
  }

  //_____________________________________________________________________________
  //                                                                  cable_delay

  std::vector<std::string> TBB_Timeseries::cable_delay_unit ()
  {
    std::vector<std::string> out;

    getAttributes("CABLE_DELAY_UNIT", out);

    return out;
  }

  // -------------------------------------------------------------- sample_offset

  std::vector<int> TBB_Timeseries::sample_offset (uint const &refAntenna)
  {
    // Store current antenna selection
    std::set<std::string> selection = selectedDipoles ();

    // Select only the reference antenna
    std::set<std::string> ref;
    ref.insert(dipoleNames()[refAntenna]);
    selectDipoles(ref);

    int refTime = time()[0];
    uint refSample = sample_number()[0];

    // Restore antenna selection
    selectDipoles(selection);
    
    uint nofDipoles              = nofSelectedDatasets();
    std::vector<uint> valTime   = time();
    std::vector<uint> valSample = sample_number();
    std::vector<int> offset (nofDipoles);

    for (uint n(0); n<nofDipoles; n++) {
      offset[n] = valTime[n]-refTime + valSample[n]-refSample;
    }

    return offset;
  }

  // -------------------------------------------------------------- alignment_reference_antenna
  uint TBB_Timeseries::alignment_reference_antenna ()
  {
    // Store current antenna selection
    std::set<std::string> selection = selectedDipoles();

    // Select all dipoles to find overall best antenna for alignment
    selectAllDipoles();

    // Reference antenna for alignment
    uint refAntenna = 0;
    double current = 0.;

    // Get TIME for each antenna
    std::vector<uint> t = time();

    // Get SAMPLE_NUMBER for each antenna
    std::vector<uint> sn = sample_number();

    // Get FREQUENCY_VALUE for each antenna
    std::vector<double> f = sample_frequency_value();

    // Find antenna that starts getting data last and use it as reference
    double max = static_cast<double>(t[0])+(static_cast<double>(sn[0])/(f[0]*1.e6));

    for (uint i=1; i<t.size(); ++i)
    {
      // WARNING this assumes frequency is in MHz this needs to be migrated
      // to parrent class taking units into account.
      current = static_cast<double>(t[i])+(static_cast<double>(sn[i])/(f[i]*1.e6));

      if (current > max)
      {
        refAntenna = i;
        max = current;
      }
    }

    // Restore antenna selection
    selectDipoles(selection);

    return refAntenna;
  }

  // -------------------------------------------------------------- maximum_read_length
  uint TBB_Timeseries::maximum_read_length (uint const &refAntenna)
  {
    std::vector<uint> length = data_length();
    std::vector<int> offset = sample_offset(refAntenna);

    uint currentLength = 0;
    uint maxLength = 0;

    for (uint i=0; i<length.size(); ++i)
    {
      currentLength = length[i] - offset[i];

      if (currentLength > maxLength)
      {
        maxLength = currentLength;
      }
    }

    return maxLength;
  }

  // ============================================================================
  //
  //  High-level access to data and attributes
  //
  // ============================================================================

#ifdef DAL_WITH_CASA

  //_____________________________________________________________________________
  //                                                                     readData
  
  /*!
    \retval data -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
    \param start      -- Number of the sample at which to start reading.
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
  */
  bool TBB_Timeseries::readData (casa::Matrix<double> &data,
				 int const &start,
				 int const &nofSamples)
  {
    uint nofDipoles = selectedDipoles().size();
    casa::Vector<int> startPositions (nofDipoles,start);

    return readData (data,
		     startPositions,
		     nofSamples);
  }
  
  //_____________________________________________________________________________
  //                                                                     readData
  
  /*!
    \retval data -- [nofSamples,dipole] Array of raw ADC samples representing
            the electric field strength as function of time.
    \param start      -- Number of the sample at which to start reading; w.r.t.
           to the variant where just a single value is provided, this function
           allows to set the starting point for each dipole individually
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
  */
  bool TBB_Timeseries::readData (casa::Matrix<double> &data,
				 casa::Vector<int> const &start,
				 int const &nofSamples)
  {
    uint sizeSelection = selectedDatasets_p.size();
    uint sizeStart     = start.nelements();
    casa::IPosition shape (2,nofSamples,sizeSelection);
    
    // Check input parameters ______________________________
    
    if (sizeStart != sizeSelection) {
      std::cerr << "[TBB_Timeseries::readData]"
		<< " Wrong length of vector with start positions!"
		<< std::endl;
      std::cerr << " -- size(selection) = " << sizeSelection << std::endl;
      std::cerr << " -- size(start)     = " << sizeStart     << std::endl;
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
    std::cout << "[TBB_Timeseries::readData]" << std::endl;
    std::cout << " -- start            = " << start             << std::endl;
    std::cout << " -- selected dipoles = " << selectedDipoles() << std::endl;
    std::cout << " -- shape(data)      = " << data.shape()      << std::endl;
#endif

    return status;
  }
  
#endif
  
  // ============================================================================
  //
  //  Methods using casacore
  //
  // ============================================================================

#ifdef DAL_WITH_CASA

  //_____________________________________________________________________________
  //                                                             sample_frequency

  casa::Vector<casa::MFrequency> TBB_Timeseries::sample_frequency ()
  {
    bool status (true);
    uint n           = 0;
    uint dipole      = 0;
    uint nofDipoles  = nofSelectedDatasets();
    casa::Vector<casa::MFrequency> freq (nofDipoles);
    casa::Vector<casa::MFrequency> tmp;
    std::map<std::string,TBB_StationGroup>::iterator it;

    for (it = stationGroups_p.begin(); it!=stationGroups_p.end(); ++it) {
      /* Retrieve sample frequencies per station */
      status = (*it).second.sample_frequency(tmp);
      /* Determine number of dipoles within this station */
      nofDipoles = (*it).second.nofSelectedDatasets();
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
