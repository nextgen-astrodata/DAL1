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

#ifndef HDF5DATASET_H
#define HDF5DATASET_H

#include <iostream>
#include <string>
#include <vector>

#include <dalCommon.h>
#include <CommonInterface.h>
#include <HDF5Hyperslab.h>

namespace DAL {
  
  /*!
    \class HDF5Dataset

    \ingroup DAL
    \ingroup data_common

    \brief A class to encapsulate the operations required to work with a HDF5 dataset

    \author Lars B&auml;hren

    \date 2009/12/04
    
    \todo Implement read function to access data array within the dataset.

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>DAL::HDF5Hyperslab
    </ul>

    <h3>Synopsis</h3>

    Main encapsulated HDF5 library functions:
    <ol>
      <li>\b H5Dread
      \code
      herr_t H5Dread (hid_t dataset_id,
                      hid_t mem_type_id,
		      hid_t mem_space_id,
		      hid_t file_space_id,
		      hid_t xfer_plist_id,
		      void * buf) 
      \endcode
      H5Dread reads a (partial) dataset, specified by its identifier
      <i>dataset_id</i>, from the file into an application memory buffer
      <i>buf</i>. Data transfer properties are defined by the argument
      <i>xfer_plist_id</i>. The memory datatype of the (partial) dataset is
      identified by the identifier <i>mem_type_id</i>. The part of the dataset
      to read is defined by <i>mem_space_id</i> and <i>file_space_id</i>.

      <i>file_space_id</i> is used to specify only the selection within the
      file dataset's dataspace. Any dataspace specified in <i>file_space_id</i>
      is ignored by the library and the dataset's dataspace is always used.
      <i>file_space_id</i> can be the constant H5S_ALL. which indicates that 
      the entire file dataspace, as defined by the current dimensions of the
      dataset, is to be selected.

      <i>mem_space_id</i> is used to specify both the memory dataspace and the
      selection within that dataspace. <i>mem_space_id</i> can be the constant
      H5S_ALL, in which case the file dataspace is used for the memory dataspace
      and the selection defined with <i>file_space_id</i> is used for the
      selection within that dataspace.

      If raw data storage space has not been allocated for the dataset and a fill
      value has been defined, the returned buffer <i>buf</i> is filled with the
      fill value.

      Examples:
      \code
      H5Dread (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]);
      \endcode

      <li>\b H5Dwrite
      \code
      herr_t H5Dwrite (hid_t dataset_id,
                       hid_t mem_type_id,
		       hid_t mem_space_id,
		       hid_t file_space_id,
		       hid_t xfer_plist_id,
		       const void * buf) 
      \endcode
      H5Dwrite writes a (partial) dataset, specified by its identifier
      <i>dataset_id</i>, from the application memory buffer <i>buf</i> into the
      file. Data transfer properties are defined by the argument
      <i>xfer_plist_id</i>. The memory datatype of the (partial) dataset is
      identified by the identifier <i>mem_type_id</i>. The part of the dataset to
      write is defined by <i>mem_space_id</i> and <i>file_space_id</i>.
      
      <i>file_space_id</i> is used to specify only the selection within the file
      dataset's dataspace. Any dataspace specified in <i>file_space_id</i> is
      ignored by the library and the dataset's dataspace is always used.
      <i>file_space_id</i> can be the constant H5S_ALL. which indicates that the
      entire file dataspace, as defined by the current dimensions of the dataset,
      is to be selected.

      <i>mem_space_id</i> is used to specify both the memory dataspace and the
      selection within that dataspace. <i>mem_space_id</i> can be the constant
      H5S_ALL, in which case the file dataspace is used for the memory dataspace
      and the selection defined with <i>file_space_id</i> is used for the
      selection within that dataspace. 
    </ol>
      
    <h3>Example(s)</h3>

    <ol>
      <li>Open dataset \e Array1D attached to the root group of a file:
      \code
      // Open the HDF5 file
      hid_t fileID = H5Fopen (filename.c_str(),
                              H5F_ACC_RDWR,
			      H5P_DEFAULT);

      // Open the dataset
      std::string name ("Array1D");
      DAL::HDF5Dataset dataset (fileID,
                                name);
      \endcode
    </ol>
    
  */
  class HDF5Dataset : public CommonInterface {
    
