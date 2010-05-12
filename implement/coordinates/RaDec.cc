/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include <RaDec.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                        RaDec
  
  RaDec::RaDec ()
  {
    setAngles (0.0, 0.0);
  }
  
  //_____________________________________________________________________________
  //                                                                        RaDec
  
  RaDec::RaDec (double const &ra,
		double const &dec,
		bool const &angleInDegrees)
  {
    setAngles (ra, dec, angleInDegrees);
  }
  
  //_____________________________________________________________________________
  //                                                                        RaDec
  
  RaDec::RaDec (RaDec const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  RaDec::~RaDec ()
  {
    destroy();
  }
  
  void RaDec::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  RaDec& RaDec::operator= (RaDec const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void RaDec::copy (RaDec const &other)
  {
    radec_p.resize (other.radec_p.size());

    radec_p = other.radec_p;
  }

  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                        setRA
  
  bool RaDec::setRA (double const &ra,
		     bool const &angleInDegrees)
  {
    bool status (true);

    DAL::Angle angle (ra, angleInDegrees);

    radec_p[0] = angle;
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       setDec
  
  bool RaDec::setDec (double const &dec,
		      bool const &angleInDegrees)
  {
    bool status (true);

    DAL::Angle angle (dec, angleInDegrees);

    radec_p[1] = angle;
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                    setAngles
  
  bool RaDec::setAngles (double const &ra,
			 double const &dec,
			 bool const &angleInDegrees)
  {
    bool status (true);

    status *= setRA (ra, angleInDegrees);
    status *= setDec (dec, angleInDegrees);
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                    setAngles
  
  bool RaDec::setAngles (std::vector<DAL::Angle> const &angles)
  {
    bool status (true);

    if (angles.size() == 2) {
      radec_p = angles;
    } else {
      std::cerr << "[RaDec::setAngles]" << std::endl;
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void RaDec::summary (std::ostream &os)
  {
    os << "[RaDec] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
