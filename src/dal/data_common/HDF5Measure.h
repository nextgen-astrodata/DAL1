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
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Measure : public HDF5Quantity {

    //! Name of the reference frame
    std::string itsFrame;
    //! Suffix appended to the attribute storing the frame
    std::string itsFrameSuffix;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Measure ();

    //! Argumented constructor
    HDF5Measure (std::string const &name,
		 double const &value,
		 std::string const &unit,
		 std::string const &frame);
    
    //! Argumented constructor
    HDF5Measure (std::vector<double> const &values,
		 std::string const &unit,
		 std::string const &frame);
    
    //! Argumented constructor
    HDF5Measure (std::vector<double> const &values,
		 std::vector<std::string> const &units,
		 std::string const &frame);
    
    //! Argumented constructor
    HDF5Measure (HDF5Quantity const &quantity,
		 std::string const &frame);
    
    //! Copy constructor
    HDF5Measure (HDF5Measure const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~HDF5Measure ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Measure& operator= (HDF5Measure const &other); 
    
    // === Parameter access =====================================================
    
    inline std::string frame () const {
      return itsFrame;
    }

    inline bool setFrame (std::string const &frame) {
      itsFrame = frame;
      return true;
    }
    
    inline std::string frameSuffix () const {
      return itsFrameSuffix;
    }
      
    bool setMeasure (double const &value,
		     std::string const &unit,
		     std::string const &frame);
    
    bool setMeasure (std::vector<double> const &values,
		     std::string const &unit,
		     std::string const &frame);
    
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

    //! Write measure to HDF5 file/group identified by \c location
    bool write (hid_t const &location);
    
    //! Read measure from HDF5 file/group identified by \c location
    bool read (hid_t const &location);
    
    // === Static methods =======================================================
    
    
    
  private:

    //! Unconditional copying
    void copy (HDF5Measure const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Measure -- end
  
} // Namespace DAL -- end

#endif /* HDF5MEASURE_H */
  
