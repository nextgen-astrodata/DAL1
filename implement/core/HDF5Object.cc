/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <HDF5Object.h>

using std::string;

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   HDF5Object
  
  HDF5Object::HDF5Object ()
    : itsLocation (-1)
  {;}
  
  //_____________________________________________________________________________
  //                                                                   HDF5Object
  
  HDF5Object::HDF5Object (std::string const &filename,
			  hid_t const &access)
    : itsLocation (-1)
  {
    open (filename, access);
  }
  
  //_____________________________________________________________________________
  //                                                                   HDF5Object
  
  /*!
    \param location -- File or group identifier.
    \param name     -- Path to the object, relative to \c location.
    \param access   -- Access property list identifier for the link pointing to
           the object.
  */
  HDF5Object::HDF5Object (hid_t const &location,
			  string const &name,
			  hid_t const &access)
  {
    itsLocation = open (location, name, access);
  }
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Object::HDF5Object (HDF5Object const &other)
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
  //  Public methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param filename -- Path to the object.
    \param access   -- Access property list identifier for the link pointing to
           the object.
    \return status  -- Status of the operation; returns \e false in case an error 
            was encountered, e.g. because no object \c name could be found.
  */
  bool HDF5Object::open (std::string const &filename,
			 hid_t const &access)
  {
    bool status (true);

    /* _______________________________________________________________
       Check if the file actually exists is this is not the case, 
       there is no use trying to open it up.
    */

    std::ifstream infile;
    infile.open (filename.c_str(), std::ifstream::in);
    
    if (infile.is_open() && infile.good()) {
      status = true;
    } else {
      status = false;
    }
    infile.close();
    
    /* _______________________________________________________________
       Case handling matrix to either open or create the HDF5 file
    */

    if (status) {
      
      if (access == hid_t(H5F_ACC_TRUNC)) {
	itsLocation = H5Fcreate (filename.c_str(),
				 access,
				 H5P_DEFAULT,
				 H5P_DEFAULT);
      } else {
	itsLocation = H5Fopen (filename.c_str(),
			       access,
			       H5P_DEFAULT);
      }
      
    } else {
    }
    
    return status;
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

    data[H5I_FILE]      = "H5I_FILE";
    data[H5I_GROUP]     = "H5I_GROUP";
    data[H5I_DATATYPE]  = "H5I_DATATYPE";
    data[H5I_DATASPACE] = "H5I_DATASPACE";
    data[H5I_DATASET]   = "H5I_DATASET";
    data[H5I_ATTR]      = "H5I_ATTR";
    data[H5I_BADID]     = "H5I_BADID";
    
    return data;
  }

  //_____________________________________________________________________________
  //                                                                  objectTypes
  
  /*!
    \return types -- Array with types of HDF5 objects.
  */
  std::vector<H5I_type_t> HDF5Object::objectTypes ()
  {
    std::map<H5I_type_t,string> data = objectTypesMap();
    std::map<H5I_type_t,string>::iterator it;
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
    std::map<H5I_type_t,string> data = objectTypesMap();
    std::map<H5I_type_t,string>::iterator it;
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
    std::map<H5I_type_t,string> data         = objectTypesMap();
    std::map<H5I_type_t,string>::iterator it = data.find(H5Iget_type(location));
    
    if (it!=data.end()) {
      return it->second;
    } else {
      return "H5I_BADID";
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   objectType
  
  /*!
    \param location -- Object identifier whose type is to be determined.
    \return type    -- Object type if successful; otherwise H5I_BADID. 
  */
  H5I_type_t HDF5Object::objectType (hid_t const &location)
  {
    std::map<H5I_type_t,string> data         = objectTypesMap();
    std::map<H5I_type_t,string>::iterator it = data.find(H5Iget_type(location));
    
    if (it!=data.end()) {
      return it->first;
    } else {
      return H5I_BADID;
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
    H5I_type_t result = objectType(location);
    
    if (result != H5I_BADID) {

      /*______________________________________________________________
	H5Lexists allows an application to determine whether the link
	name exists in the group or file specified with loc_id. The
	link may be of any type; only the presence of a link with that
	name is checked. Returns TRUE or FALSE if successful; otherwise
	returns a negative value. 
      */
      htri_t status = H5Lexists (location,
				 name.c_str(),
				 H5P_DEFAULT); 
      
      if (status>0) {
	/* Open the object ... */
	hid_t id = H5Oopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
	/* ... to extact its type */
	result = objectType (id);
	/* close object */
	close (id);
      } else {
	result = H5I_BADID;
      }
    }
    
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
      H5O_info_t info = objectInfo (location);
      return info.num_attrs;
    } else {
      return 0;
    }
  }

  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  std::set<std::string> HDF5Object::attributes (hid_t const &location)
  {
    std::set<std::string> names;
    
    if (H5Iis_valid(location)) {

      herr_t status;

      status = H5Literate (location,
			   H5_INDEX_NAME,
			   H5_ITER_NATIVE,
			   NULL,
			   H5Literate_showAttributes,
			   NULL);
      
    }
    
    return names;
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
			  hid_t const &access)
  {
    hid_t objectID (-1);
    
    if (H5Iis_valid(location)) {
      /*______________________________________________________________
	H5Lexists allows an application to determine whether the link
	name exists in the group or file specified with location. The
	link may be of any type; only the presence of a link with that
	name is checked. Returns TRUE or FALSE if successful;
	otherwise returns a negative value. 
      */
      htri_t errExists = H5Lexists (location,
				    name.c_str(),
				    access);
      if (errExists>0) {
	/*____________________________________________________________
	  H5Lget_info returns information about the specified link
	  through the buffer argument. Returns a non-negative value if
	  successful, with the fields of buffer (if non-null)
	  initialized. Otherwise returns a negative value.
	*/
	H5L_info_t buffer;
	herr_t errInfo = H5Lget_info (location,
				      name.c_str(),
				      &buffer,
				      access);
	if (errInfo>0) {
	  /*__________________________________________________________
	    H5Oopen opens the object in the same manner as
	    H5Gopen, H5Topen, and H5Dopen. However, H5Oopen does not
	    require the type of object to be known beforehand. This
	    can be useful with user-defined links, for instance, when
	    only a path may be known. H5Oopen cannot be used to open
	    a dataspace, attribute, property list, or file. 
	  */
	  H5I_type_t otype = objectType (location, name);
	  
	  switch (otype) {
	  case H5I_FILE:
	    objectID = H5Fopen (name.c_str(),
				access);
	    break;
	  case H5I_DATASET:
	    objectID = H5Dopen (location,
				name.c_str(),
				access);
	    break;
	  case H5I_GROUP:
	    objectID = H5Gopen (location,
				name.c_str(),
				access);
	    break;
	  default:
	    objectID = H5Oopen (location,
				name.c_str(),
				access);
	  };
	} else {
	  objectID = -1;
	}
      } else {
	objectID = -1;
      }
    }   //   END -- H5Iis_valid(location)
    else {
      objectID = -1;
    }
    
    return objectID;
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
			  hid_t const &access)
  {
    hid_t objectID = open (location, name, access);

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
    herr_t status (-1);

    if (H5Iis_valid(location)) {
      H5I_type_t otype = objectType (location);
      
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
      default:
	status = H5Oclose (location);
	break;
      }
    }

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
    
    if (objectType(location,name) == H5I_ATTR) {
      std::cout << "-- Found attribute " << name << std::endl;
    }
    
    return status;
  }
  
  
} // Namespace DAL -- end
