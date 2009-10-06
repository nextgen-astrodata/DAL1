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

#ifndef FILENAME_H
#define FILENAME_H

// Standard library header files
#include <iostream>
#include <map>
#include <string>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Filename
    
    \ingroup DAL
    
    \brief Class to filenames matching convention
    
    \author Lars B&auml;hren

    \date 2009/09/28

    \test tFilename.cpp
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICD: Naming conventions (LOFAR-USG-ICD-005)
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class Filename {

    std::map<std::string,std::string> extensionsList_p;
/*     map<char,int>::iterator it; */
    
    //! Unique identifier for the observation
    std::string observationID_p;
    //! Optional descriptors
    std::string optionalDescription_p;
    //! Marker for the contents of the file
    std::string filetype_p;
    //! Extension of the file
    std::string extension_p;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    Filename ();
    
    /*!
      \brief Copy constructor
      
      \param other -- Another Filename object from which to create this new
             one.
    */
    Filename (Filename const &other);
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~Filename ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another Filename object from which to make a copy.
    */
    Filename& operator= (Filename const &other); 
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, Filename.
    */
    inline std::string className () const {
      return "Filename";
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods
    
    
    
  private:

    //! Initialize the internal parameters held by an object of this class
    void init ();
    
    //! Unconditional copying
    void copy (Filename const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Filename -- end
  
} // Namespace DAL -- end

#endif /* FILENAME_H */
  
