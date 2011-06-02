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

#include "HDF5Measure.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  HDF5Measure::HDF5Measure ()
    : HDF5Quantity()
  {
  }
  
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
    setFrame (frame);
  }
  
  /*!
    \param name   -- Name of the quantity used a base for the attributes.
    \param values -- 
    \param unit   -- 
    \param frame  -- 
  */
  HDF5Measure::HDF5Measure (std::string const &name,
			    std::vector<double> const &values,
			    std::string const &unit,
			    std::string const &frame)
    : HDF5Quantity (name, values, unit)
  {
    setFrame (frame);
  }
  
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
    setMeasure (values, units, frame);
  }
  
  /*!
    \param quantity -- 
    \param frame    -- 
  */
  HDF5Measure::HDF5Measure (HDF5Quantity const &quantity,
			    std::string const &frame)
    : HDF5Quantity(quantity)
  {
    setFrame (frame);
  }
  
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
  
  HDF5Measure::~HDF5Measure ()
  {
    destroy();
  }
  
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
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
