/***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#ifndef STOKES_H
#define STOKES_H

// Standard library header files
#include <iostream>
#include <map>
#include <string>
#include <vector>

// DAL header files
#include <core/dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Stokes
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Components of a Stokes coordinate
    
    \author Lars B&auml;hren

    \date 2010/10/22

    \test tStokes.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_002
    </ul>
    
    <h3>Synopsis</h3>

    <b>Important note:</b> All constructors must call <tt>init()</tt> in
    order to set up the internal paramaters!
    
    <h3>Example(s)</h3>
    
  */  
  class Stokes {
    
  public:
    
    //! Recognized values for a Stokes component
    enum Component {
      /*! Standard Stokes total intensity, i.e. total Poynting vector of flux
	density of the wave. */
      I,
      /*! Standard Stokes linear; degree of polarization, i.e. the difference in
	intensities between horizontal and vertical linearly polarized
	components. */
      Q,
      /*! Standard Stokes linear; plane of polarization, i.e. the difference in
	intensities between linearly polarized components oriented at
	\f$ \pm \pi/4 \f$ w.r.t. the components of \f$ Q \f$ */
      U,
      /*! Standard Stokes circular; ellipticity, i.e. the differences in
	intensities between right and left circular polarized components. */
      V,
      //! Right circular
      R,
      //! Left circular
      L,
      //! Right-right circular
      RR,
      //! Left-left circular
      LL,
      //! Right-left circular
      RL,
      //! Left-right circular
      LR,
      //! X linear
      X,
      //! Y linear
      Y,
      //! X parallel linear
      XX,
      //! Y parallel linear
      YY,
      //! XY cross linear
      XY,
      //! YX cross linear
      YX
    };
    
    // === Construction =========================================================
    
    //! Default constructor; created Stokes::I component.
    Stokes ();
    
    //! Argumented constructor
    Stokes (Stokes::Component const &type);
    
    //! Argumented constructor
    Stokes (std::string const &type);
    
    //! Copy constructor
    Stokes (Stokes const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~Stokes ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    Stokes& operator= (Stokes const &other); 
    
    // === Parameter access =====================================================

    //! Get the type of the Stokes component
    inline Stokes::Component type () const {
      return component_p->first;
    }

    //! Set the Stokes component by type
    bool setType (Stokes::Component const &type);

    //! Set the Stokes component by name
    bool setType (std::string const &name);

    //! Get the name of the Stokes component
    inline std::string name () const {
      return component_p->second;
    }

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, Stokes.
    */
    inline std::string className () const {
      return "Stokes";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    //! Is the component linear?
    bool isLinear ();
    //! Is the component linear?
    static bool isLinear (Stokes::Component const &type);
    //! Is the component circular
    bool isCircular ();
    //! Is the component circular
    static bool isCircular (Stokes::Component const &type);
    //! Is the component parallel?
    bool isParallel ();
    //! Is the component parallel?
    static bool isParallel (Stokes::Component const &type);
    //! Is the component cross?
    bool isCross ();
    //! Is the component cross?
    static bool isCross (Stokes::Component const &type);
    
    //! Get map of component types and names
    static std::map<Stokes::Component,std::string> componentsMap ();
    //! Get list of component types
    static std::vector<Stokes::Component> componentsType ();
    //! Get list of component names 
    static std::vector<std::string> componentsName ();

  private:

    // === Private variables ====================================================

    //! List of Stokes components
    std::map<Stokes::Component,std::string> components_p;
    //! Stokes component represented by this object
    std::map<Stokes::Component,std::string>::iterator component_p;

    // === Private methods ======================================================
    
    //! Unconditional copying
    void copy (Stokes const &other);
    
    //! Unconditional deletion 
    void destroy(void);

    //! Initialize the object's internal parameters
    void init (Stokes::Component const &type=Stokes::I);
    
  }; // Class Stokes -- end
  
} // Namespace DAL -- end

#endif /* STOKES_H */
  
