/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 2286 2009-02-03 10:50:48Z baehren                    $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#ifndef SAS_SETTINGS_H
#define SAS_SETTINGS_H

// Standard library header files
#include <iostream>
#include <set>
#include <map>
#include <string>

// DAL header files
#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class SAS_Settings
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Brief description for class SAS_Settings
    
    \author Lars B&auml;hren

    \date 2009/12/09

    \test tSAS_Settings.cc
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class SAS_Settings {

    //! Values for the AntennaSet field in SAS
    std::set<std::string> antennaSet_p;
    //! Values for the FilterSelection field in SAS
    std::set<std::string> filterSelection_p;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    SAS_Settings ();
    
    /*!
      \brief Copy constructor
      
      \param other -- Another SAS_Settings object from which to create this new
             one.
    */
    SAS_Settings (SAS_Settings const &other);
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~SAS_Settings ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another SAS_Settings object from which to make a copy.
    */
    SAS_Settings& operator= (SAS_Settings const &other); 
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, SAS_Settings.
    */
    inline std::string className () const {
      return "SAS_Settings";
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
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods

    //! Get the values for the AntennaSet field in SAS
    inline std::set<std::string> antennaSet () {
      return antennaSet_p;
    }

    //! Is <tt>name</tt> a valid value for the AntennaSet field in SAS?
    inline bool haveAntennaSet (std::string const &name) const {
      return static_cast<bool>(antennaSet_p.count(name));
    }

    //! Get the values for the FilterSelection field in SAS
    inline std::set<std::string> filterSelection () {
      return filterSelection_p;
    }
    
    //! Is <tt>name</tt> a valid value for the FilterSelection field in SAS?
    inline bool haveFilterSelection (std::string const &name) const {
      return static_cast<bool>(filterSelection_p.count(name));
    }

  private:

    //! Set the values for the various SAS settings
    void setValues ();
    
    //! Unconditional copying
    void copy (SAS_Settings const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class SAS_Settings -- end
  
} // Namespace DAL -- end

#endif /* SAS_SETTINGS_H */
  
