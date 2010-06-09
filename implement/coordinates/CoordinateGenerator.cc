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
    init ();
  }
  
  /*!
    \param other -- Another CoordinateGenerator object from which to create this new
           one.
  */
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
  
  /*!
    \param other -- Another CoordinateGenerator object from which to make a copy.
  */
  void CoordinateGenerator::copy (CoordinateGenerator const &other)
  {
    haveCASA_p = other.haveCASA_p;

    projections_p.clear();
    projections_p = other.projections_p;
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
    os << "-- Have CASA/casacore   = " << haveCASA()        << std::endl;
    os << "-- Map projection names = " << projectionNames() << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void CoordinateGenerator::init ()
  {
#ifdef HAVE_CASA
    haveCASA_p = true;
#else
    haveCASA_p = false;
#endif

    setProjections ();
  }

  //_____________________________________________________________________________
  //                                                               setProjections
  
  void CoordinateGenerator::setProjections ()
  {
    projections_p.clear();

    projections_p["AIR"] = "Airy";
    projections_p["AIT"] = "Hammer-Aitoff";
    projections_p["CAR"] = "Cartesian";
    projections_p["MER"] = "Mercator";
    projections_p["MOL"] = "Molweide";
    projections_p["SIN"] = "Orthographics/synthesis";
    projections_p["STG"] = "Stereographic";
    projections_p["TAN"] = "Gnomonic";
  }

  //_____________________________________________________________________________
  //                                                              projectionNames
  
  std::vector<std::string> CoordinateGenerator::projectionNames ()
  {
    std::map<std::string,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=projections_p.begin(); it!=projections_p.end(); ++it) {
      names.push_back (it->first);
    }
    
    return names;
  }

  //_____________________________________________________________________________
  //                                                       projectionDescriptions
  
  std::vector<std::string> CoordinateGenerator::projectionDescriptions ()
  {
    std::map<std::string,std::string>::iterator it;
    std::vector<std::string> info;

    for (it=projections_p.begin(); it!=projections_p.end(); ++it) {
      info.push_back (it->second);
    }
    
    return info;
  }


  // ============================================================================
  //
  //   Static methods
  //
  // ============================================================================

#ifdef HAVE_CASA
  
  //_____________________________________________________________________________
  //                                                               makeCoordinate

  /*!
    \retval coord     -- casa::DirectionCoordinate object created from the input
            parameters.
    \param refcode    -- Reference code for the celestial coordinate frame
    \param projection -- Reference code for the sphercial map projection
    \param refValue   -- Reference value, CRVAL
    \param increment  -- Coordinate increment, CDELT
    \param refPixel   -- Reference pixel, CRPIX
    \return status    -- Status of the operation; return \e false in case an
            error was encountered, \e true otherwise.
  */
  bool CoordinateGenerator::makeCoordinate (casa::DirectionCoordinate &coord,
					    casa::MDirection::Types const &refcode,
					    casa::Projection::Type const &projection,
					    Vector<Quantum<double> > const &refValue,
					    Vector<Quantum<double> > const &increment,
					    Vector<double> const &refPixel)
  {
    bool status (true);
    unsigned int rank (2);
    
    /* Check the dimensions of the vectors with the input parameters */
    
    if (refValue.nelements() == rank
	&& increment.nelements() == rank
	&& refPixel.nelements() == rank) {
      /* Set linear transformation matrix */
      casa::Matrix<casa::Double> xform (rank,rank);
      xform            = 0.0;
      xform.diagonal() = 1.0;
      /* Create DirectionCoordinate object */
      coord = casa::DirectionCoordinate (refcode,
					 casa::Projection(projection),
					 refValue(0),
					 refValue(1),
					 increment(0),
					 increment(1),
					 xform,
					 refPixel(0),
					 refPixel(1));
    } else {
      status = false;
      coord  = casa::DirectionCoordinate ();
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                               makeCoordinate
  
  /*!
    \retval coord  -- casa::LinearCoordinate object created from the input
            parameters.
    \param nofAxes -- The number of axes for which to create the coordinate
    \param names   -- World axis names of the coordinate.
    \param units   -- World axis units of the coordinate.
    \return status   -- Status of the operation; return \e false in case an
            error was encountered, \e true otherwise.
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
    \retval coord    -- casa::LinearCoordinate object created from the input
            parameters.
    \param nofAxes   -- The number of axes for which to create the coordinate
    \param names     -- World axis names of the coordinate.
    \param units     -- World axis units of the coordinate.
    \param refValue  -- Reference value, CRVAL.
    \param increment -- Coordinate increment, CDELT.
    \param refPixel  -- Reference pixel, CRPIX.
    \return status   -- Status of the operation; return \e false in case an
            error was encountered, \e true otherwise.
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
    \retval coord    -- casa::LinearCoordinate object created from the input
            parameters.
    \param names     -- World axis names of the coordinate.
    \param units     -- World axis units of the coordinate.
    \param refValue  -- Reference value, CRVAL.
    \param increment -- Coordinate increment, CDELT.
    \param refPixel  -- Reference pixel, CRPIX.
    \return status   -- Status of the operation; return \e false in case an
            error was encountered, \e true otherwise.
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

  //_____________________________________________________________________________
  //                                                               makeCoordinate
  
  /*!
    \retval coord    -- casa::SpectralCoordinate object created from the input
            parameters.
    \param refValue  -- Reference value, CRVAL, [s]
    \param increment -- Coordinate increment, CDELT, [s]
    \param refPixel  -- Reference pixel, CRPIX
    \return status   -- Status of the operation; return \e false in case an
            error was encountered, \e true otherwise.
  */
  bool CoordinateGenerator::makeCoordinate (casa::SpectralCoordinate &coord,
					    double const &refValue,
					    double const &increment,
					    double const &refPixel)
  {
    bool status (true);
    casa::Vector<casa::String> names (1,"Frequency");
    
    coord = casa::SpectralCoordinate (casa::MFrequency::TOPO,
				      refValue,
				      increment,
				      refPixel);
    coord.setWorldAxisNames(names);
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                               makeCoordinate
  
  /*!
    \retval coord    -- casa::StokesCoordinate object.
    \return status   -- Status of the operation; return \e false in case an
            error was encountered, \e true otherwise.
  */
  bool CoordinateGenerator::makeCoordinate (casa::StokesCoordinate &coord)
  {
    bool status (true);
    casa::Vector<casa::Int> iquv (4);
    
    iquv(0) = casa::Stokes::I;
    iquv(1) = casa::Stokes::Q;
    iquv(2) = casa::Stokes::U;
    iquv(3) = casa::Stokes::V;
    
    coord = casa::StokesCoordinate (iquv);
    
    return status;
  }
  
#endif
  
} // Namespace DAL -- end
