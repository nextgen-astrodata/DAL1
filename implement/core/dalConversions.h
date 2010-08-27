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

#ifndef DALCONVERSIONS_H
#define DALCONVERSIONS_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

#ifdef HAVE_CASA
#include <casa/Arrays/Vector.h>
#endif

/*!
  \file dalConversions.h
  
  \ingroup DAL
  \ingroup core
  
  \brief Collection of functions to perform various types of conversions
  
  \author Lars B&auml;hren
  
  \date 2010/08/27
  
  \test tdalConversions.cc
  
  <h3>Synopsis</h3>
  
  <h3>Example(s)</h3>
  
*/  
namespace DAL { // Namespace DAL -- begin

  // === Conversion of vectors ==================================================
  
#ifdef HAVE_CASA
  
  /*!
    \brief Convert std::vector<T> to casac::Vector<T>

    \retval casaVector -- casa::Vector<T> to which the input vector is converted.
    \param stdVector   -- std::vector<T> input vector to be converted.
  */
  template <class T, class S>
    void convertVector (casa::Vector<T> &casaVector,
			std::vector<S> const &stdVector)
  {
    unsigned int nelem = stdVector.size();
    // adjust the shape of the returned casa::Vector<T>
    casaVector.resize(nelem);
    
    for (unsigned int n(0); n<nelem; ++n) {
      casaVector(n) = (T)stdVector[n];
    }
  }
  
  //! Convert std::vector<T> to casa::Vector<T>
  template <class T, class S>
    void convertVector (std::vector<T> & stdVector,
			const casa::Vector<S> casaVector)
  {
    unsigned int nelem = casaVector.size();
    // adjust the shape of the returned casa::Vector<T>
    stdVector.resize(nelem);
    
    for (unsigned int n(0); n<nelem; ++n) {
      stdVector(n) = (T)casaVector[n];
    }
  }
  
#endif

  // === Conversions to string ==================================================
  
} // Namespace DAL -- end

#endif /* DALCONVERSIONS_H */

