/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

#include <core/dalGroup.h>

namespace DAL {
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                     dalGroup
  
  dalGroup::dalGroup()
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                     dalGroup
  
  /*!
    Create a group in a certain file.
    
    \param groupname The name of the group to create.
    \param file A pointer to the file where you want to create the group.
  */
  dalGroup::dalGroup (const char * gname,
		      void * voidfile)
  {
    init ();

    hid_t * lclfile = (hid_t*)voidfile; // H5File object
    file            = lclfile;
    itsFileID         = *lclfile;  // get the file handle

    itsName    = gname;
    groupname_full = "/" + stringify(gname);
    if ( ( itsGroupID = H5Gcreate( itsFileID, groupname_full.c_str(),  H5P_DEFAULT,
                                 H5P_DEFAULT, H5P_DEFAULT ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create group'" << groupname_full
                  << "'.\n";
      }

  }


  //_____________________________________________________________________________
  //                                                                     dalGroup

  /*!
    Create a subgroup in an existing group.
    
    \param group_id The parent group identifier.
    \param gname The name of the subgroup.
  */
  dalGroup::dalGroup (hid_t location_id,
		      const char * gname )
  {
    init();
    
    if ( ( itsGroupID = H5Gcreate( location_id, gname, H5P_DEFAULT, H5P_DEFAULT,
                                 H5P_DEFAULT ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create group'" << std::string( gname )
                  << "'.\n";
      }
  }


  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param file A pointer to the file.
    \param groupname The name of the group you want to open.
    \return An identifier for the new group.
   */
  int dalGroup::open (void * voidfile,
		      std::string gname)
  {
    hid_t * lclfile = (hid_t*)voidfile; // H5File object
    file            = lclfile;
    itsFileID         = *lclfile;  // get the file handle
    itsName     = gname;

    groupname_full = "/" + itsName;
    if ( ( itsGroupID = H5Gopen ( itsFileID, groupname_full.c_str(), H5P_DEFAULT ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create group'" << groupname_full
                  << "'.\n";
        return 0;
      }

    return( itsGroupID );
  }

  //_____________________________________________________________________________
  //                                                                        close

  bool dalGroup::close()
  {
    if ( 0 != itsGroupID ) {
      if ( H5Gclose(itsGroupID) < 0 ) {
	std::cerr << "ERROR: dalGroup::close() failed.\n";
	return DAL::FAIL;
      }
      itsGroupID = 0;
    }
    return DAL::SUCCESS;
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  dalGroup::~dalGroup()
  {
    if ( 0 != itsGroupID ) {
      if ( H5Gclose(itsGroupID) < 0 ) {
	std::cerr << "ERROR: dalGroup::close() failed.\n";
      }
      itsGroupID = 0;
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                        getId
  
  /*!
    \return The group identifier as an integer.
  */
  hid_t dalGroup::getId()
  {
    return itsGroupID;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void dalGroup::init () 
  {
    file           = NULL;
    itsName    = "UNKNOWN";
    groupname_full = "UNKNOWN";
    group          = NULL;
    filter         = NULL;
    itsFileID      = 0;
    itsGroupID     = 0;
    status         = 0;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary will be written.
  */
  void dalGroup::summary(std::ostream &os)
  {
    os << "[dalGroup] Summary of object properties"  << endl;
    
    os << "-- File ID            = " << itsFileID     << std::endl;
    os << "-- Group ID           = " << itsGroupID    << std::endl;
    os << "-- Group name         = " << itsName << std::endl;
    os << "-- Status             = " << status      << std::endl;
    
    std::vector<std::string> memberNames = getMemberNames();
    os << "-- nof. group members = " << memberNames.size() << std::endl;
    os << "-- Member names       = [";
    for (unsigned int n(0); n<memberNames.size(); n++)
      {
        os << " " << memberNames[n];
      }
    os << " ]" << std::endl;
    
  }

  // -------------------------------------------------- dalGroup_file_info

  /*
   * Operator function.
   */
  herr_t dalGroup_file_info(hid_t loc_id, const char *name, void *opdata)
  {
    H5G_stat_t statbuf;

    /*
     * Get type of the object and display its name and type.
     * The name of the object is passed to this function by
     * the Library. Some magic :-)
     */
    if ( H5Gget_objinfo(loc_id, name, false, &statbuf) > 0 )
      {
        std::cerr << "ERROR: Could not get group object info.\n";
        return 1;
      }

    switch (statbuf.type)
      {
      case H5G_DATASET:
        (*(std::vector<std::string>*)opdata).push_back( std::string(name) );
        break;
      case H5G_GROUP:
      case H5G_TYPE:
      default:
        break;
      }

    return 0;
  }

  //_____________________________________________________________________________
  //                                                               getMemberNames

  /*!
    \return memberNames -- A vector of strings representing the member names; if
            the group contains no members or is invalid itself, an empty list will
	    be returned.
  */
  std::vector<std::string> dalGroup::getMemberNames ()
  {
    std::vector<std::string> member_names;
    
    if (H5Iis_valid(itsFileID)) {
      H5Giterate (itsFileID,
		  groupname_full.c_str(),
		  NULL,
		  dalGroup_file_info,
		  &member_names);
    }
    
    return member_names;
  }
  
  //_____________________________________________________________________________
  //                                                             createShortArray
  
  /*!
    Create an array of shorts with any dimensions.  This is usually
    called from the dataset object and not from the developer.
    
    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createShortArray( std::string arrayname,
                              std::vector<int> dims,
                              short data[],
                              std::vector<int> cdims )
  {
    dalShortArray * la;
    la = new dalShortArray( itsGroupID, arrayname, dims, data, cdims );
    return la;
  }

  //_____________________________________________________________________________
  //                                                               createIntArray
  
  /*!
    \brief Create an array of integers within the group.

    Create an array of integers with any dimensions.  This is usually
    called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createIntArray( std::string arrayname,
                            std::vector<int> dims,
                            int data[],
                            std::vector<int> cdims )
  {
    dalIntArray * la;
    la = new dalIntArray( itsGroupID, arrayname, dims, data, cdims );
    return la;
  }

  //_____________________________________________________________________________
  //                                                             createFloatArray
  
  /*!
    \brief Create an array of floating point values within the group.

    Create an array of floating point values with any dimensions.  This
    is usually called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createFloatArray( std::string arrayname,
                              std::vector<int> dims,
                              float data[],
                              std::vector<int> cdims )
  {
    dalFloatArray * la;
    la = new dalFloatArray( itsGroupID, arrayname, dims, data, cdims );
    return la;
  }

  //_____________________________________________________________________________
  //                                                      createComplexFloatArray
  
  /*!
    \brief Create an array of complex floating point values within the group.

    Create an array of complex floating point values with any dimensions.
    This is usually called from the dataset object and not from the
    developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createComplexFloatArray( std::string arrayname,
                                     std::vector<int> dims,
                                     std::complex<float> data[],
                                     std::vector<int> cdims )
  {
    dalComplexArray_float32 * la;
    la = new dalComplexArray_float32( itsGroupID, arrayname, dims, data, cdims );
    return la;
  }

  //_____________________________________________________________________________
  //                                                      createComplexShortArray
  
  /*!
    \brief Create an array of complex int16 values within the group.

    Create an array of complex int16 values with any dimensions.
    This is usually called from the dataset object and not from the
    developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createComplexShortArray( std::string arrayname,
                                     std::vector<int> dims,
                                     std::complex<Int16> data[],
                                     std::vector<int> cdims )
  {
    dalComplexArray_int16 * la;
    la = new dalComplexArray_int16( itsGroupID, arrayname, dims, data, cdims );
    return la;
  }

  //_____________________________________________________________________________
  //                                                      createComplexShortArray
  
  /*!
    \param name      -- Name of the group to create.
    \return dalGroup -- Object holding the newly created group.
  */
  dalGroup * dalGroup::createGroup (const char * name)
  {
    dalGroup * newGroup = NULL;
    newGroup = new dalGroup (itsGroupID, name);
    return newGroup;
  }

} // end namespace DAL
