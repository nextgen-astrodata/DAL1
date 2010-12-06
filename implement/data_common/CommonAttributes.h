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
#include <CommonAttributesProject.h>
#include <CommonAttributesObservation.h>

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
        <li>LOFAR Data Format ICD: TBB Time-Series Data (LOFAR-USG-ICD-001)
	<li>LOFAR Data Format ICD: Representations of World Coordinates
	(LOFAR-USG-ICD-002)
	<li>LOFAR Data Format ICD: Beam-Formed Data (LOFAR-USG-ICD-003)
	<li>LOFAR Data Format ICD: Radio Sky Image Cubes (LOFAR-USG-ICD-004)
	<li>LOFAR Data Format ICD: File Naming Conventions (LOFAR-USG-ICD-005)
	<li>LOFAR Data Format ICD: Dynamic Spectrum Data (LOFAR-USG-ICD-006)
	<li>LOFAR Data Format ICD: Visibility Data (LOFAR-USG-ICD-007)
	<li>LOFAR Data Format ICD: Rotation Measure Synthesis Cubes (LOFAR-USG-ICD-008)
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
    std::string itsFilename;
    //! File creation date, YYYY-MM-DDThh:mm:ss.s
    std::string filedate_p;
    //! File type
    std::string filetype_p;
    //! Name of the telescope ("LOFAR")
    std::string telescope_p;
    //! Name(s) of the observer(s)
    std::string observer_p;

    //! Common LOFAR attributes for description of project
    CommonAttributesProject attributesProject_p;
    //! Common LOFAR attributes for description of observation
    CommonAttributesObservation observation_p;

    //! Clock frequency (LOFAR: 200.0 or 160.0)
    double clockFrequency_p;
    //! Clock frequency physical unit
    std::string clockFrequencyUnit_p;
    //! Antenna set specification of observation
    std::string antennaSet_p;
    //! Filter selection
    std::string filterSelection_p;
    //! Single or list of observation targets/sources
    std::string target_p;
    //! Processing system name/version
    std::string systemVersion_p;
    //! Processing pipeline name
    std::string pipelineName_p;
    //! Processing pipeline version
    std::string pipelineVersion_p;
    //! Notes or comments
    std::string notes_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CommonAttributes ();
    
    //! Argumented constructor
    CommonAttributes (Filename const &filename,
		      std::string const &filetype,
		      std::string const &filedate);

    //! Argumented constructor
    CommonAttributes (CommonAttributesProject const &attributesProject);

    //! Argumented constructor
    CommonAttributes (CommonAttributesObservation const &attributesObservation);

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
      return itsFilename;
    }
    
    //! Set the name of the file
    inline void setFilename (Filename const &name) {
      Filename tmp = name;
      itsFilename = tmp.filename();
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

    //! Get the name(s) of the observer(s)
    inline std::string observer () const {
      return observer_p;
    }

    inline void setObserver (std::string const &observer) {
      observer_p = observer;
    }

    CommonAttributesProject attributesProject () const {
      return attributesProject_p;
    }

    //! Set the various infos on the project
    void setAttributesProject (CommonAttributesProject const &attributesProject);
    
    //! Set the various infos on the project
    void setAttributesProject (std::string const &projectID,
			       std::string const &projectTitle,
			       std::string const &projectPI,
			       std::string const &projectCoI,
			       std::string const &projectContact);
    
    //! Get common LOFAR attributes for description of observation
    CommonAttributesObservation attributesObservation () const {
      return observation_p;
    }

    //! Set observation start date
    void setObservationStart (std::string const &startMJD,
			      std::string const &startTAI,
			      std::string const &startUTC);

    //! Set observation end date
    void setObservationEnd (std::string const &endMJD,
			    std::string const &endTAI,
			    std::string const &endUTC);

    //! Get clock frequency (LOFAR: 200.0 or 160.0)
    inline double clockFrequency () const {
      return clockFrequency_p;
    }
    //! Set clock frequency (LOFAR: 200.0 or 160.0)
    inline void setClockFrequency (double const &clockFrequency) {
      clockFrequency_p = clockFrequency;
    }

    //! Get clock frequency physical unit
    inline std::string clockFrequencyUnit () const {
      return clockFrequencyUnit_p;
    }
    //! Set clock frequency physical unit
    inline void setClockFrequencyUnit (std::string const &clockFrequencyUnit) {
      clockFrequencyUnit_p = clockFrequencyUnit;
    }

    //! Get antenna set specification of observation
    inline std::string antennaSet () const {
      return antennaSet_p;
    }
    //! Set antenna set specification of observation
    inline void setAntennaSet (std::string const &antennaSet) {
      antennaSet_p = antennaSet;
    }

    //! Get filter selection
    inline std::string filterSelection () const {
      return filterSelection_p;
    }
    //! Set filter selection
    inline void setFilterSelection (std::string const &filterSelection) {
      filterSelection_p = filterSelection;
    }

    //! Get single or list of observation targets/sources
    inline std::string target () const {
      return target_p;
    }
    //! Set single or list of observation targets/sources
    inline void setTarget (std::string const &target) {
      target_p = target;
    }

    //! Get processing system name/version
    inline std::string systemVersion () const {
      return systemVersion_p;
    }
    //! Set processing system name/version
    inline void setSystemVersion (std::string const &systemVersion) {
      systemVersion_p = systemVersion;
    }

    //! Processing pipeline name
    inline std::string pipelineName () const {
      return pipelineName_p;
    }
    //! Set processing pipeline name
    inline void setPipelineName (std::string const &pipelineName) {
      pipelineName_p = pipelineName;
    }

    //! Processing pipeline version
    inline std::string pipelineVersion () const {
      return pipelineVersion_p;
    }
    //! Set processing pipeline version
    inline void setPipelineVersion (std::string const &pipelineVersion) {
      pipelineVersion_p = pipelineVersion;
    }

    //! Notes or comments
    inline std::string notes () const {
      return notes_p;
    }
    //! Notes or comments
    inline void setNotes (std::string const &notes) {
      notes_p = notes;
    }
    
    // === Methods ==============================================================
    
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

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // Methods which require HDF5 __________________________

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

    // Methods which require casacore ______________________

#ifdef HAVE_CASA
    //! Retrieve common attributes using csac::Record as container
    bool getAttributes (casa::Record &rec);
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
  
