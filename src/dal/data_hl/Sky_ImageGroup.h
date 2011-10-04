/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#ifndef SKY_IMAGEGROUP_H
#define SKY_IMAGEGROUP_H

// Standard library header files
#include <iostream>
#include <string>

/* DAL header files */
#include <coordinates/CoordinatesGroup.h>
#include <data_common/HDF5GroupBase.h>
#include <data_hl/Sky_ImageDataset.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Sky_ImageGroup
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class DAL::Sky_ImageGroup
    
    \author Lars B&auml;hren

    \date 2011-09-19

    \test tSky_ImageGroup.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>

    \verbatim
    L1002977_sky.h5                         ...  (Root) Group
    |-- SYS_LOG                             ...  Group
    |-- IMAGE_000                           ...  Group
    |   |-- GROUPTYPE                       ...  Attr.          string
    |   |-- REFERENCE_FREQUENCY_VALUE       ...  Attr.          double
    |   |-- REFERENCE_FREQUENCY_UNIT        ...  Attr.          string
    |   |-- REFERENCE_BANDWIDTH_VALUE       ...  Attr.          double
    |   |-- REFERENCE_BANDWIDTH_UNIT        ...  Attr.          string
    |   |-- EFFECTIVE_FREQUENCY_VALUE       ...  Attr.          double
    |   |-- EFFECTIVE_FREQUENCY_UNIT        ...  Attr.          string
    |   |-- EFFECTIVE_BANDWIDTH_VALUE       ...  Attr.          double
    |   |-- EFFECTIVE_BANDWIDTH_UNIT        ...  Attr.          string
    |   |-- COORDINATES                     ...  Group
    |   |   |-- COORDINATE_0                ...  Group
    |   |   |-- COORDINATE_1                ...  Group
    |   |   `-- COORDINATE_2                ...  Group
    |   |-- IMAGE_DATA                      ...  Dataset
    |   |-- SOURCE_TABLE                    ...  Dataset
    |   |-- PSF_INFO                        ...  Group
    |   `-- PROCESS_HISTORY                 ...  Group
    |-- IMAGE_001                           ...  Group
    |
    `-- IMAGE{NNN}                          ...  Group
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class Sky_ImageGroup : public HDF5GroupBase {

    //! Coordinates group
    CoordinatesGroup itsCoordinates;
    //! Image dataset storing the actual pixel data
    Sky_ImageDataset itsDataset;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    Sky_ImageGroup ();
    //! Argumented constructor
    Sky_ImageGroup (hid_t const &location,
		    std::string const &name);
    //! Argumented constructor
    Sky_ImageGroup (hid_t const &location,
		    unsigned int const &index,
		    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    //! Copy constructor
    Sky_ImageGroup (Sky_ImageGroup const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~Sky_ImageGroup ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    Sky_ImageGroup& operator= (Sky_ImageGroup const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, Sky_ImageGroup.
    */
    inline std::string className () const {
      return "Sky_ImageGroup";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    /*!
      \brief Open an image group
      
      \param location -- Identifier of the location to which the to be opened
      structure is attached
      \param name     -- Name of the dataset to be opened.
      \param flags    -- FIle create mode.
	     
      \return status -- Status of the operation; returns <tt>false</tt> in case
              an error was encountered.
    */
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    
    // === Static methods =======================================================
    
    //! Convert image group index to name of the HDF5 group
    static std::string getName (unsigned int const &index);
    
    
  private:
    
    /*!
      \brief Open the structures embedded within the current one

      \param create -- Create the corresponding data structure, if it does not 
             exist yet?
      
      \return status -- Status of the operation; returns <tt>false</tt> in case
              an error was encountered.
    */
    bool openEmbedded (IO_Mode const &flags);
    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Unconditional copying
    void copy (Sky_ImageGroup const &other);
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Sky_ImageGroup -- end
  
} // Namespace DAL -- end

#endif /* SKY_IMAGEGROUP_H */
  
