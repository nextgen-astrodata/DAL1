/***************************************************************************
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

#include <data_hl/RM_RootGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                 RM_RootGroup

  /*!
    \param filename -- Name of the dataset to open.
  */
  RM_RootGroup::RM_RootGroup (std::string const &filename)
    : HDF5GroupBase()
  {
    if (!open (0,filename,false)) {
      std::cerr << "[RM_RootGroup::RM_RootGroup] Failed to open file "
		<< filename
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 RM_RootGroup

  /*!
    \param filename -- Filename object from which the actual file name of the
           dataset is derived.
    \param create   -- Create the corresponding data structure, if it does not 
           exist yet?
  */
  RM_RootGroup::RM_RootGroup (DAL::Filename &infile,
			      IO_Mode const &flags)
    : HDF5GroupBase()
  {
    if (!open (0,infile.filename(),flags)) {
      std::cerr << "[RM_RootGroup::RM_RootGroup] Failed to open file "
		<< infile.filename()
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 RM_RootGroup

  /*!
    \param attributes -- CommonAttributes object from which the actual file name
           of the dataset is extracted.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
  */
  RM_RootGroup::RM_RootGroup (CommonAttributes const &attributes,
			      IO_Mode const &flags)
  {
    if (!open (0,attributes.filename(),flags)) {
      std::cerr << "[RM_RootGroup::RM_RootGroup] Failed to open file "
		<< attributes.filename()
		<< std::endl;
    }
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  RM_RootGroup::~RM_RootGroup ()
  {
    if (location_p > 0) {
      // clear maps with embedded objects
      sysLog_p.clear();
      // release HDF5 object
      herr_t h5error;
      H5I_type_t object_type = H5Iget_type(location_p);
      if (object_type == H5I_FILE) {
	h5error = H5Fclose(location_p);
	location_p = 0;
      }
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             commonAttributes
  
  CommonAttributes RM_RootGroup::commonAttributes ()
  {
    if (location_p > 0) {
      commonAttributes_p.h5read (location_p);
    }

    return commonAttributes_p;
  }

  //_____________________________________________________________________________
  //                                                          setCommonAttributes
  
  /*!
    \param attributes -- Set of LOFAR common attributes to be attached to the 
           root group of the beamformed dataset.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool RM_RootGroup::setCommonAttributes (CommonAttributes const &attributes)
  {
    bool status (true);

    try {
      commonAttributes_p = attributes;
    } catch (std::string message) {
      std::cerr << "[RM_RootGroup::setCommonAttributes] " << message << std::endl;
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void RM_RootGroup::summary (std::ostream &os,
			      bool const &showAttributes)
  {
    os << "[RM_RootGroup] Summary of internal parameters." << std::endl;
    os << "-- Filename                    = " << filename_p            << std::endl;
    os << "-- Location ID                 = " << location_p            << std::endl;
    os << "-- nof. attributes             = " << attributes_p.size()   << std::endl;
    os << "-- nof. SysLog groups          = " << sysLog_p.size()       << std::endl;
    
    if (showAttributes) {
      os << "-- Attributes              = " << attributes_p          << std::endl;
    }
  }
  

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void RM_RootGroup::init (CommonAttributes const &attributes)
  {
    /* Store the common attributes */
    setCommonAttributes (attributes);
    /* Set up the list of attributes attached to the root group */
    setAttributes();
    /* Try opening the file holding the dataset */
    if (!open (0,commonAttributes_p.filename(),true)) {
      std::cerr << "[RM_RootGroup::init] Initialization of object failed!"
		<< " Unable to access file holding the dataset!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void RM_RootGroup::setAttributes ()
  {
    attributes_p.clear();
    
    attributes_p.insert("IMGROUPS");
    attributes_p.insert("NOF_IMAGES");
    attributes_p.insert("TARGET_RA");
    attributes_p.insert("TARGET_DEC");
    attributes_p.insert("INPUT_FILE");
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
  bool RM_RootGroup::open (hid_t const &location,
			   std::string const &name,
			   IO_Mode const &flags)
  {
    bool status (true);
    
    // Initialize private variables ________________________
    location_p = location;
    filename_p = name;
    setAttributes();
    sysLog_p.clear();
    
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
      // Read the common LOFAR attributes
      commonAttributes_p.h5read(location_p);
    } else {
      /* If failed to open file, check if we are supposed to create one */
      if ( (flags.flags() & IO_Mode::Create) ||
	   (flags.flags() & IO_Mode::CreateNew) ) {
	location_p = H5Fcreate (name.c_str(),
				H5F_ACC_TRUNC,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* Write LOFAR common attribute to the root group of the file */
	commonAttributes_p.h5write(location_p);
	/* Write the additional attributes attached to the root group */
	std::string undefined ("UNDEFINED");
	//
	HDF5Attribute::write (location_p,"IMGROUPS",    bool(false) );
	HDF5Attribute::write (location_p,"NOF_IMAGES",  int(0)      );
	HDF5Attribute::write (location_p,"TARGET_RA",   float(0.0)  );
	HDF5Attribute::write (location_p,"TARGET_DEC",  float(0.0)  );
	HDF5Attribute::write (location_p,"INPUT_FILE",  undefined   );
	/* Read back in the common attributes after storing default values */
	commonAttributes_p.h5read(location_p);
      } else {
	std::cerr << "[RM_RootGroup::open] Failed to open file "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (flags);
    } else {
      std::cerr << "[RM_RootGroup::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \return status -- Status of the operation; returns <tt>False</tt> in case
            no primary pointing direction groups were found.
   */
  bool RM_RootGroup::openEmbedded (IO_Mode const &flags)
  {
    bool status = flags.flags();
    std::set<std::string> groups;
    std::set<std::string>::iterator it;

    /* Retrieve the names of the groups attached to the root group */
    status = h5get_names (groups,
			  location_p,
			  H5G_GROUP);
    
    /* Open system log group ... */
    status = openSysLog (flags);
    /* ... and remove its name from the previously retrieved list */
    groups.erase("SysLog");

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                   openSysLog
  
  bool RM_RootGroup::openSysLog (IO_Mode const &flags)
  {
    bool status = flags.flags();

    if (sysLog_p.size() == 0 && location_p > 0) {
      sysLog_p["SysLog"] = SysLog (location_p,flags);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                       sysLog
  
  SysLog RM_RootGroup::sysLog ()
  {
    std::map<std::string,SysLog>::iterator it;
    it = sysLog_p.begin();
    return it->second;
  }


} // Namespace DAL -- end
