/***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#ifndef COMMONATTRIBUTES_H
#define COMMONATTRIBUTES_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

// DAL header files
#include <core/dalCommon.h>
#include <core/HDF5Attribute.h>
#include <data_common/Filename.h>

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
        <li>\ref dal_icd_001
        <li>\ref dal_icd_002
        <li>\ref dal_icd_003
        <li>\ref dal_icd_004
        <li>\ref dal_icd_005
        <li>\ref dal_icd_006
        <li>\ref dal_icd_007
        <li>\ref dal_icd_008
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

    //! Attributes names
    std::set<std::string> itsAttributes;
    //! Filename object, holding the various components making up the filename
    Filename itsFilenameHolder;

    //! LOFAR group type ("Root")
    std::string itsGroupType;
    //! File name
    std::string itsFilename;
    //! File type
    std::string itsFiletype;
    //! File creation date, YYYY-MM-DDThh:mm:ss.s
    std::string itsFiledate;
    //! Name of the telescope ("LOFAR")
    std::string itsTelescope;
    //! Name(s) of the observer(s)
    std::string itsObserver;
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    //! Unique identifier for the project
    std::string itsProjectID;
    //! Name of the project
    std::string itsProjectTitle;
    //! Name of the project's principal investigator
    std::string itsProjectPI;
    //! Name(s) of the project's co-PI(s)
    std::string itsProjectCoI;
    //! Names/Email-addresses of the project's primary contact person(s)
    std::string itsProjectContact;
    /*________________________________________________________________
      Common LOFAR attributes for description of observation
    */
    //! Unique identifier for the observation
    std::string itsObservationID;
    //! Start date of the observation (MJD)
    std::string itsStartMJD;
    //! Start date of the observation (TAI)
    std::string itsStartTAI;
    //! Start date of the observation (UTC)
    std::string itsStartUTC;
    //! End date of the observation (MJD)
    std::string itsEndMJD;
    //! End date of the observation (TAI)
    std::string itsEndTAI;
    //! End date of the observation (UTC)
    std::string itsEndUTC;
    //! nof. stations used during the observation
    int itsNofStations;
    //! List of stations used during the observation
    std::vector<std::string> itsStationsList;
    //! Observation minimum frequency
    double itsFrequencyMin;
    //! Observation maximum frequency
    double itsFrequencyMax;
    //! Observation center frequency
    double itsFrequencyCenter;
    //! Observation frequency physical units
    std::string itsFrequencyUnit;
    //! Number of bits per sample in the incoming data stream
    int itsNofBitsPerSample;
    /*________________________________________________________________
     */
    //! Clock frequency (LOFAR: 200.0 or 160.0)
    double itsClockFrequency;
    //! Clock frequency physical unit
    std::string itsClockFrequencyUnit;
    //! Antenna set specification of observation
    std::string itsAntennaSet;
    //! Filter selection
    std::string itsFilterSelection;
    //! Single or list of observation targets/sources
    std::string itsTarget;
    //! Processing system name/version
    std::string itsSystemVersion;
    //! Processing pipeline name
    std::string itsPipelineName;
    //! Processing pipeline version
    std::string itsPipelineVersion;
    //! Interface Control Document (ICD) number 
    std::string itsIcdNumber;
    //! Interface Control Document (ICD) document version
    std::string itsIcdVersion;
    //! Notes or comments
    std::string itsNotes;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CommonAttributes ();
    
    //! Argumented constructor
    CommonAttributes (Filename const &filename);

    //! Argumented constructor
    CommonAttributes (Filename const &filename,
		      std::string const &filedate);
    
    //! Argumented constructor
    CommonAttributes (std::string const &projectID,
		      std::string const &projectTitle,
		      std::string const &projectPI,
		      std::string const &projectCoI,
		      std::string const &projectContact);
    
