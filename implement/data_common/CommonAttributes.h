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
#include <vector>

// DAL header files
#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CommonAttributes
    
    \ingroup DAL
    
    \brief Collection of attributes common to all LOFAR datasets
    
    \author Lars B&auml;hren

    \date 2009/08/31

    \test tCommonAttributes.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICD: TBB Time-series data (LOFAR-USG-ICD-001)
      <li>LOFAR Data Format ICD: Beam-Formed Data (LOFAR-USG-ICD-003)
      <li>LOFAR Data Format ICD: LOFAR Sky Image (LOFAR-USG-ICD-004)
      <li>LOFAR Data Format ICD: Naming conventions (LOFAR-USG-ICD-005)
    </ul>
    
    <h3>Synopsis</h3>

    \image html lofar_common_metadata.png
    
    <h3>Example(s)</h3>
    
  */  
  class CommonAttributes {
    
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
    std::string projectCO_I_p;
    //! Names/Email-addresses of the project's primary contact person(s)
    std::string projectContact_p;
    //! Name(s) of the observer(s)
    std::string observer_p;
    //! Unique identifier for the observation
    std::string observationID_p;
    //! Reference system for time
    std::string observationTimeSys_p;
    //! Start date of the observation
    std::string observationDateStart_p;
    //! End date of the observation
    std::string observationDateEnd_p;
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
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    CommonAttributes ();

    //! Argumented constructor
    CommonAttributes (std::string const &filename,
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
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~CommonAttributes ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another CommonAttributes object from which to make a copy.
    */
    CommonAttributes& operator= (CommonAttributes const &other); 
    
    // --------------------------------------------------------------- Parameters

    /*!
      \brief Get the LOFAR group type
      \return groupName -- The name of the LOFAR group
    */
    inline std::string groupType () const {
      return groupType_p;
    }

    /*!
      \brief Set the LOFAR group type
      \param groupName -- The name of the LOFAR group.
    */
    inline void setGroupType (std::string const &groupType) {
      groupType_p = groupType;
    }

    //! Get the name of the file
    inline std::string filename () const {
      return filename_p;
    }

    //! Set the name of the file
    inline void setFilename (std::string const &filename) {
      filename_p = filename;
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
    inline std::string projectCO_I () const {
      return projectCO_I_p;
    }

    //! Set the name(s) of the project's co-PI(s)
    inline void setProjectCO_I (std::string const &projectCO_I) {
      projectCO_I_p = projectCO_I;
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
			 std::string const &projectCO_I,
			 std::string const &projectContact);

    //! Get the  unique identifier for the observation
    inline std::string observationID () const {
      return observationID_p;
    }

    //! Set the  unique identifier for the observation
    inline void setObservationID (std::string const &obsID) {
      observationID_p = obsID;
    }

    //! Get the reference system for time
    inline std::string observationTimeSys () const {
      return observationTimeSys_p;
    }

    //! Set the reference system for time
    inline void setObservationTimeSys (std::string const &obsTimeSys) {
      observationTimeSys_p = obsTimeSys;
    }

    //! Get start date of the observation
    inline std::string observationDateStart () const {
      return observationDateStart_p;
    }

    //! Set start date of the observation
    inline void setObservationDateStart (std::string const &obsDateStart) {
      observationDateStart_p = obsDateStart;
    }

    //! Get start date of the observation
    inline std::string observationDateEnd () const {
      return observationDateEnd_p;
    }

    //! Set start date of the observation
    inline void setObservationDateEnd (std::string const &obsDateEnd) {
      observationDateEnd_p = obsDateEnd;
    }

    //! Set basic information on the observation
    void setObservationInfo (std::string const &obsID,
			     std::string const &obsDateStart,
			     std::string const &obsDateEnd,
			     std::string const &obsTimeSys="UTC");
    
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
    inline int clockFrequency () const {
      return clockFrequency_p;
    }
    
    //! Set the clock frequency of the observation
    inline void setClockFrequency (int const &clockFrequency) {
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

    //! Get the number of stations used during the observation
    inline int nofStations () const {
      return nofStations_p;
    }
    
    //! Set the number of stations used during the observation
    inline void nofStations (int const &nofStations) {
      nofStations_p = nofStations;
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

    // ------------------------------------------------------------------ Methods
    
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
    
    //! Unconditional copying
    void copy (CommonAttributes const &other);
    
    //! Unconditional deletion 
    void destroy(void);

    //! Initialize the object's internal parameters
    void init ();
    
  }; // Class CommonAttributes -- end
  
} // Namespace DAL -- end

#endif /* COMMONATTRIBUTES_H */
  
