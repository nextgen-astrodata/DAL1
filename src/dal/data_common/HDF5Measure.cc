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

#include <data_common/HDF5Measure.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  HDF5Measure::HDF5Measure ()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  /*!
    \param name  -- Name of the quantity used a base for the attributes.
  */
  HDF5Measure::HDF5Measure (std::string const &name)
    : HDF5Quantity (name)
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  /*!
    \param name        -- Name of the quantity used a base for the attributes.
    \param valueSuffix -- Suffix appended to the attribute storing the value(s).
    \param unitsSuffix -- Suffix appended to the attribute storing the unit(s).
    \param frameSuffix -- 
    \param separator   -- Separator inserted betwen the base and the suffix of
           the name.
  */
  HDF5Measure::HDF5Measure (std::string const &name,
			    std::string const &valueSuffix,
			    std::string const &unitsSuffix,
			    std::string const &frameSuffix,
			    std::string const &separator)
    : HDF5Quantity (name, valueSuffix, unitsSuffix, separator)
  {
    setFrameSuffix(frameSuffix);
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  /*!
    \param name  -- Name of the quantity used a base for the attributes.
    \param value -- Numerical value.
    \param unit  -- Physical unit associated with the value.
    \param frame -- 
  */
  HDF5Measure::HDF5Measure (std::string const &name,
			    double const &value,
			    std::string const &unit,
			    std::string const &frame)
    : HDF5Quantity (name, value, unit)
  {
    init(frame);
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  /*!
    \param name   -- Name of the quantity used a base for the attributes.
    \param values -- Numerical values.
    \param unit   -- Physical unit associated with the value.
    \param frame  -- 
  */
  HDF5Measure::HDF5Measure (std::string const &name,
			    std::vector<double> const &values,
			    std::string const &unit,
			    std::string const &frame)
    : HDF5Quantity (name, values, unit)
  {
    init(frame);
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  /*!
    \param name   -- Name of the quantity used a base for the attributes.
    \param values -- 
    \param units  -- 
    \param frame  -- 
  */
  HDF5Measure::HDF5Measure (std::string const &name,
			    std::vector<double> const &values,
			    std::vector<std::string> const &units,
			    std::string const &frame)
    : HDF5Quantity (name, values, units)
  {
    init(frame);
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  /*!
    \param quantity -- 
    \param frame    -- 
  */
  HDF5Measure::HDF5Measure (HDF5Quantity const &quantity,
			    std::string const &frame)
    : HDF5Quantity(quantity)
  {
    init(frame);
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Measure
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Measure::HDF5Measure (HDF5Measure const &other)
    : HDF5Quantity(other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  void HDF5Measure::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another HDF5Measure object from which to make a copy.
  */
  HDF5Measure& HDF5Measure::operator= (HDF5Measure const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Measure::copy (HDF5Measure const &other)
  {
    itsFrame       = other.itsFrame;
    itsFrameSuffix = other.itsFrameSuffix;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   setMeasure
  
  bool HDF5Measure::setMeasure (double const &value,
				std::string const &unit,
				std::string const &frame)
  {
    setQuantity (value,unit);
    setFrame (frame);
    return true;
  }
  
  //_____________________________________________________________________________
  //                                                                   setMeasure
  
  bool HDF5Measure::setMeasure (std::vector<double> const &values,
				std::string const &unit,
				std::string const &frame)
  {
    setQuantity (values,unit);
    setFrame (frame);
    return true;
  }
  
  //_____________________________________________________________________________
  //                                                                   setMeasure
  
  bool HDF5Measure::setMeasure (std::vector<double> const &values,
				std::vector<std::string> const &units,
				std::string const &frame)
  {
    setQuantity (values,units);
    setFrame (frame);
    return true;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Measure::summary (std::ostream &os)
  {
    os << "[HDF5Measure] Summary of internal parameters." << std::endl;
    os << "-- Numerical value      = " << itsValue       << std::endl;
    os << "-- Physical units       = " << itsUnits       << std::endl;
    os << "-- Name base            = " << itsName        << std::endl;
    os << "-- Value suffix         = " << itsValueSuffix << std::endl;
    os << "-- Unit suffix          = " << itsUnitSuffix  << std::endl;
    os << "-- Frame suffix         = " << itsFrameSuffix << std::endl;
    os << "-- Separation token     = " << itsSeparator   << std::endl;
    os << "-- Attribute name value = " << nameValue()    << std::endl;
    os << "-- Attribute name units = " << nameUnits()    << std::endl;
    os << "-- Attribute name frame = " << nameFrame()    << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  /*!
    \param frame  -- Name of the reference frame.
    \param suffix -- Suffix appended to the attribute storing the frame.
  */
  void HDF5Measure::init (std::string const &frame,
			  std::string const &suffix)
  {
    itsFrame       = frame;
    itsFrameSuffix = suffix;
  }

  //_____________________________________________________________________________
  //                                                                    nameFrame
  
  std::string HDF5Measure::nameFrame ()
  {
    std::string name = itsName;

    if (!itsFrameSuffix.empty()) {
      name += itsSeparator;
      name += itsFrameSuffix;
    }

    return name;
  }

  //_____________________________________________________________________________
  //                                                                        write
  
  /*!
    \param location -- Identifier of the HDF5 object, to which the attributes 
           storing the measure are being written.
    \return status  -- Status of the operation; returns \e false in case an 
            error was encountered.
  */
  bool HDF5Measure::write (hid_t const &location)
  {
    bool status = true;

    if ( H5Iis_valid(location) ) {

      // Get the names of the attributes ...
      std::string attributeFrame = nameFrame();
      // ... and write them
      status *= HDF5Quantity::write  (location);
      status *= HDF5Attribute::write (location, attributeFrame, itsFrameSuffix);

    } else {
      std::cerr << "[HDF5Measure::write] Invalid location ID!" << std::endl;
      return false;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                         read
  
  /*!
    \todo Implement method 
    \param location -- Identifier of the HDF5 object, from which the attributes 
           storing the measure are being read.
    \return status  -- Status of the operation; returns \e false in case an 
            error was encountered.
  */
  bool HDF5Measure::read (hid_t const &location)
  {
    bool status = true;

    if ( H5Iis_valid(location) ) {

      // Get the names of the attributes ...
      std::string attributeFrame = nameFrame();
      // ... and read them
      
    } else {
      std::cerr << "[HDF5Measure::write] Invalid location ID!" << std::endl;
      return false;
    }

    return status;
  }
  
  
} // Namespace DAL -- end
