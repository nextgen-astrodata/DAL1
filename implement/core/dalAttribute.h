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

#ifndef DALATTRIBUTE_H
#define DALATTRIBUTE_H

// Standard library header files
#include <iostream>
#include <string>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class dalAttribute
    
    \ingroup DAL
    
    \brief Collection of attributes/keywords used throughout the DAL
    
    \author Lars B&auml;hren

    \date 2009/09/02

    \test tdalAttribute.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR-USG-ICD-001 : TBB time-series data
      <li>LOFAR-USG-ICD-003 : LOFAR Beam-Formed Data Format ICD
      <li>LOFAR-USG-ICD-004 : LOFAR Sky Image
    </ul>
    
    <h3>Synopsis</h3>

    Given the number and intended standardization of attributes/keywords used
    throughout the various data-sets supported by the DAL, we need a possibility
    to keep track of entries, ensure their consistency and provide means for 
    translation (e.g. when converting parts of a HDF5 file to FITS).
    
    <h3>Example(s)</h3>
    
  */  
  class dalAttribute {

  public:

    /*!
      \brief Enumeration holding the list of attributes used throughout the DAL

      Even at the risk of getting quite extensive, there is a considerable 
      advantage of keeping a single enumeration list of all the attributes
      used throughout the DAL: by this choice not only we avoid having the
      definition of the various attributes spread across multiple files but
      also will be able to better enforce consistence of attribute names 
      throghout the complete code base.
    */
    enum Name {
      /************************************************************************
       * Common attributes to be attached to the root group of every data-set
       ************************************************************************/
      //! LOFAR group type (mostly applies to HDF5 data-sets)
      GROUPTYPE,
      //! Name of the file/data-set
      FILENAME,
      //! Type of the file/data-set
      FILETYPE,
      //! File creation date
      FILEDATE,
      //! Name of the telescope
      TELESCOPE,
      //! Unique identifier for the project
      PROJECT_ID,
      //! Name of the project
      PROJECT_NAME,
      //! Brief description of the project
      PROJECT_DESCRIPTION,
      //! Name of the observer
      OBSERVER,
      //! Unique identifier for the observation
      OBSERVATION_ID,
      //! Date of the observation
      OBSERVATION_DATE,
      //! Filter selection
      FILTER_SELECTION,
      //! Name of the observation target
      TARGET,
      SYSTEM_TIME,
      SYSTEM_VERSION,
      PIPELINE_NAME,
      PIPELINE_VERSION,
      //! Number of stations used for the observation
      NOF_STATIONS,
      //! Names of the stations used for the observation
      STATIONS_LIST,
      NOTES,
      /************************************************************************
       * Attributes required for TBB data
       ************************************************************************/
      //! Identifier of a LOFAR station
      STATION_ID,
      //! Identifier of a RSP board at a LOFAR station
      RSP_ID,
      //! Identifier of a receiver unit (RCU) at a LOFAR station
      RCU_ID,
      //! Numerical value of the A/D sampling frequency
      SAMPLE_FREQUENCY_VALUE,
      //! Physical unit within which the sampling frequency is given
      SAMPLE_FREQUENCY_UNIT,
      //! Numerical value of the antenna position
      ANTENNA_POSITION_VALUE,
      //! Physical unit within which the antenna position is given
      ANTENNA_POSITION_UNIT,
      //! Identifier for the reference frame of the antenna position
      ANTENNA_POSITION_FRAME,
      //! Numerical value of the antenna orientation
      ANTENNA_ORIENTATION_VALUE,
      //! Physical unit within which the antenna orientation is given
      ANTENNA_ORIENTATION_UNIT,
      //! Identifier for the reference frame of the antenna orientation
      ANTENNA_ORIENTATION_FRAME
    };
    
  private:
    
    Name attributeID_p;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    dalAttribute ();
    
    //! Argumented constructor
    dalAttribute (Name const &id);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another dalAttribute object from which to create this new
             one.
    */
    dalAttribute (dalAttribute const &other);
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~dalAttribute ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another dalAttribute object from which to make a copy.
    */
    dalAttribute& operator= (dalAttribute const &other); 
    
    // --------------------------------------------------------------- Parameters

    //! Get the (enumeration) type of the attribute
    inline Name getType () {
      return attributeID_p;
    }

    //! Get the name of an attribute
    inline std::string getName () {
      return getName (attributeID_p);
    }

    //! Get the FITS name of an attribute
    inline std::string getNameFITS () {
      return getNameFITS (attributeID_p);
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, dalAttribute.
    */
    inline std::string className () const {
      return "dalAttribute";
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
    
    //! Get the name of an attribute
    static std::string getName (Name const &id);
    
    //! Get the FITS name of an attribute
    static std::string getNameFITS (Name const &id);
    
  private:
    
    //! Unconditional copying
    void copy (dalAttribute const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class dalAttribute -- end
  
} // Namespace DAL -- end

#endif /* DALATTRIBUTE_H */
  
