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

#ifndef TABULARCOORDINATE_H
#define TABULARCOORDINATE_H

// DAL header files
#include <coordinates/CoordinateBase.h>

#ifdef DAL_WITH_CASA
#include <coordinates/Coordinates/TabularCoordinate.h>
#endif

namespace DAL {  // Namespace DAL -- begin
  
  /*!
    \class TabularCoordinate
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Brief description for class TabularCoordinate
    
    \author Lars B&auml;hren

    \date 2009/06/24

    \test tTabularCoordinate.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>[start filling in your text here]
    </ul>

    <h3>Synopsis</h3>

    <h3>Example(s)</h3>

    <ul>
      <li>One of the cases where a tabular axis can become of use is for a
      non-contiguous time-axis (e.g. due to the fact that parts of an observation
      were dropped during transfer).
      \verbatim
      COORDINATE_TYPE    = Tabular
      NOF_AXES           = 1
      AXIS_NAMES         = "Time"
      AXIS_UNITS         = "s"
      AXIS_LENGTH        = 8
      AXIS_VALUES_PIXEL  = [0,1,2,3,4,5,6,7]
      AXIS_VALUES_WORLD  = [0,1,2,5,10,20,50,100]
      \endverbatim
    </ul>
    
  */
  template <class T>
  class TabularCoordinate : public CoordinateBase<T> {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TabularCoordinate ()
      {
	init();
      }
    //! Argumented constructor
    TabularCoordinate (std::string const &axisNames,
		       std::string const &axisUnits)
      {
	init (axisNames,
	      axisUnits);
      }
    //! Argumented constructor
    TabularCoordinate (std::string const &axisNames,
		       std::string const &axisUnits,
		       std::vector<double> const &pixelValues,
		       std::vector<T> const &worldValues)
      {
	// Initialize basic parameters
	init (axisNames,
	      axisUnits);
	// Set axis values
	setAxisValues (pixelValues,
		       worldValues);
      }
    //! Copy constructor
    TabularCoordinate (TabularCoordinate<T> const &other)
      : CoordinateBase<T> (other)
      {
	copy (other);
      }
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~TabularCoordinate () {
      destroy();
    }
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TabularCoordinate object from which to make a copy.
    */
    TabularCoordinate<T>& operator= (TabularCoordinate<T> const &other) {
      if (this != &other) {
	destroy ();
	copy (other);
      }
      return *this;
    }
    
    // === Parameter access =====================================================

