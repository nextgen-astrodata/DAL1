/***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

#ifndef DALARRAY_H
#define DALARRAY_H

#include <complex>

#include <core/dalCommon.h>
#include <core/dalObjectBase.h>
#include <core/HDF5Attribute.h>

namespace DAL {
  
  /*!
    \class dalArray

    \ingroup DAL
    \ingroup core

    \brief Represents an n-dimensional array.

    \author Joseph Masters
    \author Lars B&auml;hren
    
    The dalArray object holds an n-dimensional array of a single datatype.
  */
  class dalArray : public dalObjectBase {
    
    //! Number of dimensions
    int itsRank;
    //! Array datatype identifier
    std::string datatype;
    //! HDF5 return status
    herr_t status;
    
  protected:
    
    //! HDF5 object ID for array
    hid_t itsDatasetID;
    //! HDF5 object ID for file
    hid_t itsFileID;
    
  public:

    // === Construction =========================================================

    //! Default constructor
    dalArray();

    // === Parameter access =====================================================

    //! Retrieve the dimensions of an array
    std::vector<int> dims();
    //! Open an existing array.
    int open (void * file,
	      std::string arrayname);
    //! Close an existing array.
    bool close();
    //! Get the array ID.
    inline hid_t getId() {
      return itsDatasetID;
    }
    //! Get the rank of the array, i.e. the number of dimensions
    inline int getRank () const {
      return itsRank;
    }
    
    //! Get attributes attached to the array
    bool getAttributes();

    // === Methods ==============================================================

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os);
    
    /*!
      \brief Get the value of an attribute.
      
      Get the value of an attribute.  This is different from printAttribute
      because the value of the attribute is returned into a structure
      instead of simply printing.
      
      \param attrname The name of the attribute you want to retrieve.
      \param value Attribute value to return.
      \return bool -- DAL::FAIL or DAL::SUCCESS
    */
    template <class T>
      bool getAttribute (std::string attrname,
			 T &value )
      {
        return HDF5Attribute::write (itsDatasetID, attrname, value );
      }
    
    /*!
      \brief Set attribute attached to array
      \param name --  The name of the attribute you want to create.
      \param data --  The value of the attribute you want to create.
      \param size -- Optional parameter specifying the array size of the
             attribute. Default is scalar.
      \return bool -- Status of the operation; returns \e false in case an
              error was encountered.
   */
    template <class T>
      bool setAttribute (std::string name,
			 T * data,
			 int size=1 )
      {
	return HDF5Attribute::write (itsDatasetID,
					    name,
					    data,
					    size);
      }
    
    /*!
      \brief Set attribute attached to array
      \param name --  The name of the attribute you want to create.
      \param data --  The value of the attribute you want to create.
      \return bool -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool setAttribute (std::string name,
			 std::vector<T> const &data)
      {
	return HDF5Attribute::write (itsDatasetID,
					    name,
					    &data[0],
					    data.size());
      }

    /*!
      \brief Set attribute attached to array
      \param name --  The name of the attribute you want to create.
      \param data --  The value of the attribute you want to create.
      \return bool -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool setAttribute (std::string name,
			 T const &data)
      {
	return HDF5Attribute::write (itsDatasetID,
					    name,
					    &data,
					    1);
      }

    //! Increase the dimensions of the array.
    bool extend (std::vector<int> const &dims);
    //! Write \e data of type \e short.
    bool write (int offset, short data[], int arraysize);
    //! Write \e data of type \e int.
    bool write (int offset, int data[], int arraysize);
    //! Write \e data of type \e complex<float>.
    bool write (int offset, std::complex<float> data[], int arraysize );
    //! Write \e data of type \e complex<Int16>.
    bool write (int offset, std::complex<Int16> data[], int arraysize );
    
    // === Python wrapper functions =============================================

#ifdef PYTHON
    
    void extend_boost (boost::python::list pydims );
    
    dalArray * csa_boost_list (std::string arrayname,
			       boost::python::list dims,
			       boost::python::list data);
    dalArray * cia_boost_list (std::string arrayname,
			       boost::python::list dims,
			       boost::python::list data);
    dalArray * cfa_boost_list (std::string arrayname,
			       boost::python::list dims,
			       boost::python::list data);
    
    boost::python::numeric::array ria_boost( std::string arrayname );
    
    bool setAttribute_char   (std::string name, char data);
    bool setAttribute_short  (std::string const &name, short const &data);
    bool setAttribute_int    (std::string const &name, int const &data);
    bool setAttribute_uint   (std::string const &name, uint const &data);
    bool setAttribute_long   (std::string const &name, long const &data);
    bool setAttribute_float  (std::string const &name, float const &data);
    bool setAttribute_double (std::string const &name, double const &data);
    bool setAttribute_string (std::string name, std::string data );
    bool setAttribute_char_vector( std::string attrname, boost::python::list data );
    bool setAttribute_short_vector( std::string attrname, boost::python::list data );
    //! Set attribute of type \e vector<int>
    bool setAttribute_int_vector( std::string attrname, boost::python::list data );
    //! Set attribute of type \e vector<uint>
    bool setAttribute_uint_vector( std::string attrname, boost::python::list data );
    //! Set attribute of type \e vector<long>
    bool setAttribute_long_vector( std::string attrname, boost::python::list data );
    //! Set attribute of type \e vector<float>
    bool setAttribute_float_vector( std::string attrname, boost::python::list data );
    //! Set attribute of type \e vector<double>
    bool setAttribute_double_vector( std::string attrname, boost::python::list data );
    //! Set attribute of type \e vector<string>
    bool setAttribute_string_vector( std::string attrname, boost::python::list data );
    
#endif
  };
  
  /*!
    \class dalFloatArray
    \ingroup DAL
    \ingroup  core
    \brief Represents an n-dimensional array of type \e float
  */
  class dalFloatArray: public dalArray
  {
    
  public:

    //! Argumented constructor
    dalFloatArray (hid_t obj_id,
		   std::string arrayname,
		   std::vector<int> dims,
		   float data[],
		   std::vector<int>chnkdims);
  };
  
  /*!
    \class dalComplexArray_float32
    \ingroup DAL
    \ingroup  core
    \brief Represents an n-dimensional array of type \e complex<float32>
  */
  class dalComplexArray_float32: public dalArray {
    
  public:
    
    //! Argumented constructor
    dalComplexArray_float32( hid_t objfile,
			     std::string arrayname,
			     std::vector<int> dims,
			     std::complex<float> data[],
			     std::vector<int>chnkdims);
  };
  
} // end namespace DAL

#endif // end #ifndef DALARRAY_H
