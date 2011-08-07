/***************************************************************************
 *   Copyright (C) 2006-2011                                               *
 *   Joseph Masters (jmasters@science.uva.nl)                              *
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

#ifndef DALDATASET_H
#define DALDATASET_H

#include <core/dalObjectBase.h>
#include <core/dalGroup.h>
#include <core/dalIntArray.h>
#include <core/HDF5Attribute.h>
#include <core/HDF5Object.h>

namespace DAL {
  
  /*!
    \class dalDataset

    \ingroup DAL
    \ingroup core

    \brief Represents the file containing all sub-structures (tables, arrays, etc.)

    \author Joseph Masters
    \author Lars B&auml;hren

    \test tdalDataset.cc
    
    <h3>Prerequisite</h3>

    <ul>
      <li><a href="http://www.hdfgroup.org/HDF5/doc/RM/PredefDTypes.html">HDF5
      Predefined Datatypes</a>
    </ul>

    <h3>Synopsis</h3>

    The dalDataset is the highest level container for dalData.  It may
    consist of one or more files on disk, each of which contain multiple tables,
    images and attributes.  These tables and images can be grouped.

    <h3>Examples</h3>

    <ol>
      <li>Create new object through default constuctor:
      \code
      #include <dal/dalDataset.h>

      DAL::dalDataset dataset;
      \endcode
      and display some of its internal properties
      \code
      dataset.summary();
      \endcode
      <li>Create object for an already existing dataset on disk:
      \code
      std::string filename ("mydata.h5");

      DAL::dalDataset dataset (filename.c_str(),"HDF5");
      \endcode
      <li>Obtain a summary of the object's internal properties:
      \code
      dataset.summary();
      \endcode
      The output will look something like this:
      \verbatim
      [dalDataset] Summary of object properties
      -- Dataset type     = HDF5
      -- Dataset name     = /Users/lars/Code/lofar/usg/data/bf_20080604_121337.cor.h5
      -- HDF5 file handle = 16777216
      -- HDF5 group ID    = 16777216
      \endverbatim
      <li>Get the values of the internal parameters handling/describing the access
      to the dataset on disk:
      \code
      // Get the group ID
      dataset.getId();
      // Retrieve the dataset type ("HDF5", "CASA_MS", etc.)
      dataset.getType();
      // Retrieve the name of the data set
      dataset.getName();
      // Get the HDF5 file handle identifier
      dataset.getFileHandle();
      \endcode
    </ol>
  */

  class dalDataset : public dalObjectBase {
    
    //! Dataset filter
    dalFilter itsFilter;
    //! HDF5 file handle
    hid_t h5fh_p;
    
#ifdef DAL_WITH_CASA
    casa::MeasurementSet itsMS;   // CASA measurement set
    casa::Vector<casa::String> ms_tables; // vector of CASA MS tables
#endif
    
  public:

    // === Construction =========================================================
    
    //! Default constructor
    dalDataset();

    //! Argumented constructor
    dalDataset (std::string const &filename,
		dalFileType const &filetype=dalFileType(),
		IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    
    //! Argumented constructor
    dalDataset (std::string const &filename,
		dalFileType::Type const &filetype,
		IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    
    //! Argumented constructor
    dalDataset (std::string const &filename,
		std::string filetype,
		IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~dalDataset();

    // === Parameter access =====================================================

    //! Retrieve the identifier for the group.
    inline hid_t getId () {
      return h5fh_p;
    }

    // === Public methods =======================================================
    
    //! Open the dataset
    bool open (std::string const &filename,
	       IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    //! Close the dataset
    bool close();
    //! Get the attributes of the dataset
    bool getAttributes();
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);
    
    // ---------------------------------------------------------- getAttribute
    
    /*!
      \brief Get the value of an attribute.
      
      Get the value of an attribute.  This is different from printAttribute
      because the value of the attribute is returned into a structure
      instead of simply printing.
      
      \param attrname The name of the attribute you want to retrieve.
      \param value Attribute value to return.
      \return bool -- DAL::FAIL or DAL::SUCCESS
    */
    template <class T>
      bool getAttribute (std::string attrname, T &value )
      {
	switch (itsFiletype.type()) {
	  case dalFileType::HDF5:
	  return HDF5Attribute::read (h5fh_p, attrname, value );
	  break;
	default:
	  return false;
	  break;
	};
      }
    
    /*!
      \brief Write attribute value
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      bool setAttribute (std::string const &name,
			 T const &data)
      {
	switch (itsFiletype.type()) {
	  case dalFileType::HDF5:
	  return HDF5Attribute::write (h5fh_p,
				       name,
				       data);
	  break;
	default:
	  return false;
	  break;
	};
      }
    
    /*!
      \brief Write attribute value
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \param size    -- nof. element in the data array.
      \return status -- Status of the operation
    */
    template <class T>
      bool setAttribute (std::string const &name,
			 T const *data,
			 unsigned int const &size=1)
      {
	switch (itsFiletype.type()) {
	case dalFileType::HDF5:
	  return HDF5Attribute::write (h5fh_p,
				       name,
				       data,
				       size);
	  break;
	default:
	  return false;
	  break;
	};
      }
    
    //! Define a string attribute.
    bool setAttribute_string (std::string attrname,
			      std::vector<std::string> data );
    //! Create a new array in the root group.
    dalArray * createArray (std::string arrayname,
			    dalData * data_object);
    //! Create a new extendible integer array in the root group.
    dalArray * createIntArray (std::string arrayname,
			       std::vector<int> dims,
			       int data[],
			       std::vector<int>cdims );
    //! Create a new extendible floating point array in the root group.
    dalArray * createFloatArray (std::string arrayname,
				 std::vector<int> dims,
				 float data[],
				 std::vector<int>cdims);
    dalArray * createComplexFloatArray (std::string arrayname,
					std::vector<int> dims,
					std::complex<float> data[],
					std::vector<int>cdims );
    //! Create a new table in the root group
    dalTable * createTable (std::string tablename );
    //! Create a new table in a specified group
    dalTable * createTable (std::string tablename, std::string groupname );
    dalGroup * createGroup (const char * groupname );
    //! Set table filter.
    void setFilter (std::string const &columns);
    //! Set table filter.
    void setFilter (std::string const &columns,
		    std::string const &conditions);
    //! Open a table (that's not in a group) by name.
    dalTable * openTable (std::string const &tablename);
    //! Open a table within a group
    dalTable * openTable (std::string const &tablename,
			  std::string const &groupname );
    //! Opens an array at the higest level of a dataset
    dalArray * openArray (std::string const &array);
    //! Open an array in a group.
    dalArray * openArray (std::string const &array,
			  std::string const &group);
    //! Get a list of groups in the dataset.
    std::vector<std::string> getGroupNames();
    dalGroup * openGroup (std::string groupname );
    //! List the tables in a dataset.
    std::vector<std::string> listTables();
    /*!
      \brief Retrieve the dataset type ("HDF5", "CASA_MS", etc.).
      \return type -- A string describing the file format ("HDF5", "CASA_MS", etc.)
    */
    inline std::string getType() {
      return itsFiletype.name();
    }
    /*!
      \brief Get the HDF5 file handle identifier.
      \return h5fh -- File handle identifier to communicate with the HDF5
              file through the HDF5 library.
    */
    inline hid_t getFileHandle () const {
      return h5fh_p;
    }
    //! Read TBB data
    void read_tbb (std::string id,
		   int const &start,
		   int const &length,
		   short data_out[]);
    
    // === Private methods ======================================================
    
  private:
    
    //! Initialize the object's internal parameters
    void init ();
    //! Initialize the object's internal parameters
    void init (std::string const &filename,
	       dalFileType const &filetype,
	       IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    //! Unconditional deletion of internal parameters
    bool destroy ();

    //! Try to open HDF5 file
    bool openHDF5 (std::string const &filename,
		   IO_Mode const &flags=IO_Mode(IO_Mode::Open));
    
#ifdef DAL_WITH_CFITSIO
    //! Try to open FITS file
    bool openFITS (std::string const &filename,
		   IO_Mode const &flags=IO_Mode(IO_Mode::Open));
#endif
    
#ifdef DAL_WITH_CASA
    //! Try to open casa MeasurementSet file
    bool openMS (std::string const &filename,
		 IO_Mode const &flags=IO_Mode(IO_Mode::Open));
#endif
    
    // ==========================================================================
    //
    //  The following functions are boost wrappers to allow some previously
    //  defined functions to be easily called from a python prompt.
    //
    // ==========================================================================
    
#ifdef PYTHON
    
  public:
    
    // create[]Array wrappers
    dalArray * cia_boost1 (std::string arrayname,
			   boost::python::list dims,
			   boost::python::list data);
    dalArray * cia_boost2 (std::string arrayname,
			   boost::python::list dims,
			   boost::python::list data,
			   boost::python::list cdims);
    dalArray * cia_boost_numarray1 (std::string arrayname,
				    boost::python::list dims,
				    boost::python::numeric::array data);
    dalArray * cia_boost_numarray2 (std::string arrayname,
				    boost::python::list dims,
				    boost::python::numeric::array data,
				    boost::python::list cdims);
    dalArray * cfa_boost (std::string arrayname,
			  boost::python::list dims,
			  boost::python::list data,
			  boost::python::list cdims);
    dalArray * cfa_boost_numarray (std::string arrayname,
				   boost::python::list dims,
				   boost::python::numeric::array data,
				   boost::python::list cdims);
    boost::python::numeric::array ria_boost (std::string arrayname);
    boost::python::numeric::array rfa_boost (std::string arrayname);
    
    boost::python::list listTables_boost();
    
    bool setAttribute_char_vector (std::string attrname, boost::python::list data);
    bool setAttribute_short_vector (std::string attrname, boost::python::list data);
    bool setAttribute_int_vector (std::string attrname, boost::python::list data);
    bool setAttribute_uint_vector (std::string attrname, boost::python::list data);
    bool setAttribute_long_vector (std::string attrname, boost::python::list data);
    bool setAttribute_float_vector (std::string attrname, boost::python::list data);
    bool setAttribute_double_vector (std::string attrname, boost::python::list data);
    bool setAttribute_string_vector (std::string attrname, boost::python::list data);
    
    boost::python::numeric::array getAttribute_float_boost (std::string attrname);
    boost::python::numeric::array getAttribute_double_boost (std::string attrname);
    boost::python::numeric::array getAttribute_int_boost (std::string attrname);
    boost::python::numeric::array getAttribute_uint_boost (std::string attrname);
    boost::python::numeric::array getAttribute_short_boost (std::string attrname);
    boost::python::numeric::array getAttribute_long_boost (std::string attrname);
    boost::python::list getAttribute_string_boost (std::string attrname);

#endif  /* end PYTHON */
    
  };  /* end class dalDataset */
  
}  /* end namespace DAL */

#endif