    //! Set the values along the pixel and world axis
    bool setAxisValues (std::vector<double> const &pixelValues,
			std::vector<T> const &worldValues)
    {
      bool status (true);
      
      if (pixelValues.empty() && worldValues.empty()) {
	this->itsPixelValues.clear();
	this->itsWorldValues.clear();
      } else if (pixelValues.size() == worldValues.size()) {
	// adjust array sizes
	this->itsPixelValues.resize(pixelValues.size());
	this->itsWorldValues.resize(worldValues.size());
	// copy values
	this->itsPixelValues = pixelValues;
	this->itsWorldValues = worldValues;
      }
      else {
	std::cerr << "[TabularCoordinate::setAxisValues]" 
		  << " Mismatch in length of input vectors!"
		  << std::endl;
	status = false;
      }

      return status;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TabularCoordinate.
    */
    inline std::string className () const {
      return "TabularCoordinate";
    }
    
    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () {
      summary (std::cout);
    }
    
    /*!
      \brief Provide a summary of the internal status
      
      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os)
    {
      os << "[TabularCoordinate] Summary of internal parameters." << std::endl;
      os << "-- Coordinate type       = " << this->type()
	 << " / " <<  this->name() << std::endl;
      os << "-- Storage type          = " << this->storageType_p.type()
	 << " / " <<  this->storageType_p.name() << std::endl;
      os << "-- nof. axes             = " << this->nofAxes_p     << std::endl;
      os << "-- World axis names      = " << this->axisNames_p   << std::endl;
      os << "-- World axis units      = " << this->axisUnits_p   << std::endl;
      os << "-- Pixel values          = " << this->itsPixelValues << std::endl;
      os << "-- World values          = " << this->itsWorldValues << std::endl;
      os << "-- Reference value       = " << this->refValue_p    << std::endl;
      os << "-- Reference pixel       = " << this->refPixel_p    << std::endl;
      os << "-- Transformation matrix = " << this->pc_p          << std::endl;
    }
    
    // === Public methods =======================================================

#ifdef DAL_WITH_HDF5    
    //___________________________________________________________________________
    //                                                                 write_hdf5
    //! Write the coordinate object to a HDF5 file
    void write_hdf5 (hid_t const &groupID)
    {
      HDF5Attribute::write (groupID, "COORDINATE_TYPE", this->name() );
      HDF5Attribute::write (groupID, "NOF_AXES",        this->nofAxes_p );
      HDF5Attribute::write (groupID, "AXIS_NAMES",      this->axisNames_p );
      HDF5Attribute::write (groupID, "AXIS_UNITS",      this->axisUnits_p );
      HDF5Attribute::write (groupID, "PIXEL_VALUES",    this->itsPixelValues );
      HDF5Attribute::write (groupID, "WORLD_VALUES",    this->itsWorldValues );
    }
    //___________________________________________________________________________
    //                                                                 write_hdf5
    //! Write the coordinate object to a HDF5 file
    void write_hdf5 (hid_t const &locationID,
		     std::string const &name)
    {
      hid_t groupID (0);
      // create HDF5 group
      if (H5Lexists (locationID, name.c_str(), H5P_DEFAULT)) {
	groupID = H5Gopen (locationID,
			   name.c_str(),
			   H5P_DEFAULT);
	
      } else {
	groupID = H5Gcreate( locationID,
			     name.c_str(),
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT );
      }
      // write coordinate attributes
      write_hdf5 (groupID);
      // close the group after write
      H5Gclose (groupID);
    }
    //___________________________________________________________________________
    //                                                                  read_hdf5
    //! Read the coordinate object from a HDF5 file
    void read_hdf5 (hid_t const &groupID)
    {
      std::string coordinateTypeName;
      
      HDF5Attribute::read (groupID, "COORDINATE_TYPE",  coordinateTypeName );
      HDF5Attribute::read (groupID, "NOF_AXES",         this->nofAxes_p );
      HDF5Attribute::read (groupID, "AXIS_NAMES",       this->axisNames_p );
      HDF5Attribute::read (groupID, "AXIS_UNITS",       this->axisUnits_p );
      HDF5Attribute::read (groupID, "PIXEL_VALUES",     this->itsPixelValues );
      HDF5Attribute::read (groupID, "WORLD_VALUES",     this->itsWorldValues );
    }
    //___________________________________________________________________________
    //                                                                  read_hdf5
    //! Read the coordinate object from a HDF5 file
    void read_hdf5 (hid_t const &locationID,
		    std::string const &name)
    {
      hid_t groupID (0);
      
      groupID = H5Gopen (locationID,
			 name.c_str(),
			 H5P_DEFAULT);
      
      if (groupID) {
	read_hdf5 (groupID);
      }
      else {
	std::cerr << "[TabularCoordinate::read_hdf5] Error opening group "
		  << name
		  << std::endl;
      }
      
      H5Gclose (groupID);
    }
#endif
    
#ifdef DAL_WITH_CASA
    //! Create coordinate from casa::Coordinate object
    void importCoordinate (casa::TabularCoordinate const &coord);
    //! Create casa::Coordinate object from coordinate parameters
    void exportCoordinate (casa::TabularCoordinate &coord);
#endif

  private:
    
    //! Initialize internal parameters
    void init (std::string const &axisNames="UNDEFINED",
	       std::string const &axisUnits="UNDEFINED")
    {
      std::cout << "[TabularCoordinate<T>::init(string,string)]" << std::endl;
      // arrays to be passed on
      std::vector<std::string> names (1,axisNames);
      std::vector<std::string> units (1,axisUnits);
      std::vector<double> pixel;
      std::vector<T> world;
      // forward function call
      init (names, units, pixel, world);
    }

    //! Initialize internal parameters
    void init (std::vector<std::string> const &axisNames,
	       std::vector<std::string> const &axisUnits,
	       std::vector<double> const &pixelValues,
	       std::vector<T> const &worldValues)
    {
      std::cout << "[TabularCoordinate<T>::init(string,string,vector,vector)]" << std::endl;
      /* Initialize base class */
      CoordinateBase<T>::init (Coordinate::TABULAR,
				    1,
				    Coordinate::TABULAR);
      
      CoordinateBase<T>::setAxisNames (axisNames);
      CoordinateBase<T>::setAxisUnits (axisUnits);
      setAxisValues (pixelValues, worldValues);
    }
    
    //! Unconditional copying
    void copy (TabularCoordinate const &other);
    
    //! Unconditional deletion
    void destroy(void) {}
    
  }; // Class TabularCoordinate -- end
  
} // Namespace DAL -- end

#endif /* TABULARCOORDINATE_H */

