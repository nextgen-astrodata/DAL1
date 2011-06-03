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

#ifndef HDF5MEASURE_H
#define HDF5MEASURE_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

#include <data_common/HDF5Quantity.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Measure
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Brief description for class HDF5Measure
    
    \author Lars B&auml;hren

    \date 2011/06/01

    \test tHDF5Measure.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>DAL::HDF5Quantity
    </ul>
    
    <h3>Synopsis</h3>
    
    The attribute name of the components is constructed as follows:

    \verbatim
    <name><separator><valueSuffix>
    <name><separator><unitSuffix>
    <name><separator><frameSuffix>
    \endverbatim

    Depending on the values of the individual parameters, the resulting 
    attribute names will be as follows:
    
    <center>
    <table>
      <tr>
        <td class="indexkey">Name</td>
        <td class="indexkey">ValueSuffix</td>
        <td class="indexkey">UnitSuffix</td>
        <td class="indexkey">frameSuffix</td>
        <td class="indexkey">Separator</td>
        <td class="indexkey">Attribute value</td>
        <td class="indexkey">Attribute units</td>
        <td class="indexkey">Attribute frame</td>
      </tr>
      <tr>
        <td>TIME</td>
        <td>VALUE</td>
        <td>UNITS</td>
        <td>FRAME</td>
        <td>_</td>
        <td>TIME_VALUE</td>
        <td>TIME_UNITS</td>
        <td>TIME_FRAME</td>
      </tr>
      <tr>
        <td>TIME</td>
        <td></td>
        <td>UNITS</td>
        <td>FRAME</td>
        <td>_</td>
        <td>TIME</td>
        <td>TIME_UNITS</td>
        <td>TIME_FRAME</td>
      </tr>
      <tr>
        <td>Time</td>
        <td>Value</td>
        <td>Units</td>
        <td>Frame</td>
        <td>.</td>
        <td>Time.Value</td>
        <td>Time.Units</td>
        <td>Time.Frame</td>
      </tr>
      <tr>
        <td>Time</td>
        <td></td>
        <td>Units</td>
        <td>Frame</td>
        <td>.</td>
        <td>Time</td>
        <td>Time.Units</td>
        <td>Time.Frame</td>
      </tr>
      <tr>
        <td>Time</td>
        <td>Value</td>
        <td>Units</td>
        <td>Frame</td>
        <td></td>
        <td>TimeValue</td>
        <td>TimeUnits</td>
        <td>TimeFrame</td>
      </tr>
    </table>
    </center>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Measure : public HDF5Quantity {

  protected:

    //! Name of the reference frame
    std::string itsFrame;
    //! Suffix appended to the attribute storing the frame
    std::string itsFrameSuffix;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Measure ();

    //! Argumented constructor
    HDF5Measure (std::string const &name);
    
    //! Argumented constructor
    HDF5Measure (std::string const &name,
		 std::string const &valueSuffix,
		 std::string const &unitsSuffix,
		 std::string const &frameSuffix,
		 std::string const &separator);
    
    //! Argumented constructor
    HDF5Measure (std::string const &name,
		 double const &value,
		 std::string const &unit,
		 std::string const &frame);
    
    //! Argumented constructor
    HDF5Measure (std::string const &name,
		 std::vector<double> const &values,
		 std::string const &unit,
		 std::string const &frame);
    
    //! Argumented constructor
    HDF5Measure (std::string const &name,
		 std::vector<double> const &values,
		 std::vector<std::string> const &units,
		 std::string const &frame);
    
    //! Argumented constructor
    HDF5Measure (HDF5Quantity const &quantity,
		 std::string const &frame);
    
    //! Copy constructor
    HDF5Measure (HDF5Measure const &other);
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Measure& operator= (HDF5Measure const &other); 
    
    // === Parameter access =====================================================

    //! Get the name of the reference frame
    inline std::string frame () const {
      return itsFrame;
    }

    //! Set the name of the reference frame
    inline bool setFrame (std::string const &frame) {
      itsFrame = frame;
      return true;
    }
    
    //! Get the suffix appended to the attribute storing the frame
    inline std::string frameSuffix () const {
      return itsFrameSuffix;
    }

    //! Set the suffix appended to the attribute storing the frame
    inline bool setFrameSuffix (std::string const &suffix) {
      itsFrameSuffix = suffix;
      return true;
    }

    //! Set the measure
    bool setMeasure (double const &value,
		     std::string const &unit,
		     std::string const &frame);
    
    //! Set the measure
    bool setMeasure (std::vector<double> const &values,
		     std::string const &unit,
		     std::string const &frame);
    
    //! Set the measure
    bool setMeasure (std::vector<double> const &values,
		     std::vector<std::string> const &units,
		     std::string const &frame);
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5Measure.
    */
    inline std::string className () const {
      return "HDF5Measure";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Get the name for the attribute storing the frame of the measure
    std::string nameFrame ();

    //! Write measure to HDF5 file/group identified by \c location
    bool write (hid_t const &location);
    
    //! Read measure from HDF5 file/group identified by \c location
    bool read (hid_t const &location);
    
    // === Static methods =======================================================
    
    
    
  private:

    //! Initialize internal parameters
    void init (std::string const &frame="",
	       std::string const &suffix="FRAME");

    //! Unconditional copying
    void copy (HDF5Measure const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Measure -- end
  
} // Namespace DAL -- end

#endif /* HDF5MEASURE_H */
  
