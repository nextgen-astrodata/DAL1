/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

#ifndef COMMONATTRIBUTES_H
#define COMMONATTRIBUTES_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

// DAL header files
#include <dalCommon.h>
#include <Filename.h>
#include <CommonInterface.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CommonAttributes
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Collection of attributes common to all LOFAR datasets
    
    \author Lars B&auml;hren

    \date 2009/08/31

    \test tCommonAttributes.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICDs:
      <ul>
        <li>TBB Time-series data (LOFAR-USG-ICD-001)
	<li>Beam-Formed Data (LOFAR-USG-ICD-003)
	<li>LOFAR Sky Image (LOFAR-USG-ICD-004)
	<li>Naming conventions (LOFAR-USG-ICD-005)
      </ul>
      <li>Components of the LOFAR user software:
      <ul>
        <li>Filename -- Class to generate filenames matching the LOFAR convention
      </ul>
      <li>Components of the LOFAR system software:
      <ul>
        <li>LCS/ApplCommon/include/ApplCommon/Observation.h
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    This class implements the set of <i>LOFAR Common Root Attributes</i>, as they
    are listed in the LOFAR Data Format ICDs.
    
    \image html lofar_common_metadata.png
    
    <h3>Example(s)</h3>

  */  
  class CommonAttributes {

    //! Names of the attributes attached to the structure
    std::set<std::string> attributes_p;
    
    //! LOFAR group type ("Root")
    std::string groupType_p;
    //! File name
    std::string filename_p;
    //! File type
    std::string filetype_p;
    //! File creation date
    std::string filedate_p;
    //! Name of the telescope ("LOFAR")
    std::string telescope_p;
    //! Unique identifier for the project
    std::string projectID_p;
    //! Name of the project
    std::string projectTitle_p;
    //! Name of the project's principal investigator
    std::string projectPI_p;
    //! Name(s) of the project's co-PI(s)
    std::string projectCoI_p;
    //! Names/Email-addresses of the project's primary contact person(s)
    std::string projectContact_p;
    //! Name(s) of the observer(s)
    std::string observer_p;
    //! Unique identifier for the observation
    std::string observationID_p;
    //! Start date of the observation (MJD)
    std::string observationStartMJD_p;
    //! Start date of the observation (TAI)
    std::string observationStartTAI_p;
    //! Start date of the observation (UTC)
    std::string observationStartUTC_p;
    //! End date of the observation (MJD)
    std::string observationEndMJD_p;
    //! End date of the observation (TAI)
    std::string observationEndTAI_p;
    //! End date of the observation (UTC)
    std::string observationEndUTC_p;
    //! Antenna set specification of observation
    std::string antennaSet_p;
    //! Filter selection
    std::string filterSelection_p;
    //! Clock frequency of the observation
    double clockFrequency_p;
    //! Physical unit associated with the clock frequency
    std::string clockFrequencyUnit_p;
    //! (List of) Oberservation target(s)
    std::string target_p;
    //! Data processing system name and version number
    std::string systemVersion_p;
    //! Processing pipeline name
    std::string pipelineName_p;
    //! Processing pipeline version number
    std::string pipelineVersion_p;
    //! Number of stations used during the observation
    int nofStations_p;
    //! List of stations used during the observation
    std::vector<std::string> stationsList_p;
    //! Notes and/or comments
    std::string notes_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CommonAttributes ();
    
    //! Argumented constructor
    CommonAttributes (Filename const &filename,
		      std::string const &filetype,
		      std::string const &filedate);

#ifdef HAVE_HDF5
    //! Argumented constructor
    CommonAttributes (hid_t const &locationID);
#endif
    
    /*!
      \brief Copy constructor
      
      \param other -- Another CommonAttributes object from which to create this new
             one.
    */
    CommonAttributes (CommonAttributes const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~CommonAttributes ();

    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another CommonAttributes object from which to make a copy.
    */
    CommonAttributes& operator= (CommonAttributes const &other); 
    
    // === Parameter access =====================================================

    //! Get a set with the list of attribute names
    inline std::set<std::string> attributes () const {
      return attributes_p;
    }
    //! Is an attribute of given name part of the LOFAR common attributes?
    inline bool haveAttribute (std::string const &name) const {
      return static_cast<bool>(attributes_p.count(name));
    }
    /*!
      \brief Get the LOFAR group type
      \return groupName -- The name of the LOFAR group
    */
    inline std::string groupType () const {
      return groupType_p;
    }
    
    //! Get the name of the file
    inline std::string filename () const {
      return filename_p;
    }
    
    //! Set the name of the file
    inline void setFilename (Filename const &name) {
      Filename tmp = name;
      filename_p = tmp.filename();
    }
    
    //! Get the type of the file
    inline std::string filetype () const {
      return filetype_p;
    }
    
    //! Set the type of the file
    inline void setFiletype (std::string const &filetype) {
      filetype_p = filetype;
    }
    
    //! Get the file creation date
    inline std::string filedate () const {
      return filedate_p;
    }

    //! Set the file creation date
    inline void setFiledate (std::string const &filedate) {
      filedate_p = filedate;
    }

    //! Get the name of the telescope
    inline std::string telescope () const {
      return telescope_p;
    }

    //! Set the name of the telescope
    inline void setTelescope (std::string const &telescope) {
      telescope_p = telescope;
    }

    //! Get the unique identifier for the project
    inline std::string projectID () const {
      return projectID_p;
    }

    //! Set the unique identifier for the project
    inline void setProjectID (std::string const &projectID) {
      projectID_p = projectID;
    }

    //! Get the name of the project
    inline std::string projectTitle () const {
      return projectTitle_p;
    }

    //! Set the name of the project
    inline void setProjectTitle (std::string const &projectTitle) {
      projectTitle_p = projectTitle;
    }
    
    //! Get the name of the project's principal investigator
    inline std::string projectPI () const {
      return projectPI_p;
    }
    
    //! Set the name of the project's principal investigator
    inline void setProjectPI (std::string const &projectPI) {
      projectPI_p = projectPI;
    }

    //! Get the name(s) of the project's co-PI(s)
    inline std::string projectCoI () const {
      return projectCoI_p;
    }

    //! Set the name(s) of the project's co-PI(s)
    inline void setProjectCoI (std::string const &projectCoI) {
      projectCoI_p = projectCoI;
    }

    //! Get the names/Email-addresses of the project's primary contact person(s)
    inline std::string projectContact () const {
      return projectContact_p;
    }

    //! Set the names/Email-addresses of the project's primary contact person(s)
    inline void setProjectContact (std::string const &projectContact) {
      projectContact_p = projectContact;
    }

    //! Set the various infos on the project
    void setProjectInfo (std::string const &projectID,
			 std::string const &projectTitle,
			 std::string const &projectPI,
			 std::string const &projectCoI,
			 std::string const &projectContact);

    //! Get the name(s) of the observer(s)
    inline std::string observer () const {
      return observer_p;
    }

    //! Set the name(s) of the observer(s)
    inline void setObserver (std::string const &observer) {
      observer_p = observer;
    }

    //! Get the  unique identifier for the observation
    inline std::string observationID () const {
      return observationID_p;
    }

    //! Set the  unique identifier for the observation
    inline void setObservationID (std::string const &obsID) {
      observationID_p = obsID;
    }

    //! Get start date of the observation (MJD)
    inline std::string observationStartMJD () const {
      return observationStartMJD_p;
    }

    //! Set start date of the observation (MJD)
    inline void setObservationStartMJD (std::string const &obsDateStart) {
      observationStartMJD_p = obsDateStart;
    }

    //! Get start date of the observation (MJD)
    inline std::string observationEndMJD () const {
      return observationEndMJD_p;
    }

    //! Set start date of the observation (MJD)
    inline void setObservationEndMJD (std::string const &obsDateEnd) {
      observationEndMJD_p = obsDateEnd;
    }

    //! Get start date of the observation (TAI)
    inline std::string observationStartTAI () const {
      return observationStartTAI_p;
    }

    //! Set start date of the observation (TAI)
    inline void setObservationStartTAI (std::string const &obsDateStart) {
      observationStartTAI_p = obsDateStart;
    }

    //! Get start date of the observation (TAI)
    inline std::string observationEndTAI () const {
      return observationEndTAI_p;
    }

    //! Set start date of the observation (TAI)
    inline void setObservationEndTAI (std::string const &obsDateEnd) {
      observationEndTAI_p = obsDateEnd;
    }

    //! Get start date of the observation (UTC)
    inline std::string observationStartUTC () const {
      return observationStartUTC_p;
    }

    //! Set start date of the observation (UTC)
    inline void setObservationStartUTC (std::string const &obsDateStart) {
      observationStartUTC_p = obsDateStart;
    }

    //! Get start date of the observation (UTC)
    inline std::string observationEndUTC () const {
      return observationEndUTC_p;
    }

    //! Set start date of the observation (UTC)
    inline void setObservationEndUTC (std::string const &obsDateEnd) {
      observationEndUTC_p = obsDateEnd;
    }

    //! Get the antenna set specification of observation
    inline std::string antennaSet () const {
      return antennaSet_p;
    }
    
    //! Set the antenna set specification of observation
    inline void setAntennaSet (std::string const &antennaSet)  {
      antennaSet_p = antennaSet;
    }
    
    //! Get the filter selection
    inline std::string filterSelection () const {
      return filterSelection_p;
    }
    
    //! Set the filter selection
    inline void setFilterSelection (std::string const &filterSelection) {
      filterSelection_p = filterSelection;
    }

    //! Get the clock frequency of the observation
    inline double clockFrequency () const {
      return clockFrequency_p;
    }
    
    //! Set the clock frequency of the observation
    inline void setClockFrequency (double const &clockFrequency) {
      clockFrequency_p = clockFrequency;
    }

    //! Get the physical unit associated with the clock frequency
    inline std::string clockFrequencyUnit () const {
      return clockFrequencyUnit_p;
    }

    //! Set the physical unit associated with the clock frequency
    inline void setClockFrequencyUnit (std::string const &unit) {
      clockFrequencyUnit_p = unit;
    }

    //! Get the (List of) Oberservation target(s)
    inline std::string target () const {
      return target_p;
    }

    //! Set the (List of) Oberservation target(s)
    inline void setTarget (std::string const &target) {
      target_p = target;
    }

    //! Get the data processing system name and version number
    inline std::string systemVersion () const {
      return systemVersion_p;
    }
    
    //! Set the data processing system name and version number
    inline void setSystemVersion (std::string const &systemVersion) {
      systemVersion_p = systemVersion;
    }
    
    //! Get the processing pipeline name
    inline std::string pipelineName () const {
      return pipelineName_p;
    }
    
    //! Set the processing pipeline name
    inline void setPipelineName (std::string const &name) {
      pipelineName_p = name;
    }

    //! Get the processing pipeline version number
    inline std::string pipelineVersion () const {
      return pipelineVersion_p;
    }

    //! Get the processing pipeline version number
    inline void setPipelineVersion (std::string const &version) {
      pipelineVersion_p = version;
    }

    //! Set information on the processing pipeline
    void setPipelineInfo (std::string const &name,
			  std::string const &version);

    //! Get the number of stations used during the observation
    inline int nofStations () const {
      return nofStations_p;
    }

    //! Get the list of stations used during the observation
    inline std::vector<std::string> stationsList () const {
      return stationsList_p;
    }

    //! Set the list of stations used during the observation
    inline void setStationsList (std::vector<std::string> const &stations) {
      nofStations_p = stations.size();
      stationsList_p.resize(nofStations_p);
      stationsList_p = stations;
    }
    
    //! Add a station to the list of stations
    bool addStationsList (std::string const &station);

    //! Get notes and/or comments
    inline std::string notes () const {
      return notes_p;
    }

    //! Set notes and/or comments
    inline void setNotes (std::string const &notes) {
      notes_p = notes;
    }

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, CommonAttributes.
    */
    inline std::string className () const {
      return "CommonAttributes";
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // === Methods ==============================================================

#ifdef HAVE_HDF5
    //! Write the attributes to a HDF5 file
    void h5write (hid_t const &groupID);
    
    //! Write the attributes to a HDF5 file
    void h5write (hid_t const &locationID,
		  std::string const &name);
    
    //! Read the attributes from a HDF5 file
    void h5read (hid_t const &groupID);
    
    //! Read the attributes from a HDF5 file
    void h5read (hid_t const &locationID,
		 std::string const &name);
#endif    
    
  private:

    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Unconditional copying
    void copy (CommonAttributes const &other);
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CommonAttributes -- end
  
} // Namespace DAL -- end

#endif /* COMMONATTRIBUTES_H */
  