    //! Name of the dataset
    std::string name_p;
    //! Dataspace identifier
    hid_t dataspace_p;
    //! Datatype identifier
    hid_t datatype_p;
    //! Shape of the dataset
    std::vector<hsize_t> shape_p;
    //! Hyperslab for the dataspace attached to the dataset
    std::vector<DAL::HDF5Hyperslab> hyperslab_p;
    
  public:
    
    // === Construction =========================================================

    //! Default constuctor
    HDF5Dataset ();
    
    //! Argumented constructor
    HDF5Dataset (hid_t const &location,
		 std::string const &name);
    
    //! Argumented constructor
    HDF5Dataset (hid_t const &location,
		 std::string const &name,
		 std::vector<hsize_t> const &shape,
		 hid_t const &datatype=H5T_NATIVE_DOUBLE);
    
    // === Destruction ==========================================================
    
    // Destructor
    ~HDF5Dataset ();
    
    // === Parameter access =====================================================
    
    //! Get the name of the dataset
    inline std::string name () const {
      return name_p;
    }
    
    //! Get the shape of the dataset
    inline std::vector<hsize_t> shape () const {
      return shape_p;
    }
    
    //! Get the rank (i.e. the number of axes) of the dataset
    inline unsigned int rank () const {
      return shape_p.size();
    }

    //! Get the nof. datapoints (i.e. array elements) of the dataset
    unsigned int nofDatapoints ();
    
    //! Get the dataspace identifier
    inline hid_t dataspaceID () const {
      return dataspace_p;
    }
    
    //! Get the datatype identifier
    inline hid_t datatypeID () const {
      return datatype_p;
    }

    // === Public Methods =======================================================
    
    //! Open the dataset
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=false);
    
    //! Open the dataset
    bool open (hid_t const &location,
	       std::string const &name,
	       std::vector<hsize_t> const &shape,
	       hid_t const &datatype=H5T_NATIVE_DOUBLE);

    //! Select a hyperslab for the dataspace attached to the dataset
    bool setHyperslab (std::vector<int> const &start,
		       std::vector<int> const &count,
		       H5S_seloper_t const &selection=H5S_SELECT_SET);
    
    //! Select a hyperslab for the dataspace attached to the dataset
    bool setHyperslab (std::vector<int> const &start,
		       std::vector<int> const &stride,
		       std::vector<int> const &count,
		       std::vector<int> const &block,
		       H5S_seloper_t const &selection=H5S_SELECT_SET);

    // === Read the data ========================================================

    template <class T>
      bool readData (T data[],
		     HDF5Hyperslab &slab,
		     hid_t const &datatype)
      {
	bool status (true);
	
	/* Set the Hyperslab for the dataspace attached to a dataset */
	status = slab.setHyperslab (dataspace_p);
	
	if (status) {
	  /* Local variables */
	  unsigned int nelem (rank());
	  hsize_t dimensions[nelem];
	  herr_t h5error;
	  std::vector<int> stride = slab.stride();
	  std::vector<int> block  = slab.block();
	  /* Setup the memory space */
	  for (unsigned int n(0); n<nelem; ++n) {
	    dimensions[n] = block[n];
	  }
	  hid_t memorySpace = H5Screate_simple (nelem,
						dimensions,
						NULL);
	  /* Read the data from the dataset */
	  h5error = H5Dread (location_p,
			     datatype,
			     memorySpace,
			     dataspace_p,
			     H5P_DEFAULT,
			     data);
	  /* Release HDF5 object identifier */
	  H5Sclose (memorySpace);
	} else {
	  std::cerr << "[HDF5Dataset::readData] Failed to properly set up Hyperslab!"
		    << std::endl;
	  status = false;
	}
	
	return status;
      }

