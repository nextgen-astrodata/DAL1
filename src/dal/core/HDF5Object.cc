/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <core/HDF5Object.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   HDF5Object
  
  HDF5Object::HDF5Object ()
  {
    itsFlags    = IO_Mode();
    itsLocation = -1;
  }
  
  //_____________________________________________________________________________
  //                                                                   HDF5Object

  /*!
    \param name  -- Name of the object.
    \param flags -- I/O mode flags.
  */
  HDF5Object::HDF5Object (std::string const &name,
			  IO_Mode const &flags)
  {
    itsFlags    = flags;
    itsLocation = openFile (name, flags);
  }
  
  //_____________________________________________________________________________
  //                                                                   HDF5Object
  
  /*!
    \param location -- File or group identifier.
    \param name     -- Path to the object, relative to \c location.
    \param flags    -- I/O mode flags.
  */
  HDF5Object::HDF5Object (hid_t const &location,
			  std::string const &name,
			  IO_Mode const &flags)
  {
    itsFlags    = flags;
    itsLocation = open (location, name, flags);
  }
  
  //_____________________________________________________________________________
  //                                                                   HDF5Object
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Object::HDF5Object (HDF5Object const &other)
    : itsLocation (-1)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Object::~HDF5Object ()
  {
    destroy();
  }
  
  void HDF5Object::destroy ()
  {
    close (itsLocation);
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another HDF5Object object from which to make a copy.
  */
  HDF5Object& HDF5Object::operator= (HDF5Object const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Object::copy (HDF5Object const &other)
  {
    itsLocation = other.itsLocation;

    if (H5Iis_valid(itsLocation)) {

      /*______________________________________________________________
	Returns a non-negative value if successful; otherwise returns
	a negative value.
      */
      herr_t status = H5Oincr_refcount(itsLocation);

      if (status<0) {
	std::cerr << "[HDF5Object::copy] Error incrementing object reference counter!"
		  << std::endl;
      }
    }
  }
  
  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Object::summary (std::ostream &os)
  {
    os << "[HDF5Object] Summary of internal parameters."          << std::endl;
    os << "-- Object name               = " << name()             << std::endl;
    os << "-- Object ID                 = " << itsLocation        << std::endl;
    os << "-- Object type               = " << objectType()       << std::endl;
    os << "-- Object type name          = " << objectName()       << std::endl;
    os << "-- Object address in file    = " << objectAddress()    << std::endl;
    os << "-- Reference count of object = " << referenceCount()   << std::endl;
    os << "-- Access time               = " << accessTime()       << std::endl;
    os << "-- Modification time         = " << modificationTime() << std::endl;
    os << "-- Change time               = " << changeTime()       << std::endl;
    os << "-- Birth time                = " << birthTime()        << std::endl;
    os << "-- nof. attached attributes  = " << nofAttributes()    << std::endl;
  }
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                               objectTypesMap
  
  std::map<H5I_type_t,std::string> HDF5Object::objectTypesMap ()
  {
    std::map<H5I_type_t,std::string> data;

    data[H5I_UNINIT]      = "H5I_UNINIT";       /* uninitialized type          */
    data[H5I_BADID]       = "H5I_BADID";        /* invalid Type                */
    data[H5I_FILE]        = "H5I_FILE";         /* File object type            */
    data[H5I_GROUP]       = "H5I_GROUP";        /* Group object type           */
    data[H5I_DATATYPE]    = "H5I_DATATYPE";     /* Datatype object type        */
    data[H5I_DATASPACE]   = "H5I_DATASPACE";    /* Dataspace object type       */
    data[H5I_DATASET]     = "H5I_DATASET";      /* Dataset object type         */
    data[H5I_ATTR]        = "H5I_ATTR";         /* Attribute object type       */
    data[H5I_REFERENCE]   = "H5I_REFERENCE";    /* Reference object type       */
    data[H5I_VFL]         = "H5I_VFL";          /* Virtual file layer type     */
    data[H5I_GENPROP_CLS] = "H5I_GENPROP_CLS";  /* Generic property list class */
    data[H5I_GENPROP_LST] = "H5I_GENPROP_LST";  /* Generic property lists      */
    data[H5I_ERROR_CLASS] = "H5I_ERROR_CLASS";  /* error classes               */
    data[H5I_ERROR_MSG]   = "H5I_ERROR_MSG";    /* error messages              */
    data[H5I_ERROR_STACK] = "H5I_ERROR_STACK";  /* error stacks                */
    
    return data;
  }

  //_____________________________________________________________________________
  //                                                                  objectTypes
  
  /*!
    \return types -- Array with types of HDF5 objects.
  */
  std::vector<H5I_type_t> HDF5Object::objectTypes ()
  {
    std::map<H5I_type_t,std::string> data = objectTypesMap();
    std::map<H5I_type_t,std::string>::iterator it;
    std::vector<H5I_type_t> vec;

    for (it=data.begin(); it!=data.end(); ++it) {
      vec.push_back(it->first);
    }

    return vec;
  }
  
  //_____________________________________________________________________________
  //                                                                  objectNames
  
  /*!
    \return names -- Array with names of HDF5 objects.
  */
  std::vector<std::string> HDF5Object::objectNames ()
  {
    std::map<H5I_type_t,std::string> data = objectTypesMap();
    std::map<H5I_type_t,std::string>::iterator it;
    std::vector<std::string> vec;

    for (it=data.begin(); it!=data.end(); ++it) {
      vec.push_back(it->second);
    }

    return vec;
  }

  //_____________________________________________________________________________
  //                                                                         name
  
  /*!
    \param location -- Object identifier whose name is to be determined.
    \return name    -- Name of the object identified by \c location.

    \b Note: Interestingly enough the underlying HDF5 library calls -- though
    intended for the same task -- a) have different signatures and b) will not
    work on all types of objects:
    \code
    ssize_t H5Iget_name( hid_t location, char *buffer, size_t size )
    ssize_t H5Fget_name (hid_t location, char *buffer, size_t size ) 
    ssize_t H5Aget_name (hid_t location, size_t size, char *buffer ) 
    \endcode
    The present method addresses these short-comings, providing a generic
    interface to retrieve the name of the object.
  */
  std::string HDF5Object::name (hid_t const &location)
  {
    std::string varObjectName;
    
    if (H5Iis_valid(location)) {

      ssize_t length;
      size_t bufferSize = 10;
      char *buffer      = new char[bufferSize];
      H5I_type_t varObjectType = objectType (location);

      /*______________________________________________________________
	Step 1: Determine the proper length of the name string
      */
      
      switch (varObjectType) {
      case H5I_FILE:
	length = H5Fget_name (location, buffer, bufferSize);
	break;
      case H5I_ATTR:
	length = H5Aget_name (location, bufferSize, buffer);
	break;
      default:
	length = H5Iget_name (location, buffer, bufferSize);
	break;
      };
      
      delete [] buffer;
      
      /*______________________________________________________________
	Step 2: Retrive the name of the object within the HDF5 file;
	if the object itself is a file, its name will be retrieved.
      */
      
      if (length>0) {

	/* Update the settings for the buffer */
	bufferSize = length+1;
	buffer     = new char[bufferSize];
	
	/* Retrive the name/path of the object */
	switch (varObjectType) {
	case H5I_FILE:
	  length = H5Fget_name (location, buffer, bufferSize);
	  break;
	case H5I_ATTR:
	  length = H5Aget_name (location, bufferSize, buffer);
	  break;
	default:
	  length = H5Iget_name (location, buffer, bufferSize);
	  break;
	};

	/* copy the contents of the buffer to the return variable ... */
	varObjectName = buffer;
	/* ... and release the allocated memory for the buffer */
	delete [] buffer;
      }
      
    } else {
      varObjectName = "";
    }
    
    return varObjectName;
  }

  //_____________________________________________________________________________
  //                                                                   objectName
  
  /*!
    \param location -- Object identifier whose type is to be determined.
    \return name    -- Object type as name if successful; otherwise H5I_BADID. 
   */
  std::string HDF5Object::objectName (hid_t const &location)
  {
    std::map<H5I_type_t,std::string> data         = objectTypesMap();
    std::map<H5I_type_t,std::string>::iterator it = data.find(H5Iget_type(location));
    
    if (it!=data.end()) {
      return it->second;
    } else {
      return "H5I_BADID";
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   objectType
  
  /*!
    \param location -- Object identifier whose type is to be determined. Valid
           types returned by the function are
	   - \c H5I_FILE -- File
	   - \c H5I_GROUP -- Group
	   - \c H5I_DATATYPE -- Datatype
	   - \c H5I_DATASPACE -- Dataspace
	   - \c H5I_DATASET -- Dataset
	   - \c H5I_ATTR -- Attribute
	   - \c H5I_BADID -- Invalid identifier, returned by the function if no
	   valid type can be determined or the identifier submitted is invalid
    \return type    -- Object type if successful; otherwise H5I_BADID. 
  */
  H5I_type_t HDF5Object::objectType (hid_t const &location)
  {
    std::map<H5I_type_t,std::string> data         = objectTypesMap();
    std::map<H5I_type_t,std::string>::iterator it = data.find(H5Iget_type(location));
    
    if (it==data.end()) {
      return H5I_BADID;
    } else {
      return it->first;
    }
  }

  //_____________________________________________________________________________
  //                                                                   objectType

  /*!
    \param location -- Indentifier for the object to which the object of \e name
           is attached to.
    \param name     -- Name of the object, who's type to determine.
    \return type    -- Object type if successful; otherwise H5I_BADID. 
  */
  H5I_type_t HDF5Object::objectType (hid_t const &location,
				     std::string const &name)
  {
    H5I_type_t result = H5I_BADID;

    /*______________________________________________________________
      H5Iis_valid(hid_t) determines whether an identifier is valid. 
      Returns TRUE if 'location' is valid and FALSE if invalid.
      Otherwise returns a negative value. 
    */

    if (H5Iis_valid(location)) {
      /*______________________________________________________________
	H5Lexists allows an application to determine whether the link
	name exists in the group or file specified with loc_id. The
	link may be of any type; only the presence of a link with that
	name is checked. Returns TRUE or FALSE if successful; otherwise
	returns a negative value. 
	However, H5Lexists will not detected an attribute, such that
	an extra check is required.
      */
      
      htri_t isAttribute = H5Aexists (location, name.c_str());
      htri_t isLink      = H5Lexists (location, name.c_str(), H5P_DEFAULT); 
      
      if (isAttribute) {
	/* Open attribute for verification */
	hid_t id = H5Aopen (location, name.c_str(), H5P_DEFAULT);
	if (H5Iis_valid(id)) {
	  result = H5I_ATTR;
	} else {
	  result = H5I_BADID;
	}
	H5Aclose (id);
      } else {
	if (isLink) {
	  /* [1] Open object */
	  hid_t id = H5Oopen (location,
	  		      name.c_str(),
	  		      H5P_DEFAULT);
	  /* [2] Get object type */
	  if (H5Iis_valid(id)) {
	    result = H5Iget_type (id);
	  } else {
	    std::cerr << "[HDF5Object::objectType] Failed to open object "
	  	      << name
	  	      << std::endl;
	    result = H5I_BADID;
	  }
	  /* [3] Close object */
	  HDF5Object::close (id);
	}   //   END -- if (isLink>0)
	else {
	  std::cerr << "[HDF5Object::objectType] Failed to get object type!"
		    << std::endl;
	  result = H5I_BADID;
	}
      }
    }   //   END -- if(H5Iis_valid(location))
    else {
      // std::cerr << "[HDF5Object::objectType] Invalid object identifier!"
      // 		<< std::endl;
      result = H5I_BADID;
    }
    
    return result;
  }

  //_____________________________________________________________________________
  //                                                                   objectInfo
  
  /*!
    \return class -- Returns datatype class identifier if successful; otherwise
    \c H5T_NO_CLASS (-1). 
    
    Valid class identifiers, as defined in \c H5Tpublic.h, are:
    <ul>
      <li>\c H5T_INTEGER
      <li>\c H5T_FLOAT
      <li>\c H5T_STRING
      <li>\c H5T_BITFIELD
      <li>\c H5T_OPAQUE
      <li>\c H5T_COMPOUND
      <li>\c H5T_REFERENCE
      <li>\c H5T_ENUM
      <li>\c H5T_VLEN
      <li>\c H5T_ARRAY 
    </ul>
    Note that the library returns \c H5T_STRING for both fixed-length and
    variable-length strings. 
  */
  H5T_class_t HDF5Object::datatypeClass (hid_t const &location)
  {
    H5T_class_t result;

    switch (objectType (location)) {
    case H5I_ATTR:
      {
	hid_t atype = H5Aget_type (location);
	result      = H5Tget_class (atype);
	H5Tclose (atype);
      }
      break;
    case H5I_DATASET:
      {
	hid_t atype = H5Dget_type (location);
	result      = H5Tget_class (atype);
	H5Tclose (atype);
      }
      break;
    case H5I_DATATYPE:
      result = H5Tget_class (location);
      break;
    default:
      result = H5T_NO_CLASS;
      break;
    };

    return result;
  }

  //_____________________________________________________________________________
  //                                                                   objectInfo
  
  /*!
    \param location -- Object identifier whose type is to be determined.
    \return info    -- Struct with metadata for the object
  */
  H5O_info_t HDF5Object::objectInfo  (hid_t const &location)
  {
    H5O_info_t info;

    if (H5Iis_valid(location)) {
      herr_t status = H5Oget_info (location,
				   &info);

      /* Returns a non-negative value if successful; otherwise returns a
	 negative value. */
      if (status<0){
	std::cerr << "[HDF5Object::objectInfo] Error retrieving object info!"
		  << std::endl;
      }
    }

    return info;
  }

  //_____________________________________________________________________________
  //                                                                   objectInfo

  /*!
    \param location -- File or group identifier.
    \param name     -- Path to the object, relative to \c location.
    \return info    -- Struct with metadata for the object
  */
  H5O_info_t HDF5Object::objectInfo  (hid_t const &location,
				      std::string const &name)
  {
    H5O_info_t info;

    if (H5Iis_valid(location)) {
      herr_t status = H5Oget_info_by_name (location,
					   name.c_str(),
					   &info,
					   H5P_DEFAULT);

      /* Returns a non-negative value if successful; otherwise returns a
	 negative value. */
      if (status<0){
	std::cerr << "[HDF5Object::objectInfo] Error retrieving object info!"
		  << std::endl;
      }
    }

    return info;
  }

  
  //_____________________________________________________________________________
  //                                                                objectAddress

  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  haddr_t HDF5Object::objectAddress (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      H5O_info_t info = objectInfo (location);
      return info.addr;
    } else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                               referenceCount
  
  /*!
    \param location -- Object identifier whose reference count will be retrieved.
    \return count   -- Returns a non-negative current reference count of the
            object identifier if successful; otherwise a negative value is
	    returned. 
  */
  int HDF5Object::referenceCount (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      H5O_info_t info = objectInfo (location);
      return info.rc;
    } else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   accessTime
  
  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  time_t HDF5Object::accessTime (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      H5O_info_t info = objectInfo (location);
      return info.atime;
    } else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                             modificationTime
  
  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  time_t HDF5Object::modificationTime (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      H5O_info_t info = objectInfo (location);
      return info.mtime;
    } else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   changeTime
  
  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  time_t HDF5Object::changeTime (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      H5O_info_t info = objectInfo (location);
      return info.ctime;
    } else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                    birthTime
  
  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  time_t HDF5Object::birthTime (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      H5O_info_t info = objectInfo (location);
      return info.btime;
    } else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                nofAttributes
  
  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  hsize_t HDF5Object::nofAttributes (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      if (objectType(location)==H5I_ATTR) {
	return  0;
      } else {
	H5O_info_t info = objectInfo (location);
	return info.num_attrs;
      }
    } else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param filename -- Name of the file to be opened.
    \param flags    -- I/O mode flags.
    \return fileID  -- HDF5 object identifier for the opened file; returns \e 0 
            in case the operation failed.
   */
  hid_t HDF5Object::openFile (std::string const &filename,
			  IO_Mode const &flags)
  {
    hid_t fileID = 0;

    /* Forward the function call */
    open (fileID, filename, flags);

    return fileID;
  }

  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \retvalfileID    --
    \param filename  -- 
    \param flags     --
    \return fileTruncated -- Was the file truncated? Returns \e true is this 
            was the case.
  */
  bool HDF5Object::openFile (hid_t &fileID,
			     std::string const &filename,
			     IO_Mode const &flags)
  {
    bool fileExists    = false;
    bool fileTruncated = false; 
    std::ifstream infile (filename.c_str(), std::ifstream::in);
    
    /*______________________________________________________
      Check if the file already exists
    */
    
    if (infile.is_open() && infile.good()) {
      fileExists = true;
    } else {
      fileExists = false;
    }
    infile.close();
    
    /*______________________________________________________
      Open or create file.
    */
    
    if (fileExists) {
      if ( flags.flags() & IO_Mode::Truncate ) {
	/* Truncate existing file */
	fileTruncated = true;
	fileID        = H5Fcreate (filename.c_str(),
				   H5F_ACC_TRUNC,
				   H5P_DEFAULT,
				   H5P_DEFAULT);
      } else if ( flags.flags() & IO_Mode::Create ) {
	/* Truncate existing file */
	fileTruncated = true;
	fileID        = H5Fcreate (filename.c_str(),
				   H5F_ACC_TRUNC,
				   H5P_DEFAULT,
				   H5P_DEFAULT);
      } else {
	if ( flags.flags() & IO_Mode::ReadWrite ) {
	  /* Open file as read/write */
	  fileTruncated = false;
	  fileID        = H5Fopen (filename.c_str(),
				   H5F_ACC_RDWR,
				   H5P_DEFAULT);
	} else {
	  /* Open file as read-only */
	  fileTruncated = false;
	  fileID        = H5Fopen (filename.c_str(),
				   H5F_ACC_RDONLY,
				   H5P_DEFAULT);
	}
      }
    } else {
      /* Create new file from scratch */
      fileTruncated = true;
      fileID        = H5Fcreate (filename.c_str(),
				 H5F_ACC_TRUNC,
				 H5P_DEFAULT,
				 H5P_DEFAULT);
    }

    return fileTruncated;
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    Generic method to open up an HDF5 object; uses several layers of checks --
    e.g. through \c H5Lexists and \c H5Lget_info -- before actually attempting
    to open object of given \e name attached to a \e location.
    
    \param location -- File or group identifier.
    \param name     -- Path to the object, relative to \c location.
    \param access   -- Access property list identifier for the link pointing to
           the object.
    \return id      -- Object identifier for \e name. Returns negative value
            in case opening of the object failed.
  */
  hid_t HDF5Object::open (hid_t const &location,
			  std::string const &name,
			  IO_Mode const &flags)
  {
    if (H5Iis_valid(location)) {
      /*____________________________________________________________
	H5Oopen opens the object in the same manner as H5Gopen,
	H5Topen, and H5Dopen. However, H5Oopen does not require the
	type of object to be known beforehand. This can be useful
	with user-defined links, for instance, when only a path may
	be known. H5Oopen cannot be used to open a dataspace,
	attribute, property list, or file. 
      */
      H5I_type_t otype = objectType (location, name);
      
      switch (otype) {
      case H5I_ATTR:
	return H5Aopen (location,
			name.c_str(),
			H5P_DEFAULT);
	break;
      case H5I_DATASET:
	return H5Dopen (location,
			name.c_str(),
			H5P_DEFAULT);
	break;
      case H5I_DATASPACE:
	std::cerr << "[HDF5Object::open] Unable to open object "
		  << name
		  << " - type is dataspace!"
		  << std::endl;
	return -1;
	break;
      case H5I_GROUP:
	return H5Gopen (location,
			name.c_str(),
			H5P_DEFAULT);
	break;
      case H5I_FILE:
	return openFile (name, flags);
	break;
      case H5I_BADID:
	std::cerr << "[HDF5Object::open] Unable to open object "
		  << name
		  << " - invalid identifier!"
		  << std::endl;
	return -1;
	break;
      default:
	return H5Oopen (location,
			name.c_str(),
			H5P_DEFAULT);
      };   //   END -- switch (otype)
    }   //   END -- H5Iis_valid(location)
    else {
      return -1;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- File or group identifier.
    \param name     -- Path to the object, relative to \c location.
    \param otype    -- Type of the object, to be exepcted. This parameter can
           be used for selection, only accepting a specific object type to
	   be opened up.
    \param access   -- Access property list identifier for the link pointing to
           the object.
    \return id      -- Object identifier for \e name. Returns negative value
            in case opening of the object failed, e.g. because the object did
	    not match \c otype.
  */
  hid_t HDF5Object::open (hid_t const &location,
			  std::string const &name,
			  H5I_type_t const &otype,
			  IO_Mode const &flags)
  {
    hid_t objectID = open (location, name, flags);
    
    if (otype != objectType(objectID)) {
      close (objectID);
      objectID = -1;
    }
    
    return objectID;
  }
  
  //_____________________________________________________________________________
  //                                                                        close
  
  /*!
    Though in principle there exists a generic \c H5Oclose() method, is turns
    out that this method unfortunately is not as generic as one might think:
    "H5Oclose is not used to close a dataspace, attribute, property list, or
    file." To overcome this shortcoming, a generic \c close() method is
    constructed, which interally is running the necessary checks before closing
    the HDF5 object in question.

    \param location -- HDF5 object identifier.
    \return status  -- Returns a non-negative value if successful; otherwise
            returns a negative value. 
   */
  herr_t HDF5Object::close (hid_t const &location)
  {
    herr_t status = -1;

    if (H5Iis_valid(location)) {

      H5I_type_t otype = H5Iget_type (location);
      
      /* _____________________________________________________________
	 WARNING: H5Oclose is not used to close a dataspace,
	 attribute, property list, or file - therefore the specific
	 methods for the object in question need to be called.
      */
      
      switch (otype) {
      case H5I_FILE:
	status = H5Fclose (location);
	break;
      case H5I_GROUP:
	status = H5Gclose (location);
	break;
      case H5I_DATATYPE:
	status  = H5Tclose (location);
	break;
      case H5I_DATASPACE:
	status = H5Sclose (location);
	break;
      case H5I_DATASET:
	status = H5Dclose (location);
	break;
      case H5I_ATTR:
	status = H5Aclose (location);
	break;
      case H5I_GENPROP_LST:
	status = H5Pclose (location);
	  break;
      case H5I_BADID:
	status = -1;
	break;
      default:
	status = H5Oclose (location);
	break;
      };   //  END -- switch (otype)
    }   //   END -- if (H5Iis_valid(location))
#ifdef DAL_DEBUGGING_MESSAGES
    else {
      std::cerr << "[HDF5Object::close] Invalid object identifier!"
		<< std::endl;
    }
#endif

    return status;
  }

  // ============================================================================
  //
  //  Call-back functions (used by H5Literate)
  //
  // ============================================================================

  /*!
    \param location -- Group that serves as root of the iteration; same value as
           the \e H5Lvisit \c location parameter.
    \param name     -- Name of link, relative to \c location, being examined at
           current step of the iteration.
    \param info     -- \c H5L_info_t struct containing information regarding 
           that link.
    \param op_data  -- User-defined pointer to data required by the application
           in processing the link; a pass-through of the \c op_data pointer
	   provided with the \e H5Lvisit function call.

    \return status -- The possible return values from the callback function,
            and the effect of each, are as follows: 
   */
  herr_t HDF5Object::H5Literate_showAttributes (hid_t location,
						const char *name,
						const H5L_info_t *info,
						void *op_data)
  {
    herr_t status (0);
    
    std::cout << "-- Location ID   = " << location   << std::endl;
    std::cout << "-- Relative link = " << name       << std::endl;
    std::cout << "-- Object type   = " << info->type << std::endl;

    if (op_data!=NULL) {
      std::cout << "-- op_data array = true" << std::endl;
    } else {
      std::cout << "-- op_data array = false" << std::endl;
    }

    return status;
  }
  
  
} // Namespace DAL -- end
