/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <BF_StokesDataset.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  BF_StokesDataset::BF_StokesDataset ()
    : HDF5Dataset()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param location  -- Identifier for the location at which the dataset is about
           to be created.
    \param name      -- Name of the dataset.
  */
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      std::string const &name)
    : HDF5Dataset(location,
		  name)
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param location  -- Identifier for the location at which the dataset is about
           to be created.
    \param name      -- Name of the dataset.
    \param shape     -- Shape of the dataset.
    \param component -- Stokes component stored within the dataset
    \param datatype  -- Datatype for the elements within the Dataset
  */
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      std::string const &name,
				      std::vector<hsize_t> const &shape,
				      DAL::Stokes::Component const &component,
				      hid_t const &datatype)
    : HDF5Dataset (location,
		   name,
		   shape,
		   datatype)
  {
    init (component);
  }
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  BF_StokesDataset::BF_StokesDataset (BF_StokesDataset const &other)
    : HDF5Dataset (other)
  {
    *this = other;
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BF_StokesDataset::~BF_StokesDataset ()
  {
    destroy();
  }
  
  void BF_StokesDataset::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another BF_StokesDataset object from which to make a copy.
  */
  BF_StokesDataset& BF_StokesDataset::operator= (BF_StokesDataset const &other)
  {
    if (this != &other) {
      destroy ();
      /* Copy internal parameters */
      itsStokesComponent = other.itsStokesComponent;
    }
    return *this;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void BF_StokesDataset::summary (std::ostream &os)
  {
    os << "[BF_StokesDataset] Summary of internal parameters."  << std::endl;
    os << "-- Stokes component       = " << itsStokesComponent.name() << std::endl;
    os << "-- Dataset name           = " << itsName             << std::endl;

    if (location_p) {
      os << "-- Dataset ID             = " << location_p          << std::endl;
      os << "-- Dataspace ID           = " << dataspaceID()       << std::endl;
      os << "-- Datatype ID            = " << datatypeID()        << std::endl;
      os << "-- Dataset rank           = " << rank()              << std::endl;
      os << "-- Dataset shape          = " << shape()             << std::endl;
      os << "-- Layout of the raw data = " << itsLayout           << std::endl;
      os << "-- Chunk size             = " << itsChunking         << std::endl;
      os << "-- nof. datapoints        = " << nofDatapoints()     << std::endl;
      os << "-- nof. active hyperslabs = " << itsHyperslab.size() << std::endl;
    }

    os << "-- nof. attributes        = " << attributes_p.size() << std::endl;
    os << "-- Attributes             = " << attributes_p        << std::endl;
  }
  
  //_____________________________________________________________________________
  //                                                                         init

  void BF_StokesDataset::init ()
  {
    /* Set up the list of attributes attached to the structure */
    setAttributes();

    /* Assign Stokes component parameter based on STOKES_COMPONENT attribute. */
    if (H5Iis_valid(location_p)) {
      std::string stokesComponent;
      
      if ( h5get_attribute (location_p, "STOKES_COMPONENT", stokesComponent) ) {
	itsStokesComponent.setType(stokesComponent);
      }
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void BF_StokesDataset::init (DAL::Stokes::Component const &component)
  {
    /* Basic initialization */
    init ();

    /* Store which Stokes component is represented */
    itsStokesComponent = DAL::Stokes(component);

    /* Initiaize attributes attached to the dataset */
    if (H5Iis_valid(location_p)) {
      std::string grouptype       = "Data";
      std::string datatype        = "float";
      std::string stokesComponent = itsStokesComponent.name();
      int nofSubbands             = shape()[0];
      int nofChannels             = shape()[1];

      h5set_attribute (location_p, "GROUPTYPE",         grouptype       );
      h5set_attribute (location_p, "DATATYPE",          datatype        );
      h5set_attribute (location_p, "STOKES_COMPONENT",  stokesComponent );
      h5set_attribute (location_p, "NOF_SUBBANDS",      nofSubbands     );
      h5set_attribute (location_p, "NOF_CHANNELS",      nofChannels     );
    }
    
  }
    
} // Namespace DAL -- end
