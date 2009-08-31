/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 2286 2009-02-03 10:50:48Z baehren                    $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars Baehren (bahren@astron.nl)                                       *
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
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
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
    std::string projectName_p;
    //! Brief project description
    std::string projectDescription_p;
    //! Name(s) of the observer(s)
    std::string observer_p;
    //! Unique identifier for the observation
    std::string observationID_p;
    //! Date of the observation
    std::string observationDate_p;
    //! Observation mode
    std::string observationMode_p;
    //! (List of) Oberservation target(s)
    std::string target_p;
    //! System time reference system
    std::string systemTime_p;
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
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, CommonAttributes.
    */
    inline std::string className () const {
      return "CommonAttributes";
    }

    /*!
      \brief Provide a summary of the internal status
    */
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
    //! Write the coordinate object to a HDF5 file
    void h5write (hid_t const &groupID);
    
    //! Write the coordinate object to a HDF5 file
    void h5write (hid_t const &locationID,
		  std::string const &name);
    
    //! Read the coordinate object from a HDF5 file
    void h5read (hid_t const &groupID);
    
    //! Read the coordinate object from a HDF5 file
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
  
