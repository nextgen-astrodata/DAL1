/***************************************************************************
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

#include "BF_StokesDataset.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  BF_StokesDataset::BF_StokesDataset ()
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
  {
    init();
    open (location, name);
  }

  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param location -- Identifier for the location at which the dataset is about
           to be created.
    \param index    -- Indentifier for the Stokes dataset.
  */
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      unsigned int const &index)
  {
    init();
    std::string name = getName (index);

    std::cout << "BF_StokesDataset::BF_StokesDataset (hid_t,uint)" << std::endl;
    std::cout << "-- location = " << location << std::endl;
    std::cout << "-- index    = " << index    << std::endl;
    std::cout << "-- name     = " << name     << std::endl;

    open (location, name);
  }
  
  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param location    -- Identifier for the location at which the dataset is about
           to be created.
    \param index       -- Indentifier for the Stokes dataset.
    \param nofSubbands -- Number of sub-bands.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
  */
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      unsigned int const &index,
				      unsigned int const &nofSubbands,
				      unsigned int const &nofChannels,
				      DAL::Stokes::Component const &component,
				      hid_t const &datatype)
  {
    itsName     = getName(index);
    itsDatatype = datatype;

    std::cout << "[BF_StokesDataset(hid_t,string,uint,uint,Stokes::Component,hid_t]"
	      << std::endl;

    open (location,
	  component,
	  nofSubbands,
	  nofChannels);
  }

  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param location    -- Identifier for the location at which the dataset is
           about to be created.
    \param name        -- Name of the dataset.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofSubbands -- Number of sub-bands.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
  */
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      unsigned int const &index,
				      unsigned int const &nofSamples,
				      unsigned int const &nofSubbands,
				      unsigned int const &nofChannels,
				      DAL::Stokes::Component const &component,
				      hid_t const &datatype)
  {
    itsName     = getName(index);
    itsDatatype = datatype;

    open (location,
	  component,
	  nofSamples,
	  nofSubbands,
	  nofChannels);
  }

  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param location    -- Identifier for the location at which the dataset is
           about to be created.
    \param name        -- Name of the dataset.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofChannels -- Number of channels within the individual subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
  */
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      unsigned int const &index,
				      unsigned int const &nofSamples,
				      std::vector<unsigned int> const &nofChannels,
				      DAL::Stokes::Component const &component,
				      hid_t const &datatype)
  {
    itsName     = getName(index);
    itsDatatype = datatype;

    open (location,
    	  component,
    	  nofSamples,
    	  nofChannels);
  }


  //_____________________________________________________________________________
  //                                                             BF_StokesDataset
  
  /*!
    \param location    -- Identifier for the location at which the dataset is
           about to be created.
    \param name        -- Name of the dataset.
    \param shape       -- [nofSamples,nofChannels] Shape of the dataset.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
  */
  BF_StokesDataset::BF_StokesDataset (hid_t const &location,
				      unsigned int const &index,
				      std::vector<hsize_t> const &shape,
				      DAL::Stokes::Component const &component,
				      hid_t const &datatype)
  {
    itsName     = getName(index);
    itsDatatype = datatype;

    std::vector<unsigned int> nofChannels (1, shape[1]);

    open (location,      // HDF5 object ID
	  component,     // Component name
	  shape[0],      // nof. samples
	  nofChannels);  // nof. channels
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

    itsNofChannels.clear();
    itsNofChannels = other.itsNofChannels;
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
  {
    itsNofChannels.clear();
  }
  
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
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   nofSamples

  /*!
    \return nofSamples -- The number of bins along the time axis.
  */
  unsigned int BF_StokesDataset::nofSamples ()
  {
    if (itsShape.empty()) {
      return 0;
    } else {
      return itsShape[0];
    }
  }
  
  //_____________________________________________________________________________
  //                                                               nofFrequencies

  /*!
    \return nofFrequencies -- The number of bins along the frequency axis.
  */
  unsigned int BF_StokesDataset::nofFrequencies ()
  {
    if (itsShape.empty()) {
      if (itsNofChannels.empty()) {
	return 0;
      } else {
	unsigned int nelem (1);
	for (unsigned int n(0); n<itsNofChannels.size(); ++n) {
	  nelem *= itsNofChannels[n];
	}
	return nelem;
      }
    } else {
      return itsShape[1];
    }
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
    std::string stokesComponent = itsStokesComponent.name();

    os << "[BF_StokesDataset] Summary of internal parameters."  << std::endl;
    os << "-- Stokes component       = " << stokesComponent  << std::endl;
    os << "-- nof. channels          = " << nofChannels()    << std::endl;
    os << "-- Dataset name           = " << itsName          << std::endl;
    os << "-- Dataset ID             = " << itsLocation       << std::endl;
    os << "-- Dataspace ID           = " << dataspaceID()    << std::endl;
    os << "-- Datatype ID            = " << datatypeID()     << std::endl;
    os << "-- Dataset rank           = " << rank()           << std::endl;
    os << "-- Dataset shape          = " << shape()          << std::endl;
    
    if (itsLocation) {
      os << "-- Layout of the raw data = " << itsLayout           << std::endl;
      os << "-- Chunk size             = " << itsChunking         << std::endl;
      os << "-- nof. datapoints        = " << nofDatapoints()     << std::endl;
      os << "-- nof. active hyperslabs = " << itsHyperslab.size() << std::endl;
    }

    os << "-- nof. attributes        = " << itsAttributes.size() << std::endl;
    os << "-- Attributes             = " << itsAttributes        << std::endl;
  }
  
  //_____________________________________________________________________________
  //                                                                         init

  /*
    Initialization of object's internal parameters to default values.
  */
  void BF_StokesDataset::init ()
  {
    itsNofChannels.clear();
    
    /* Set up the list of attributes attached to the structure */
    setAttributes();
    
    /* Assign Stokes component parameter based on STOKES_COMPONENT attribute. */
    if (H5Iis_valid(itsLocation)) {
      std::string stokesComponent;
      unsigned int nofSubbands;
      unsigned int nofChannels;
      
      if ( h5get_attribute (itsLocation, "STOKES_COMPONENT", stokesComponent) ) {
	itsStokesComponent.setType(stokesComponent);
      }

      if ( h5get_attribute (itsLocation, "NOF_SUBBANDS", nofSubbands) ) {
	/* Store the number of sub-bands */
	itsNofChannels.resize(nofSubbands);
	/* Reyrieve number of channels per sub-band */
	if ( h5get_attribute (itsLocation, "NOF_CHANNELS", nofChannels) ) {
	  itsNofChannels = std::vector<unsigned int>(nofSubbands,nofChannels);
	}
      }
      
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_StokesDataset::open (hid_t const &location,
			       std::string const &name,
			       bool const &create)
  {
    bool status = HDF5Dataset::open (location, name, create);

    if (status) {
      init ();
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location    -- Identifier for the location at which the dataset is about
           to be created.
    \param component   -- Stokes component stored within the dataset
    \param nofSamples  -- 
    \param nofSubbands -- Number of sub-bands.
    \param nofChannels -- Number of channels within the subbands.

    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool BF_StokesDataset::open (hid_t const &location,
			       DAL::Stokes::Component const &component,
			       unsigned int const &nofSamples,
			       unsigned int const &nofSubbands,
			       unsigned int const &nofChannels)
  {
    if (nofSubbands>0) {
      std::vector<unsigned int> tmp (nofSubbands,nofChannels);
      return open (location, component, nofSamples, tmp);
    } else {
      return false;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location    -- Identifier for the location at which the dataset is about
           to be created.
    \param component   -- Stokes component stored within the dataset
    \param nofSamples  -- Number of bins along the time axis.
    \param nofChannels -- Number of channels within the individual subbands.

    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool BF_StokesDataset::open (hid_t const &location,
			       DAL::Stokes::Component const &component,
			       unsigned int const &nofSamples,
			       std::vector<unsigned int> const &nofChannels)
  {
    bool status (true);
    std::vector<hsize_t> shape (2,1);

    /*________________________________________________________________
      Set up attributes attached to the dataset
    */
    setAttributes();
    itsStokesComponent = DAL::Stokes(component);

    /*________________________________________________________________
      Process the parameters defining the (initial) shape of the 
      dataset.
    */

    /* Time axis of the dataset */
    if (nofSamples>1) {
      shape[0] = nofSamples;
    } else {
      shape[0] = 1;
    }

    /* Frequency axis of the dataset */
    if (nofChannels.empty()) {
      itsNofChannels.resize(1);
      itsNofChannels[0] = 1;
      shape[1]          = 1;
    } else {
      itsNofChannels.clear();
      itsNofChannels = nofChannels;
      for (unsigned int n(0); n<nofChannels.size(); ++n) {
	shape[1] += nofChannels[n];
      }
    }

    /* Open/create dataset */
    status = HDF5Dataset::create (location, itsName, shape, itsDatatype);

    /* Initialize attributes attached to the dataset */
    if (H5Iis_valid(itsLocation)) {
      std::string grouptype       = "Data";
      std::string datatype        = "float";
      std::string stokesComponent = itsStokesComponent.name();
      std::vector<unsigned int> channels = itsNofChannels;
      unsigned int subbands              = channels.size();
      
#ifdef DAL_DEBUGGING_MESSAGES
      std::cout << "[BF_StokesDataset::init]"                  << std::endl;
      std::cout << "-- GROUPTYPE        = " << grouptype       << std::endl;
      std::cout << "-- DATATYPE         = " << datatype        << std::endl;
      std::cout << "-- STOKES_COMPONENT = " << stokesComponent << std::endl;
      std::cout << "-- NOF_SAMPLES      = " << itsShape[0]     << std::endl;
      std::cout << "-- NOF_SUBBANDS     = " << subbands        << std::endl;
      std::cout << "-- NOF_CHANNELS     = " << channels        << std::endl;
#endif
      
      HDF5Attribute::write (itsLocation, "GROUPTYPE",        grouptype       );
      HDF5Attribute::write (itsLocation, "DATATYPE",         datatype        );
      HDF5Attribute::write (itsLocation, "STOKES_COMPONENT", stokesComponent );
      HDF5Attribute::write (itsLocation, "NOF_SAMPLES",      itsShape[0]     );
      HDF5Attribute::write (itsLocation, "NOF_SUBBANDS",     subbands        );
      HDF5Attribute::write (itsLocation, "NOF_CHANNELS",     channels        );
    }

    return status;
  }

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the beam.
    \return name -- The name of the beam group, <tt>STOKES_<index></tt>
  */
  std::string BF_StokesDataset::getName (unsigned int const &index)
  {
    std::stringstream ss;

    ss << "STOKES_" << index;

    return ss.str();
  }
  
  
} // Namespace DAL -- end
