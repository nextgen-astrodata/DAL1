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

#include <data_common/HDF5Quantity.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                 HDF5Quantity
  
  HDF5Quantity::HDF5Quantity ()
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                 HDF5Quantity
  
  /*!
    \param name  -- Name of the quantity used a base for the attributes.
  */
  HDF5Quantity::HDF5Quantity (std::string const &name)
  {
    init (name);
  }
  
  //_____________________________________________________________________________
  //                                                                 HDF5Quantity
  
  /*!
    \param name        -- Name of the quantity used a base for the attributes.
    \param valueSuffix -- Suffix appended to the attribute storing the value(s).
    \param unitsSuffix -- Suffix appended to the attribute storing the unit(s).
    \param separator   -- Separator inserted betwen the base and the suffix of
           the name.
  */
  HDF5Quantity::HDF5Quantity (std::string const &name,
			      std::string const &valueSuffix,
			      std::string const &unitsSuffix,
			      std::string const &separator)
  {
    init (name);
    setValueSuffix (valueSuffix);
    setUnitsSuffix (unitsSuffix);
    setSeparator (separator);
  }

  //_____________________________________________________________________________
  //                                                                 HDF5Quantity
  
  /*!
    \param name  -- Name of the quantity used a base for the attributes.
    \param value -- Numerical value.
    \param unit  -- Physical unit associated with the value.
  */
  HDF5Quantity::HDF5Quantity (std::string const &name,
			      double const &value,
			      std::string const &unit)
  {
    init (name);
    setQuantity (value, unit);
  }
  
  //_____________________________________________________________________________
  //                                                                 HDF5Quantity
  
  /*!
    \param name  -- Name of the quantity used a base for the attributes.
    \param value -- Numerical value.
    \param unit  -- Physical unit associated with the value.
  */
  HDF5Quantity::HDF5Quantity (std::string const &name,
			      std::vector<double> const &value,
			      std::string const &unit)
  {
    init (name);
    setQuantity (value, unit);
  }
  
  //_____________________________________________________________________________
  //                                                                 HDF5Quantity
  
  /*!
    \param name  -- Name of the quantity used a base for the attributes.
    \param value -- Numerical value.
    \param units -- Physical units associated with the value.
  */
  HDF5Quantity::HDF5Quantity (std::string const &name,
			      std::vector<double> const &value,
			      std::vector<std::string> const &units)
  {
    init (name);
    setQuantity (value, units);
  }
  
  //_____________________________________________________________________________
  //                                                                 HDF5Quantity
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Quantity::HDF5Quantity (HDF5Quantity const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  void HDF5Quantity::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another HDF5Quantity object from which to make a copy.
  */
  HDF5Quantity& HDF5Quantity::operator= (HDF5Quantity const &other)
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
    \param other -- Another HDF5Property object from which to make a copy.
  */
  void HDF5Quantity::copy (HDF5Quantity const &other)
  {
    itsValue.resize(other.itsValue.size());
    itsValue = other.itsValue;

    itsUnits.resize(other.itsUnits.size());
    itsUnits = other.itsUnits;

    itsName        = other.itsName;
    itsValueSuffix = other.itsValueSuffix;
    itsUnitSuffix  = other.itsUnitSuffix;
    itsSeparator   = other.itsSeparator;
  }

  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                     setValue
  
  /*!
    \param value   -- Numerical value.
    \return status -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool HDF5Quantity::setValue (std::vector<double> const &value)
  {
    bool status = true;

    if ( value.size() == itsValue.size() ) {
      itsValue = value;
    } else if ( value.size() == itsUnits.size() ) {
      itsValue.resize(itsUnits.size());
      itsValue = value;
    } else if ( itsValue.size()*itsUnits.size() == 0 ) {
      itsValue.resize(value.size());
      itsValue = value;
    } else {
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                     setUnits
  
  /*!
    \param value   -- Physical unit(s) associated with the value(s).
    \return status -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool HDF5Quantity::setUnits (std::vector<std::string> const &units)
  {
    bool status = true;

    if ( units.size() == itsUnits.size() ) {
      itsUnits = units;
    } else if ( units.size() == itsValue.size() ) {
      itsUnits.resize(itsValue.size());
      itsUnits = units;
    } else if ( itsUnits.size()*itsValue.size() == 0 ) {
      itsUnits.resize(units.size());
      itsUnits = units;
    } else {
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                  setQuantity
  
  /*!
    \param value   -- Numerical value.
    \param unit    -- Physical unit associated with the value.
    \return status -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool HDF5Quantity::setQuantity (double const &value,
				  std::string const &unit)
  {
    return setQuantity (std::vector<double>(1,value),
			std::vector<std::string>(1,unit));
  }
  
  //_____________________________________________________________________________
  //                                                                  setQuantity
  
  /*!
    \param value   -- Numerical value.
    \param unit    -- Physical unit associated with the value.
    \return status -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool HDF5Quantity::setQuantity (std::vector<double> const &value,
				  std::string const &unit)
  {
    std::vector<std::string> units (value.size(),unit);
    return setQuantity (value,units);
  }
  
  //_____________________________________________________________________________
  //                                                                  setQuantity
  
  /*!
    \param value   -- Numerical value.
    \param units   -- Physical units associated with the value.
    \return status -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool HDF5Quantity::setQuantity (std::vector<double> const &value,
				  std::vector<std::string> const &units)
  {
    if (value.size() == units.size()) {
      // Value(s)
      itsValue.resize(value.size());
      itsValue = value;
      // Unit(s)
      itsUnits.resize(units.size());
      itsUnits = units;
      // return
      return true;
    } else {
      return false;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Quantity::summary (std::ostream &os)
  {
    os << "[HDF5Quantity] Summary of internal parameters." << std::endl;
    os << "-- Numerical value      = " << itsValue         << std::endl;
    os << "-- Physical units       = " << itsUnits         << std::endl;
    os << "-- Name base            = " << itsName          << std::endl;
    os << "-- Value suffix         = " << itsValueSuffix   << std::endl;
    os << "-- Unit suffix          = " << itsUnitSuffix    << std::endl;
    os << "-- Separation token     = " << itsSeparator     << std::endl;
    os << "-- Attribute name value = " << nameValue()      << std::endl;
    os << "-- Attribute name units = " << nameUnits()      << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  /*!
    \param name  -- Name of the quantity used a base for the attributes.
  */
  void HDF5Quantity::init (std::string const &name)
  {
    itsValue.clear();
    itsUnits.clear();
    
    itsName        = name;
    itsValueSuffix = "VALUE";
    itsUnitSuffix  = "UNITS";
    itsSeparator   = "_";
  }

  //_____________________________________________________________________________
  //                                                                    nameValue
  
  std::string HDF5Quantity::nameValue ()
  {
    std::string name = itsName;

    if (!itsValueSuffix.empty()) {
      name += itsSeparator;
      name += itsValueSuffix;
    }

    return name;
  }
  
  //_____________________________________________________________________________
  //                                                                    nameUnits

  std::string HDF5Quantity::nameUnits ()
  {
    std::string name = itsName;

    if (!itsUnitSuffix.empty()) {
      name += itsSeparator;
      name += itsUnitSuffix;
    }

    return name;
  }
  
  //_____________________________________________________________________________
  //                                                                        write
  
  /*!
    \param location -- Identifier of the HDF5 object, to which the attributes 
           storing the quantity are being written.
    \return status  -- Status of the operation; returns \e false in case an 
            error was encountered.
  */
  bool HDF5Quantity::write (hid_t const &location)
  {
    bool status = true;

    if ( H5Iis_valid(location) ) {

      // Get the names of the attributes ...
      std::string attributeValue = nameValue();
      std::string attributeUnits = nameUnits();
      // ... and write them
      status *= HDF5Attribute::write (location, attributeValue, itsValue);
      status *= HDF5Attribute::write (location, attributeUnits, itsUnits);

    } else {
      std::cerr << "[HDF5Quantity::write] Invalid location ID!" << std::endl;
      return false;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                         read
  
  /*!
    \param location -- Identifier of the HDF5 object, from which the attributes 
           storing the quantity are being read.
    \return status  -- Status of the operation; returns \e false in case an 
            error was encountered.
  */
  bool HDF5Quantity::read (hid_t const &location)
  {
    bool status = true;

    if ( H5Iis_valid(location) ) {

      // Get the names of the attributes ...
      std::string attributeValue = nameValue();
      std::string attributeUnits = nameUnits();
      // ... and write them
      status *= HDF5Attribute::read (location, attributeValue, itsValue);
      status *= HDF5Attribute::read (location, attributeUnits, itsUnits);

    } else {
      std::cerr << "[HDF5Quantity::write] Invalid location ID!" << std::endl;
      return false;
    }

    return status;
  }
  
} // Namespace DAL -- end
