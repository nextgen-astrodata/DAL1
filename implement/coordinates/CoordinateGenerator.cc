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

#include <CoordinateGenerator.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  CoordinateGenerator::CoordinateGenerator ()
  {
#ifdef HAVE_CASA
    haveCASA_p = true;
#else
    haveCASA_p = false;
#endif
  }
  
  CoordinateGenerator::CoordinateGenerator (CoordinateGenerator const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  void CoordinateGenerator::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  CoordinateGenerator& CoordinateGenerator::operator= (CoordinateGenerator const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void CoordinateGenerator::copy (CoordinateGenerator const &other)
  {
    haveCASA_p = other.haveCASA_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void CoordinateGenerator::summary (std::ostream &os)
  {
    os << "[CoordinateGenerator] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  // ============================================================================
  //
  //   Static methods
  //
  // ============================================================================

#ifdef HAVE_CASA
  
  //_____________________________________________________________________________
  //                                                               makeCoordinate
  
  /*!
    \retval coord  -- 
    \param nofAxes -- The number of axes for which to create the coordinate
    \param names   -- World axis names of the coordinate.
    \param units   -- World axis units of the coordinate.
  */
  bool CoordinateGenerator::makeCoordinate (casa::LinearCoordinate &coord,
					    unsigned int const &nofAxes,
					    casa::Vector<casa::String> const &names,
					    casa::Vector<casa::String> const &units)
  {
    bool status (true);
    casa::Vector<double> crpix(nofAxes,0.0);
    casa::Vector<double> crval(nofAxes,0.0);
    casa::Vector<double> cdelt(nofAxes,1.0);
    casa::Matrix<double> pc(nofAxes,nofAxes,0.0);
    
    pc.diagonal() = 1.0;
    
    if (nofAxes == names.nelements() && nofAxes == units.nelements()) {
      coord = casa::LinearCoordinate (names,
				      units,
				      crval,
				      cdelt,
				      pc,
				      crpix);
    } else {
      status = false;
      /* Issue error message */
      std::cerr << "[CoordinateGenerator::makeCoordinate] " 
		<< "Mismatch in rank of parameter vectors!" << std::endl;
      std::cerr << "-- nofAxes = " << nofAxes << std::endl;
      std::cerr << "-- names   = " << names   << std::endl;
      std::cerr << "-- units   = " << units   << std::endl;
      /* Create sensible default coordinate object */
      coord = casa::LinearCoordinate (casa::Vector<casa::String>(nofAxes,names(0)),
				      casa::Vector<casa::String>(nofAxes,units(0)),
				      crval,
				      cdelt,
				      pc,
				      crpix);
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                               makeCoordinate
  
  /*!
    \retval coord    -- 
    \param nofAxes   -- The number of axes for which to create the coordinate
    \param names     -- World axis names of the coordinate.
    \param units     -- World axis units of the coordinate.
    \param refValue  -- Reference value, CRVAL.
    \param increment -- Coordinate increment, CDELT.
    \param refPixel  -- Reference pixel, CRPIX.
  */
  bool CoordinateGenerator::makeCoordinate (casa::LinearCoordinate &coord,
					    unsigned int const &nofAxes,
					    casa::String const &name,
					    casa::String const &unit,
					    double const &refValue,
					    double const &increment,
					    double const &refPixel)
  {
    casa::Vector<casa::String> names (nofAxes,name);
    casa::Vector<casa::String> units (nofAxes,unit);
    casa::Vector<double> crval (nofAxes,refValue);
    casa::Vector<double> cdelt (nofAxes,increment);
    casa::Vector<double> crpix (nofAxes,refPixel);
    
    return makeCoordinate (coord,
				 names,
				 units,
				 crval,
				 cdelt,
				 crpix);
  }

  //_____________________________________________________________________________
  //                                                               makeCoordinate
  
  /*!
    \retval coord    -- 
    \param names     -- World axis names of the coordinate.
    \param units     -- World axis units of the coordinate.
    \param refValue  -- Reference value, CRVAL.
    \param increment -- Coordinate increment, CDELT.
    \param refPixel  -- Reference pixel, CRPIX.
  */
  bool CoordinateGenerator::makeCoordinate (casa::LinearCoordinate &coord,
					    casa::Vector<casa::String> const &names,
					    casa::Vector<casa::String> const &units,
					    casa::Vector<double> const &refValue,
					    casa::Vector<double> const &increment,
					    casa::Vector<double> const &refPixel)
  {
    bool status (true);
    uint rank = names.nelements();

    /* Check the size of the input arrays */

    if (rank != units.nelements() ||
	rank != refValue.nelements() ||
	rank != increment.nelements() ||
	rank != refPixel.nelements()) {
      std::cerr << "[CoordinateGenerator::makeCoordinate] " 
		<< "Mismatch in rank of parameter vectors!" << std::endl;
      status = false;
    } else {
    casa::Matrix<double> pc (rank,rank,0.0);
    
    for (uint n(0); n<rank; ++n) {
      pc(n,n) = 1.0;
    }
    /* Create the coordinate object */
    coord = casa::LinearCoordinate (names,
				    units,
				    refValue,
				    increment,
				    pc,
				    refPixel);
    }
    
    return status;
  }
  
#endif
  
} // Namespace DAL -- end