    /*!
      \brief Read the data
      \param data    -- Array with the data to be written.
      \param slab    -- Hyberslab defining a selection of the data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (T data[],
		     HDF5Hyperslab &slab);

    /*!
      \brief Read the data
      \param data    -- Array with the data to be written.
      \param count   -- Number of \e blocks to read.
      \param block   -- Shape of the data array.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (T data[],
		     std::vector<int> const &start,
		     std::vector<int> const &count,
		     std::vector<int> const &block)
      {
	std::vector<int> stride (block.size(),1);
	HDF5Hyperslab slab (shape_p,
			    start,
			    stride,
			    count,
			    block);
	return readData (data,
			  slab);
      }

    /*!
      \brief Read the data
      \param data    -- Array with the data to be written.
      \param block   -- Shape of the data array.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (T data[],
		     std::vector<int> const &start,
		     std::vector<int> const &block)
      {
	std::vector<int> count (block.size(),1);
	return readData (data,
			  start,
			  count,
			  block);
      }

    /*!
      \brief Read the data
      \param data    -- Array with the data to be written.
      \param block   -- Shape of the data array.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (T data[],
		     std::vector<int> const &block)
      {
	std::vector<int> start (block.size(),0);
	return readData (data,
			 start,
			 block);
      }
    
    // === Write the data =======================================================

    /*!
      \brief Write the data
      \param data    -- Array with the data to be written.
      \param slab    -- Hyberslab defining a selection of the data.
      \param datatype -- Datatype of the array elements.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool writeData (T const data[],
		      HDF5Hyperslab &slab,
		      hid_t const &datatype)
      {
	bool status (true);
	
	/* Set the Hyperslab for the dataspace attached to a dataset */
	status = slab.setHyperslab (dataspace_p);
	
	if (status) {
	  /* Local variables */
	  unsigned int nelem (rank());
	  hsize_t dimensions[nelem];
	  herr_t h5error;
	  std::vector<int> block = slab.block();
	  /* Setup the memory space */
	  for (unsigned int n(0); n<nelem; ++n) {
	    dimensions[n] = block[n];
	  }
	  hid_t memorySpace = H5Screate_simple (nelem,
						dimensions,
						NULL);
	  /* Read the data from the dataset */
	  h5error = H5Dwrite (location_p,
			      datatype,
			      memorySpace,
			      dataspace_p,
			      H5P_DEFAULT,
			      data);
	  /* Release HDF5 object identifier */
	  H5Sclose (memorySpace);
	} else {
	  std::cerr << "[HDF5Dataset::writeDate] Failed to properly set up Hyperslab!"
		    << std::endl;
	  status = false;
	}
	
	return status;
      }

    /*!
      \brief Write the data
      \param data    -- Array with the data to be written.
      \param slab    -- Hyberslab defining a selection of the data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool writeData (T const data[],
		      HDF5Hyperslab &slab);
    
    /*!
      \brief Write the data
      \param data    -- Array with the data to be written.
      \param count   -- Number of \e blocks to read.
      \param block   -- Shape of the data array.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool writeData (T const data[],
		      std::vector<int> const &start,
		      std::vector<int> const &count,
		      std::vector<int> const &block)
      {
	std::vector<int> stride (block.size(),1);
	HDF5Hyperslab slab (shape_p,
			    start,
			    stride,
			    count,
			    block);
	return writeData (data,
			  slab);
      }

    /*!
      \brief Write the data
      \param data    -- Array with the data to be written.
      \param block   -- Shape of the data array.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool writeData (T const data[],
		      std::vector<int> const &start,
		      std::vector<int> const &block)
      {
	std::vector<int> count (block.size(),1);
	return writeData (data,
			  start,
			  count,
			  block);
      }

    /*!
      \brief Write the data
      \param data    -- Array with the data to be written.
      \param block   -- Shape of the data array.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool writeData (T const data[],
		      std::vector<int> const &block)
      {
	std::vector<int> start (block.size(),0);
	return writeData (data,
			  start,
			  block);
      }
    
    // ==========================================================================
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os);

  private:
    
    //! Initialize the internal parameters
    void init ();
    //! Set up the list of attributes attached to the dataset
    void setAttributes ();
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);
    
  }; // end class HDF5Dataset
  
} // end namespace DAL

#endif
