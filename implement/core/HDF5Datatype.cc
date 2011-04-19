/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <core/HDF5Datatype.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  HDF5Datatype::HDF5Datatype ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Datatype::HDF5Datatype (HDF5Datatype const &other)
    : HDF5Object (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Datatype::~HDF5Datatype ()
  {
    destroy();
  }
  
  void HDF5Datatype::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another HDF5Datatype object from which to make a copy.
  */
  HDF5Datatype& HDF5Datatype::operator= (HDF5Datatype const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Datatype::copy (HDF5Datatype const &other)
  {
    itsLocation = other.itsLocation;
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
  void HDF5Datatype::summary (std::ostream &os)
  {
    os << "[HDF5Datatype] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                 datatypesMap
  
  std::map<hid_t,std::string> HDF5Datatype::datatypesMap ()
  {
    std::map<hid_t,std::string> result;

    /*________________________________________________________________
      Datatype class identifiers
    */

    result[H5T_INTEGER]   = "H5T_INTEGER";
    result[H5T_FLOAT]     = "H5T_FLOAT";
    result[H5T_STRING]    = "H5T_STRING";
    result[H5T_BITFIELD]  = "H5T_BITFIELD";
    result[H5T_OPAQUE]    = "H5T_OPAQUE";
    result[H5T_COMPOUND]  = "H5T_COMPOUND";
    result[H5T_REFERENCE] = "H5T_REFERENCE";
    result[H5T_ENUM]      = "H5T_ENUM";
    result[H5T_VLEN]      = "H5T_VLEN";
    result[H5T_ARRAY]     = "H5T_ARRAY";

    /*________________________________________________________________
      IEEE floating point datatypes
    */

    result[H5T_IEEE_F32BE] = "H5T_IEEE_F32BE";
    result[H5T_IEEE_F32LE] = "H5T_IEEE_F32LE";
    result[H5T_IEEE_F64BE] = "H5T_IEEE_F64BE";
    result[H5T_IEEE_F64LE] = "H5T_IEEE_F64LE";

    /*________________________________________________________________
      Standard datatypes
    */

    result[H5T_STD_I8BE]    = "H5T_STD_I8BE";
    result[H5T_STD_I8LE]    = "H5T_STD_I8LE";
    result[H5T_STD_I16BE]   = "H5T_STD_I16BE";
    result[H5T_STD_I16LE]   = "H5T_STD_I16LE";
    result[H5T_STD_I32BE]   = "H5T_STD_I32BE";
    result[H5T_STD_I32LE]   = "H5T_STD_I32LE";
    result[H5T_STD_I64BE]   = "H5T_STD_I64BE";
    result[H5T_STD_I64LE]   = "H5T_STD_I64LE";

    result[H5T_STD_U8BE]    = "H5T_STD_U8BE";
    result[H5T_STD_U8LE]    = "H5T_STD_U8LE";
    result[H5T_STD_U16BE]   = "H5T_STD_U16BE";
    result[H5T_STD_U16LE]   = "H5T_STD_U16LE";
    result[H5T_STD_U32BE]   = "H5T_STD_U32BE";
    result[H5T_STD_U32LE]   = "H5T_STD_U32LE";
    result[H5T_STD_U64BE]   = "H5T_STD_U64BE";
    result[H5T_STD_U64LE]   = "H5T_STD_U64LE";

    result[H5T_STD_B8BE]    = "H5T_STD_B8BE";
    result[H5T_STD_B8LE]    = "H5T_STD_B8LE";
    result[H5T_STD_B16BE]   = "H5T_STD_B16BE";
    result[H5T_STD_B16LE]   = "H5T_STD_B16LE";
    result[H5T_STD_B32BE]   = "H5T_STD_B32BE";
    result[H5T_STD_B32LE]   = "H5T_STD_B32LE";
    result[H5T_STD_B64BE]   = "H5T_STD_B64BE";
    result[H5T_STD_B64LE]   = "H5T_STD_B64LE";
    /* Object reference or dataset region reference */
    result[H5T_STD_REF_OBJ]     = "H5T_STD_REF_OBJ";
    result[H5T_STD_REF_DSETREG] = "H5T_STD_REF_DSETREG";

    /*________________________________________________________________
      UNIX-specific datatypes:
       - 32-bit and 64-bit
       - Big-endian and little-endian 
    */

    result[H5T_UNIX_D32BE]   = "H5T_UNIX_D32BE";
    result[H5T_UNIX_D32LE]   = "H5T_UNIX_D32LE";
    result[H5T_UNIX_D64BE]   = "H5T_UNIX_D64BE";
    result[H5T_UNIX_D64LE]   = "H5T_UNIX_D64LE";

    /*________________________________________________________________
      C-specific datatype
    */

    result[H5T_C_S1]   = "H5T_C_S1";

    /*________________________________________________________________
      Predefined native datatypes
    */

    result[H5T_NATIVE_CHAR]    = "H5T_NATIVE_CHAR";
    result[H5T_NATIVE_SCHAR]   = "H5T_NATIVE_SCHAR";
    result[H5T_NATIVE_UCHAR]   = "H5T_NATIVE_UCHAR";

    result[H5T_NATIVE_SHORT]   = "H5T_NATIVE_SHORT";
    result[H5T_NATIVE_USHORT]  = "H5T_NATIVE_USHORT";
    result[H5T_NATIVE_INT]     = "H5T_NATIVE_INT";
    result[H5T_NATIVE_UINT]    = "H5T_NATIVE_UINT";
    result[H5T_NATIVE_LONG]    = "H5T_NATIVE_LONG";
    result[H5T_NATIVE_ULONG]   = "H5T_NATIVE_ULONG";
    result[H5T_NATIVE_LLONG]   = "H5T_NATIVE_LLONG";
    result[H5T_NATIVE_ULLONG]  = "H5T_NATIVE_ULLONG";

    result[H5T_NATIVE_FLOAT]   = "H5T_NATIVE_FLOAT";
    result[H5T_NATIVE_DOUBLE]  = "H5T_NATIVE_DOUBLE";
    result[H5T_NATIVE_LDOUBLE] = "H5T_NATIVE_LDOUBLE";

    result[H5T_NATIVE_B8]      = "H5T_NATIVE_B8";
    result[H5T_NATIVE_B16]     = "H5T_NATIVE_B16";
    result[H5T_NATIVE_B32]     = "H5T_NATIVE_B32";
    result[H5T_NATIVE_B64]     = "H5T_NATIVE_B64";

    result[H5T_NATIVE_OPAQUE]  = "H5T_NATIVE_OPAQUE";
    result[H5T_NATIVE_HADDR]   = "H5T_NATIVE_HADDR";
    result[H5T_NATIVE_HSIZE]   = "H5T_NATIVE_HSIZE";
    result[H5T_NATIVE_HSSIZE]  = "H5T_NATIVE_HSSIZE";
    result[H5T_NATIVE_HERR]    = "H5T_NATIVE_HERR";
    result[H5T_NATIVE_HBOOL]   = "H5T_NATIVE_HBOOL";

    /*________________________________________________________________
      ANSI C9x-specific native integer datatypes
    */

    result[H5T_NATIVE_INT8]         = "H5T_NATIVE_INT8";
    result[H5T_NATIVE_UINT8]        = "H5T_NATIVE_UINT8";
    result[H5T_NATIVE_INT_LEAST8]   = "H5T_NATIVE_INT_LEAST8";
    result[H5T_NATIVE_UINT_LEAST8]  = "H5T_NATIVE_UINT_LEAST8";
    result[H5T_NATIVE_INT_FAST8]    = "H5T_NATIVE_INT_FAST8";
    result[H5T_NATIVE_UINT_FAST8]   = "H5T_NATIVE_UINT_FAST8";

    result[H5T_NATIVE_INT16]        = "H5T_NATIVE_INT16";
    result[H5T_NATIVE_UINT16]       = "H5T_NATIVE_UINT16";
    result[H5T_NATIVE_INT_LEAST16]  = "H5T_NATIVE_INT_LEAST16";
    result[H5T_NATIVE_UINT_LEAST16] = "H5T_NATIVE_UINT_LEAST16";
    result[H5T_NATIVE_INT_FAST16]   = "H5T_NATIVE_INT_FAST16";
    result[H5T_NATIVE_UINT_FAST16]  = "H5T_NATIVE_UINT_FAST16";

    result[H5T_NATIVE_INT32]        = "H5T_NATIVE_INT32";
    result[H5T_NATIVE_UINT32]       = "H5T_NATIVE_UINT32";
    result[H5T_NATIVE_INT_LEAST32]  = "H5T_NATIVE_INT_LEAST32";
    result[H5T_NATIVE_UINT_LEAST32] = "H5T_NATIVE_UINT_LEAST32";
    result[H5T_NATIVE_INT_FAST32]   = "H5T_NATIVE_INT_FAST32";
    result[H5T_NATIVE_UINT_FAST32]  = "H5T_NATIVE_UINT_FAST32";

    /*________________________________________________________________
      Intel-specific datatypes
    */

    result[H5T_INTEL_I8]   = "H5T_INTEL_I8";
    result[H5T_INTEL_I16]  = "H5T_INTEL_I16";
    result[H5T_INTEL_I32]  = "H5T_INTEL_I32";
    result[H5T_INTEL_I64]  = "H5T_INTEL_I64";

    result[H5T_INTEL_U8]   = "H5T_INTEL_U8";
    result[H5T_INTEL_U16]  = "H5T_INTEL_U16";
    result[H5T_INTEL_U32]  = "H5T_INTEL_U32";
    result[H5T_INTEL_U64]  = "H5T_INTEL_U64";

    result[H5T_INTEL_B8]   = "H5T_INTEL_B8";
    result[H5T_INTEL_B16]  = "H5T_INTEL_B16";
    result[H5T_INTEL_B32]  = "H5T_INTEL_B32";
    result[H5T_INTEL_B64]  = "H5T_INTEL_B64";

    result[H5T_INTEL_F32]  = "H5T_INTEL_F32";
    result[H5T_INTEL_F64]  = "H5T_INTEL_F64";

    return result;
  }

  //_____________________________________________________________________________
  //                                                                    datatypes
  
  std::vector<hid_t> HDF5Datatype::datatypes ()
  {
    std::map<hid_t,std::string> typesMap = datatypesMap();
    std::map<hid_t,std::string>::iterator it;
    std::vector<hid_t> result;

    for (it=typesMap.begin(); it!=typesMap.end(); ++it) {
      result.push_back(it->first);
    }

    return result;
  }
  
  //_____________________________________________________________________________
  //                                                                datatypeNames
  
  std::vector<std::string> HDF5Datatype::datatypeNames ()
  {
    std::map<hid_t,std::string> typesMap = datatypesMap();
    std::map<hid_t,std::string>::iterator it;
    std::vector<std::string> result;

    for (it=typesMap.begin(); it!=typesMap.end(); ++it) {
      result.push_back(it->second);
    }

    return result;
  }
  
  //_____________________________________________________________________________
  //                                                                     datatype
  
  hid_t HDF5Datatype::datatype (hid_t const &id)
  {
    hid_t result                             = -1;
    std::map<hid_t,std::string> typesMap     = datatypesMap();
    std::map<hid_t,std::string>::iterator it = typesMap.find(id);

    if (it==typesMap.end()) {
      std::cerr << "[HDF5Datatype::datatype]"
		<< " Unrecognized datatype!"
		<< std::endl;
    } else {
      result = it->first;
    }

    return result;
  }

  //_____________________________________________________________________________
  //                                                                 datatypeName
  
  /*!
    \param dtype -- Datatype for which to retrieve the name. In case the 
           identifier does not point to a datatype, but an Attribute or Dataset,
	   the datatype of the latter is retrieved.
  */
  std::string HDF5Datatype::datatypeName (hid_t const &id)
  {
    H5I_type_t otype    = HDF5Object::objectType (id);
    hid_t datatype      = 0;
    std::string name    = "UNDEFINED";
    std::map<hid_t,std::string> typesMap     = datatypesMap();
    std::map<hid_t,std::string>::iterator it = typesMap.find(id);
    
    /*________________________________________________________________
      Depending on the inspected HDF5 object, we need to extract the
      identifier for the datatype first.
    */

    if (it==typesMap.end()) {

      /* Unable to find datatype class directly ... */

      switch (otype) {
      case H5I_ATTR:
	datatype = H5Aget_type(id);
	name     = datatypeName(datatype);
	break;
      case H5I_DATASET:
	datatype = H5Dget_type(id);
	name     = datatypeName(datatype);
	break;
      default:
	std::cerr << "[HDF5Datatype::datatypeName]"
		  << " Unable to determine datatype to retrieve name!"
		  << std::endl;
	break;
      };

    } else {
      name = it->second;
    }
    
    return name;
  }
  
} // Namespace DAL -- end
