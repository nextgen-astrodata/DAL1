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

#ifndef HDF5QUANTITY_H
#define HDF5QUANTITY_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Quantity
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Brief description for class HDF5Quantity
    
    \author Lars B&auml;hren

    \date 2011/06/01

    \test tHDF5Quantity.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Quantity {

    //! Numerical value(s)
    std::vector<double> itsValues;
    //! Physical unit(s) associated with the value(s)
    std::vector<std::string> itsUnits;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Quantity ();

    //! Argumented constructor
    HDF5Quantity (double const &value,
		  std::string const &unit);

    //! Argumented constructor
    HDF5Quantity (std::vector<double> const &values,
		  std::string const &unit);
    
    //! Argumented constructor
    HDF5Quantity (std::vector<double> const &values,
		  std::vector<std::string> const &units);

    //! Copy constructor
    HDF5Quantity (HDF5Quantity const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Quantity ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Quantity& operator= (HDF5Quantity const &other); 
    
    // === Parameter access =====================================================

    //! Get the numerical value(s)
    inline std::vector<double> values () const {
      return itsValues;
    }
    
    //! Get the physical unit(s) associated with the value(s)
    inline std::vector<std::string> units () const {
      return itsUnits;
    }

    //! Set quantity composed of single value and unit
    bool setQuantity (double const &value,
		      std::string const &unit);

    //! Set quantity composed of multiple values with common single unit
    bool setQuantity (std::vector<double> const &values,
		      std::string const &unit);

    //! Set quantity
    bool setQuantity (std::vector<double> const &values,
		      std::vector<std::string> const &units);

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5Quantity.
    */
    inline std::string className () const {
      return "HDF5Quantity";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    
    
    // === Static methods =======================================================
    
    
    
  private:
    
    //! Unconditional copying
    void copy (HDF5Quantity const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Quantity -- end
  
} // Namespace DAL -- end

#endif /* HDF5QUANTITY_H */
  
