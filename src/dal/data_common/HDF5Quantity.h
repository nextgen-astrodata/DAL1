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

#include <core/dalCommon.h>
#include <core/HDF5Object.h>

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
    
    The attribute name of the components is constructed as follows:

    \verbatim
    <name><separator><valueSuffix>
    <name><separator><unitSuffix>
    \endverbatim

    Depending on the values of the individual parameters, the resulting 
    attribute names will be as follows:
    
    <center>
    <table>
      <tr>
        <td class="indexkey">Name</td>
        <td class="indexkey">ValueSuffix</td>
        <td class="indexkey">UnitSuffix</td>
        <td class="indexkey">Separator</td>
        <td class="indexkey">Attribute value</td>
        <td class="indexkey">Attribute units</td>
      </tr>
      <tr>
        <td>TIME</td>
        <td>VALUE</td>
        <td>UNITS</td>
        <td>_</td>
        <td>TIME_VALUE</td>
        <td>TIME_UNITS</td>
      </tr>
      <tr>
        <td>TIME</td>
        <td></td>
        <td>UNITS</td>
        <td>_</td>
        <td>TIME</td>
        <td>TIME_UNITS</td>
      </tr>
      <tr>
        <td>Time</td>
        <td>Value</td>
        <td>Units</td>
        <td>.</td>
        <td>Time.Value</td>
        <td>Time.Units</td>
      </tr>
      <tr>
        <td>Time</td>
        <td></td>
        <td>Units</td>
        <td>.</td>
        <td>Time</td>
        <td>Time.Units</td>
      </tr>
      <tr>
        <td>Time</td>
        <td>Value</td>
        <td>Units</td>
        <td></td>
        <td>TimeValue</td>
        <td>TimeUnits</td>
      </tr>
    </table>
    </center>
    
    <h3>Example(s)</h3>

  */  
  class HDF5Quantity {

  protected:

    //! Name of the quantity used a base for the attributes
    std::string itsName;
    //! Suffix appended to the attribute storing the value(s)
    std::string itsValueSuffix;
    //! Suffix appended to the attribute storing the unit(s)
    std::string itsUnitSuffix;
    //! Separator inserted betwen the base and the suffix of the name
    std::string itsSeparator;
    
    //! Numerical value(s)
    std::vector<double> itsValue;
    //! Physical unit(s) associated with the value(s)
    std::vector<std::string> itsUnits;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Quantity ();

    //! Argumented constructor
    HDF5Quantity (std::string const &name);

    //! Argumented constructor
    HDF5Quantity (std::string const &name,
		  std::string const &valueSuffix,
		  std::string const &unitsSuffix,
		  std::string const &separator);

    //! Argumented constructor
    HDF5Quantity (std::string const &name,
		  double const &value,
		  std::string const &unit);
    
    //! Argumented constructor
    HDF5Quantity (std::string const &name,
		  std::vector<double> const &value,
		  std::string const &unit);
    
    //! Argumented constructor
    HDF5Quantity (std::string const &name,
		  std::vector<double> const &value,
		  std::vector<std::string> const &units);

    //! Copy constructor
    HDF5Quantity (HDF5Quantity const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    virtual ~HDF5Quantity () {
      destroy();
    }
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Quantity& operator= (HDF5Quantity const &other); 
    
    // === Parameter access =====================================================

    //! Get the name of the quantity used a base for the attributes. 
    inline std::string name () const {
      return itsName;
    }
    
    //! Set the name of the quantity used a base for the attributes. 
    inline bool setName (std::string const &name) {
      itsName = name;
      return true;
    }
    
    //! Get the suffix appended to the attribute storing the value(s) 
    inline std::string valueSuffix () const {
      return itsValueSuffix;
    }
    
    //! Set the suffix appended to the attribute storing the value(s) 
    inline bool setValueSuffix (std::string const &suffix) {
      itsValueSuffix = suffix;
      return true;
    }
    
    //! Get the suffix appended to the attribute storing the unit(s) 
    inline std::string unitsSuffix () const {
      return itsUnitSuffix;
    }
    
    //! Set the suffix appended to the attribute storing the unit(s) 
    inline bool setUnitsSuffix (std::string const &suffix) {
      itsUnitSuffix = suffix;
      return true;
    }
    
    //! Get the separator inserted betwen the base and the suffix of the name. 
    inline std::string separator () const {
      return itsSeparator;
    }
    
    //! Set the separator inserted betwen the base and the suffix of the name. 
    inline bool setSeparator (std::string const &separator) {
      itsSeparator = separator;
      return true;
    }
    
    //! Get the numerical value(s)
    inline std::vector<double> value () const {
      return itsValue;
    }
    
    //! Set the numerical value(s)
    bool setValue (std::vector<double> const &value);
    
    //! Get the physical unit(s) associated with the value(s)
    inline std::vector<std::string> units () const {
      return itsUnits;
    }
    
    //! Set the physical unit(s) associated with the value(s)
    bool setUnits (std::vector<std::string> const &units);
    
    //! Set quantity composed of single value and unit
    bool setQuantity (double const &value,
		      std::string const &unit);
    
    //! Set quantity composed of multiple value with common single unit
    bool setQuantity (std::vector<double> const &value,
		      std::string const &unit);
    
    //! Set quantity
    bool setQuantity (std::vector<double> const &value,
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

    //! Get the name for the attribute storing the value of the quantity
    std::string nameValue ();
    
    //! Get the name for the attribute storing the units of the quantity
    std::string nameUnits ();
    
    //! Write quantity to HDF5 file/group identified by \c location
    virtual bool write (hid_t const &location);
    
    //! Read quantity from HDF5 file/group identified by \c location
    virtual bool read (hid_t const &location);
    
  private:

    //! Initialize internal parameters
    void init (std::string const &name="QUANTITY");
    
    //! Unconditional copying
    void copy (HDF5Quantity const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Quantity -- end
  
} // Namespace DAL -- end

#endif /* HDF5QUANTITY_H */
  
