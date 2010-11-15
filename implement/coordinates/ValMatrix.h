/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#ifndef VALMATRIX_H
#define VALMATRIX_H

#include <dalCommon.h>

#ifdef HAVE_CASA
#include <casa/Arrays/Vector.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class ValMatrix
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Encapsulation of matrix-type data
    
    \author Lars B&auml;hren

    \date 2010/11/14

    \test tValMatrix.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>

    <ol>
      <li>Create a new matrix of singular shape:
      \code
      DAL::ValMatrix mat;
      \endcode

      <li>Create a new matrix of square shape with sidelength \e N:
      \code
      size_t N = 10;
      DAL::ValMatrix mat (N);
      \endcode
      All matrix elements are initialized with 0.

      <li>Create a new matrix of shape \f$ ( N_{\rm rows}, N_{\rm cols} ) \f$:
      \code
      size_t nofRows = 4;
      size_t nofCols = 3;
      DAL::ValMatrix mat (nofRows,nofCols);
      \endcode
      All matrix elements are initialized with 0.
    </ol>
    
  */  
  template <class T> class ValMatrix {

    //! nof. rows in the matrix, \f$ N_{\rm rows} \f$
    size_t nofRows_p;
    //! nof. columns in the matrix, \f$ N_{\rm cols} \f$
    size_t nofColumns_p;
    //! Matrix data
    std::valarray<T> data_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    ValMatrix ()
      {
	nofRows_p    = 0;
	nofColumns_p = 0;
	data_p       = std::valarray<T>();
      };
    /*!
      \brief Argumented constructor
      \param nofRows    -- Number of rows in the matrix.
      \param nofColumns -- Number of columns in the matrix; if \f$ N_{\rm cols}=0 \f$
             the matrix is considered to be of square shape with 
	     \f$ N_{\rm cols} = N_{\rm rows} \f$
    */
    ValMatrix (size_t const &nofRows,
	       size_t const &nofColumns=0)
      {
	if (nofColumns==0) {
	  nofRows_p    = nofRows;
	  nofColumns_p = nofRows;
	} else {
	  nofRows_p    = nofRows;
	  nofColumns_p = nofColumns;
	}
	data_p.resize(nofRows*nofColumns);
      }
    
    /*!
      \brief Argumented constructor
      \param nofRows    -- Number of rows in the matrix.
      \param nofColumns -- Number of columns in the matrix.
      \param value      -- Initial value to be assigned to all matrix elements.
    */
    ValMatrix (size_t const &nofRows,
	       size_t const &nofColumns,
	       T const &value)
      {
	nofRows_p    = nofRows;
	nofColumns_p = nofColumns;
	data_p.resize(nofRows*nofColumns,value);
      }
    
    //! Copy constructor
    ValMatrix (ValMatrix const &other)
      {
	copy (other);
      }
    
    // === Destruction ==========================================================

    //! Destructor
    ~ValMatrix ()
      {
	destroy();
      }
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    ValMatrix& operator= (ValMatrix const &other)
      {
	if (this != &other) {
	  destroy ();
	  copy (other);
	}
	return *this;
      }
    
    // === Parameter access =====================================================
    
    //! Get the number of elements in the matrix
    inline size_t size () const {
      return nofRows_p*nofColumns_p;
    }
    //! Get the number of elements in the matrix
    inline size_t nofElements () const {
      return nofRows_p*nofColumns_p;
    }
    //! Get the number of rows in the matrix
    inline size_t nofRows () const {
      return nofRows_p;
    }
    //! Get the number columns in the matrix
    inline size_t nofColumns () const {
      return nofColumns_p;
    }
    /*!
      \brief Get the shape of the matrix
      \return shape -- Shape of the matrix, \f$ [ N_{\rm rows} , N_{\rm cols} ] \f$
    */
    inline std::vector<size_t> shape () const {
      std::vector<size_t> nelem (2);
      nelem[0] = nofRows_p;
      nelem[1] = nofColumns_p;
      return nelem;
    }
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, ValMatrix.
    */
    inline std::string className () const {
      return "ValMatrix";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the object's internal parameters and status
      \param os -- Output stream to which the summary is going to be written.
    */
    void summary (std::ostream &os)
    {
      os << "[ValMatrix] Summary of internal parameters." << std::endl;
      os << "-- Shape           = " << shape()       << std::endl;
      os << "-- nof. elements   = " << nofElements() << std::endl;
      os << "-- sum of elements = " << sum()         << std::endl;
      os << "-- min of elements = " << min()         << std::endl;
      os << "-- max of elements = " << max()         << std::endl;
    }
    
    // === Methods ==============================================================
    
    //! Get the matrix elements of row \e n.
    inline std::valarray<T> row (size_t const &n) 
      {
	return data_p[std::slice(n*nofColumns_p,nofColumns_p,1)];
      }
    //! Set the matrix elements of row \e n to the value \e val.
    inline void setRow (size_t const &n,
			T const &val) 
      {
	data_p[std::slice(n*nofColumns_p,nofColumns_p,1)] = val;
      }
    //! Sum of all matrix elements, \f$ S = \sum_{i,j} a_{ij} \f$
    inline T sum () const {
      return data_p.sum();
    }
    //! Maximum of all matrix elements
    inline T min () const {
      if (data_p.size()==0) {
	return 0;
      } else {
	return data_p.min();
      }
    }
    //! Minimum of all matrix elements
    inline T max () const {
      if (data_p.size()==0) {
	return 0;
      } else {
	return data_p.max();
      }
    }
    //! Assign running numbr to matrix elements
    inline void indgen () {
      size_t nelem = data_p.size();
      if (nelem!=0) {
	for (size_t n(0); n<nelem; ++n) {
	  data_p[n] = n;
	}
      }
    }
    /*!
      \brief Is the matrix of square shape?
      \return isSquare -- Returns \e true in case the matrix is of square shape,
              i.e. \f$ N_{\rm rows} = N_{\rm cols} \f$.
    */
    inline bool isSquare () const
    {
      if (data_p.empty()) {
	return false;
      } else {
	if (nofRows_p==nofColumns_p) {
	  return true;
	} else {
	  return false;
	}
      }
    }
    /*!
      \brief Set diagonal elements of the matrix.
      \param val -- Value to assign to the diagonal of the matrix.
      \return status -- Status of the operation; returns \e false in case the
              matrix is not of square shape.
    */
    inline bool setDiagonal (T const &val)
    {
      if (isSquare()) {
	for (size_t n(0); n<nofRows_p; +n) {
	  data_p[n*nofColumns_p+n] = val;
	}
      } else {
	return false;
      }
    }
    /*!
      \brief Get diagonal elements of the matrix.
      \retval diag -- Vector with the diagonal elements of the matrix.
      \return status -- Status of the operation; returns \e false in case the
              matrix is not of square shape.
    */
    inline bool diagonal (std::vector<T> &diag)
    {
      if (isSquare()) {
	diag.resize(nofRows_p);
	for (size_t n(0); n<nofRows_p; +n) {
	  diag[n] = data_p[n*nofColumns_p+n];
	}
	return true;
      } else {
	return false;
      }
    }
#ifdef HAVE_CASA
    /*!
      \brief Get diagonal elements of the matrix.
      \retval diag -- Vector with the diagonal elements of the matrix.
      \return status -- Status of the operation; returns \e false in case the
              matrix is not of square shape.
    */
    inline bool diagonal (casa::Vector<T> &diag)
    {
      if (isSquare()) {
	diag.resize(nofRows_p);
	for (size_t n(0); n<nofRows_p; +n) {
	  diag(n) = data_p[n*nofColumns_p+n];
	}
	return true;
      } else {
	return false;
      }
    }
#endif
    
  private:
    
    /*!
      \brief Unconditional copying
      \param other -- Another \e ValMatrix object to make a copy of.
    */
    void copy (ValMatrix const &other)
    {
      nofRows_p    = other.nofRows_p;
      nofColumns_p = other.nofColumns_p;
      data_p       = other.data_p;
    }
    
    //! Unconditional deletion 
    void destroy(void) {}
    
  }; // Class ValMatrix -- end
  
} // Namespace DAL -- end

#endif /* VALMATRIX_H */