#ifdef DAL_WITH_HDF5
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

    //! Attribute names
    inline std::set<std::string> attributes () const {
      return itsAttributes;
    }

    /*!
      \brief Get the LOFAR group type
      \return groupName -- The name of the LOFAR group
    */
    inline std::string groupType () const {
      return itsGroupType;
    }
    //! Get the name of the file
    inline std::string filename () const {
      return itsFilename;
    }
    //! Set the name of the file
    inline void setFilename (DAL::Filename const &filename) {
      itsFilenameHolder = filename;
      itsFilename       = itsFilenameHolder.filename();
      itsFiletype       = itsFilenameHolder.filetypeName();
      itsObservationID  = itsFilenameHolder.observationID();
    }
    //! Get the type of the file
    inline std::string filetype () const {
      return itsFiletype;
    }
    //! Get the file creation date
    inline std::string filedate () const {
      return itsFiledate;
    }
    //! Set the file creation date
    inline void setFiledate (std::string const &filedate) {
      itsFiledate = filedate;
    }
    //! Get the name of the telescope
    inline std::string telescope () const {
      return itsTelescope;
    }
    //! Set the name of the telescope
    inline void setTelescope (std::string const &telescope) {
      itsTelescope = telescope;
    }
    //! Get the name(s) of the observer(s)
    inline std::string observer () const {
      return itsObserver;
    }
    //! Name(s) of the observer(s)
    inline void setObserver (std::string const &observer) {
      itsObserver = observer;
    }
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    //! Unique identifier for the project
    inline std::string projectID () const {
      return itsProjectID;
    }
    //! Set unique identifier for the project
    inline void setProjectID (std::string const &id) {
      itsProjectID = id;
    }

    //! Name of the project
    inline std::string projectTitle () const {
      return itsProjectTitle;
    }
    //! Set name of the project
    inline void setProjectTitle (std::string const &title) {
      itsProjectTitle = title;
    }

    //! Name of the project's principal investigator
    inline std::string projectPI () const {
      return itsProjectPI;
    }
    //! Set name of the project's principal investigator
    inline void setProjectPI (std::string const &projectPI) {
      itsProjectPI = projectPI;
    }

    //! Name(s) of the project's co-PI(s)
    inline std::string projectCoI () const {
      return itsProjectCoI;
    }
    //! Set name(s) of the project's co-PI(s)
    inline void setProjectCoI (std::string const &projectCoI) {
      itsProjectCoI = projectCoI;
    }

    //! Get names/Email-addresses of the project's primary contact person(s)
    inline std::string projectContact () const {
      return itsProjectContact;
    }
    //! Set names/Email-addresses of the project's primary contact person(s)
    inline void setProjectContact (std::string const &contact) {
      itsProjectContact = contact;
    }
    //! Common LOFAR attributes for description of project
    void setAttributesProject (std::string const &projectID,
			       std::string const &projectTitle,
			       std::string const &projectPI,
			       std::string const &projectCoI,
			       std::string const &projectContact);
    /*________________________________________________________________
      Common LOFAR attributes for description of observation
    */
    //! Get unique identifier for the observation
    inline std::string observationID () const {
      return itsObservationID;
    }
    //! Set unique identifier for the observation
    inline void setObservationID (std::string const &id) {
      Filename tmp (itsFilenameHolder);
      tmp.setObservationID (id);
      setFilename (tmp);
    }
    //! Get start date of the observation (MJD)
    inline std::string startMJD () const {
      return itsStartMJD;
    }
    //! Set start date of the observation (MJD)
    inline void setStartMJD (std::string const &startMJD) {
      itsStartMJD = startMJD;
    }
    //! Get start date of the observation (TAI)
    inline std::string startTAI () const {
      return itsStartTAI;
    }
    //! Set start date of the observation (TAI)
    inline void setStartTAI (std::string const &startTAI) {
      itsStartTAI = startTAI;
    }
    //! Get start date of the observation (UTC)
    inline std::string startUTC () const {
      return itsStartUTC;
    }
    //! Set start date of the observation (UTC)
    inline void setStartUTC (std::string const &startUTC) {
      itsStartUTC = startUTC;
    }
    //! Get end date of the observation (MJD)
    inline std::string endMJD () const {
      return itsEndMJD;
    }
    //! Set end date of the observation (MJD)
    inline void setEndMJD (std::string const &endMJD) {
      itsEndMJD = endMJD;
    }
    //! End date of the observation (TAI)
    inline std::string endTAI () const {
      return itsEndTAI;
    }
    //! End date of the observation (TAI)
    inline void setEndTAI (std::string const &endTAI) {
      itsEndTAI = endTAI;
    }
    //! End date of the observation (UTC)
    inline std::string endUTC () const {
      return itsEndUTC;
    }
    //! End date of the observation (UTC)
    inline void setEndUTC (std::string const &endUTC) {
      itsEndUTC = endUTC;
    }
    //! nof. stations used during the observation
    inline int nofStations () const {
      return itsNofStations;
    }
    //! List of stations used during the observation
    inline std::vector<std::string> stationsList () const {
      return itsStationsList;
    }
    //! List of stations used during the observation
    inline void setStationsList (std::vector<std::string> const &stationsList) {
      itsStationsList.clear();
      if (stationsList.empty()) {
	itsNofStations = 0;
      } else {
	itsNofStations = stationsList.size();
	itsStationsList = stationsList;
      }
    }
    //! Observation minimum frequency
    inline double frequencyMin () const {
      return itsFrequencyMin;
    }
    //! Observation minimum frequency
    inline void setFrequencyMin (double const &frequencyMin) {
      itsFrequencyMin = frequencyMin;
    }
    
    //! Observation maximum frequency
    inline double frequencyMax () const {
      return itsFrequencyMax;
    }
    //! Observation maximum frequency
    inline void setFrequencyMax (double const &frequencyMax) {
      itsFrequencyMax = frequencyMax;
    }
    
    //! Observation center frequency
    inline double frequencyCenter () const {
      return itsFrequencyCenter;
    }
    //! Observation center frequency
    inline void setFrequencyCenter (double const &frequencyCenter) {
      itsFrequencyCenter = frequencyCenter;
    }
    
    //! Observation frequency physical units
    inline std::string frequencyUnit () const {
      return itsFrequencyUnit;
    }
    //! Observation frequency physical units
    inline void setFrequencyUnit (std::string const &frequencyUnit) {
      itsFrequencyUnit = frequencyUnit;
    }
    //! Get the number of bits per sample in the incoming data stream
    inline int nofBitsPerSample () const {
      return itsNofBitsPerSample;
    }
    //! Set observation start date
    void setObservationStart (std::string const &startMJD,
			      std::string const &startTAI,
			      std::string const &startUTC);
    //! Set observation end date
    void setObservationEnd (std::string const &endMJD,
			    std::string const &endTAI,
			    std::string const &endUTC);
    //! Set maximum, minimum and unit of observation frequency
    void setFrequency (double const &freqMin,
		       double const &freqMax,
		       std::string const &freqUnit="Hz");
    /*________________________________________________________________
      +++
     */
    //! Get clock frequency (LOFAR: 200.0 or 160.0)
    inline double clockFrequency () const {
      return itsClockFrequency;
    }
    //! Set clock frequency (LOFAR: 200.0 or 160.0)
    inline void setClockFrequency (double const &clockFrequency) {
      itsClockFrequency = clockFrequency;
    }

    //! Get clock frequency physical unit
    inline std::string clockFrequencyUnit () const {
      return itsClockFrequencyUnit;
    }
    //! Set clock frequency physical unit
    inline void setClockFrequencyUnit (std::string const &clockFrequencyUnit) {
      itsClockFrequencyUnit = clockFrequencyUnit;
    }

    //! Get antenna set specification of observation
    inline std::string antennaSet () const {
      return itsAntennaSet;
    }
    //! Set antenna set specification of observation
    inline void setAntennaSet (std::string const &antennaSet) {
      itsAntennaSet = antennaSet;
    }

    //! Get filter selection
    inline std::string filterSelection () const {
      return itsFilterSelection;
    }
    //! Set filter selection
    inline void setFilterSelection (std::string const &filterSelection) {
      itsFilterSelection = filterSelection;
    }

    //! Get single or list of observation targets/sources
    inline std::string target () const {
      return itsTarget;
    }
    //! Set single or list of observation targets/sources
    inline void setTarget (std::string const &target) {
      itsTarget = target;
    }

    //! Get processing system name/version
    inline std::string systemVersion () const {
      return itsSystemVersion;
    }
    //! Set processing system name/version
    inline void setSystemVersion (std::string const &systemVersion) {
      itsSystemVersion = systemVersion;
    }

    //! Processing pipeline name
    inline std::string pipelineName () const {
      return itsPipelineName;
    }
    //! Set processing pipeline name
    inline void setPipelineName (std::string const &pipelineName) {
      itsPipelineName = pipelineName;
    }

    //! Processing pipeline version
    inline std::string pipelineVersion () const {
      return itsPipelineVersion;
    }
    //! Set processing pipeline version
    inline void setPipelineVersion (std::string const &pipelineVersion) {
      itsPipelineVersion = pipelineVersion;
    }

    //! Get the ICD document number
    inline std::string icdNumber () const {
      return itsIcdNumber;
    }
    //! Set the ICD document number
    inline void setIcdNumber (std::string const &number) {
      itsIcdNumber = number;
    }
    
    //! Get the ICD document version
    inline std::string icdVersion () const {
      return itsIcdVersion;
    }
    //! Set the ICD document version
    inline void setIcdVersion (std::string const &version) {
      itsIcdVersion = version;
    }

    //! Notes or comments
    inline std::string notes () const {
      return itsNotes;
    }
    //! Notes or comments
    inline void setNotes (std::string const &notes) {
      itsNotes = notes;
    }
    
    // === Methods ==============================================================
    
    //! Is attribute of given \e name part of the group?
    inline bool haveAttribute (std::string const &name) const {
      return static_cast<bool>(itsAttributes.count(name));
    }
    
    //! Get the value of an attribute
    template<class T >
      bool getAttribute (std::string const &name, T &val);

    //! Get the value of an attribute
    template<class T >
      bool getAttribute (std::string const &name, std::vector<T> &val);

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

    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

    // Methods which require HDF5 __________________________

#ifdef DAL_WITH_HDF5
    //! Write the attributes to a HDF5 file
    bool h5write (hid_t const &groupID);
    
    //! Write the attributes to a HDF5 file
    bool h5write (hid_t const &locationID,
		  std::string const &name);
    
    //! Read the attributes from a HDF5 file
    bool h5read (hid_t const &groupID);
    
    //! Read the attributes from a HDF5 file
    bool h5read (hid_t const &locationID,
		 std::string const &name);
#endif    

    // Methods which require casacore ______________________

#ifdef DAL_WITH_CASA
    //! Retrieve common attributes using csac::Record as container
    bool getAttributes (casa::Record &rec);
#endif 
    
  private:

    //! Initialize internal parameters/data
    void init ();
    //! Unconditional copying
    void copy (CommonAttributes const &other);
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CommonAttributes -- end
  
} // Namespace DAL -- end

#endif /* COMMONATTRIBUTES_H */
  
