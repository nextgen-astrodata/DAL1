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

#include <data_common/HDF5DatasetBase.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                              HDF5DatasetBase
  
  HDF5DatasetBase::HDF5DatasetBase ()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                              HDF5DatasetBase
  
  /*!
    \param location -- Object identifier for the location below which the
           dataset \c name is to be found.
    \param name     -- Name of the dataset.
    \param flags    -- I/O mode flags.
  */
  HDF5DatasetBase::HDF5DatasetBase (hid_t const &location,
				    std::string const &name,
				    IO_Mode const &flags)
    : HDF5Dataset (location, name, flags)
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                              HDF5DatasetBase
  
  /*!
    \param location -- Identifier for the location at which the dataset is about
           to be created.
    \param name     -- Name of the dataset.
    \param shape    -- Shape of the dataset.
    \param datatype -- Datatype for the elements within the Dataset
    \param flags    -- I/O mode flags.
  */
  HDF5DatasetBase::HDF5DatasetBase (hid_t const &location,
				    std::string const &name,
				    std::vector<hsize_t> const &shape,
				    hid_t const &datatype,
				    IO_Mode const &flags)
    : HDF5Dataset (location, name, shape, datatype,flags)
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                              HDF5DatasetBase
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5DatasetBase::HDF5DatasetBase (HDF5DatasetBase const &other)
    : HDF5Dataset (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5DatasetBase::~HDF5DatasetBase ()
  {
    destroy();
  }
  
  void HDF5DatasetBase::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another HDF5DatasetBase object from which to make a copy.
  */
  HDF5DatasetBase& HDF5DatasetBase::operator= (HDF5DatasetBase const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Another HDF5DatasetBase object from which to make a copy.
  */
  void HDF5DatasetBase::copy (HDF5DatasetBase const &other)
  {
    itsFlags      = other.itsFlags;
    itsAttributes = other.itsAttributes;
    itsGroupType  = other.itsGroupType;
    itsWCSinfo    = other.itsWCSinfo;
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
  void HDF5DatasetBase::summary (std::ostream &os)
  {
    os << "[HDF5DatasetBase] Summary of internal parameters." << std::endl;
    os << "-- GROUPTYPE        = " << itsGroupType << std::endl;
    os << "-- WCSINFO          = " << itsWCSinfo   << std::endl;
    os << "-- DATASET_NOF_AXES = " << nofAxes()    << std::endl;
    os << "-- DATASET_SHAPE    = " << shape()      << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  /*!  
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param flags  -- I/O mode flags, defining whether the dataset is opened as
           read-only or read-write.
  */
  bool HDF5DatasetBase::open (hid_t const &location,
			      std::string const &name,
			      IO_Mode const &flags)
  {
    /* Check the provided object identifier */
    if (H5Iis_valid(location)) {
      /* Check if link for dataset exists */
      htri_t h5err = H5Lexists (location,
				name.c_str(),
				H5P_DEFAULT);
      if (h5err>0) {
	itsFlags = flags;
	/* Open existing dataset */
	itsLocation = H5Dopen (location,
			       name.c_str(),
			       H5P_DEFAULT);
      } else {
	std::cerr << "[HDF5DatasetBase::open]"
		  << " Object " << name << " not found at provided location!"
		  << std::endl;
	return false;
      }
    } else {
	std::cerr << "[HDF5DatasetBase::open] Invalid object identifier!"
		  << std::endl;
	return false;
    }
    
    return true;
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- 
    \param name     -- 
    \param shape    -- 
    \param datatype -- 
    \param flags    -- I/O mode flags.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool HDF5DatasetBase::open (hid_t const &location,
			      std::string const &name,
			      std::vector<hsize_t> const &shape,
			      hid_t const &datatype,
			      IO_Mode const &flags)
  {
    bool status = HDF5Dataset::open (location,
				     name,
				     shape,
				     datatype,
				     flags);
    
    return status;
  }
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  std::string HDF5DatasetBase::getName (unsigned int const &index)
  {
    std::stringstream ss;

    ss << "Dataset_" << index;

    return ss.str();
  }

  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void HDF5DatasetBase::init (IO_Mode const &flags)
  {
    setAttributes ();

    itsFlags     = flags;
    itsGroupType = "Dataset";
    itsWCSinfo   = "";
  }
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void HDF5DatasetBase::setAttributes ()
  {
    itsAttributes.clear();

    itsAttributes.insert("GROUPTYPE");
    itsAttributes.insert("WCSINFO");
    itsAttributes.insert("DATASET_NOF_AXES");
    itsAttributes.insert("DATASET_SHAPE");
  }

} // Namespace DAL -- end
