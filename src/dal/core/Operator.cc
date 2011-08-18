/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include <core/Operator.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  /*!
    \param other -- Another Operator object from which to create this new
           one.
  */
  Operator::Operator (Operator const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Operator::~Operator ()
  {
    destroy();
  }
  
  void Operator::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another Operator object from which to make a copy.
  */
  Operator& Operator::operator= (Operator const &other)
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
    \param other -- Another Operator object from which to make a copy.
  */
  void Operator::copy (Operator const &other)
  {
    itsType = other.itsType;
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
  void Operator::summary (std::ostream &os)
  {
    os << "[Operator] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                 operatorsMap
  
  /*!
    \return types -- Map with the available operator types and their
            corresponding names.
  */
  std::map<Operator::Types,std::string> Operator::operatorsMap ()
  {
    std::map<Operator::Types,std::string> types;

    /*______________________________________________________
      Combination / Comparison
    */
    types[Operator::And]          = "And";
    types[Operator::Or]           = "Or";
    types[Operator::Xor]          = "Xor";
    types[Operator::Not]          = "Not";
    types[Operator::Equal]        = "Equal";
    types[Operator::NotEqual]     = "NotEqual";
    types[Operator::Greater]      = "Greater";
    types[Operator::GreaterEqual] = "GreaterEqual";
    types[Operator::Lesser]       = "Lesser";
    types[Operator::LesserEqual]  = "LesserEqual";
    /*______________________________________________________
      Bitwise combination
    */
    types[Operator::BitAnd]       = "BitAnd";
    types[Operator::BitOr]        = "BitOr";
    types[Operator::BitXor]       = "BitXor";
    types[Operator::BitNot]       = "BitNot";
    /*______________________________________________________
      Mathematical operations
    */
    types[Operator::Plus]         = "Plus";
    types[Operator::Minus]        = "Minus";
    types[Operator::Times]        = "Times";
    types[Operator::Divide]       = "Divide";
    
    return types;
  }

  //_____________________________________________________________________________
  //                                                                operatorTypes
  
  std::set<Operator::Types> Operator::operatorTypes ()
  {
    std::map<Operator::Types,std::string> m = operatorsMap();
    std::map<Operator::Types,std::string>::iterator it;
    std::set<Operator::Types> types;

    for (it=m.begin(); it!=m.end(); ++it) {
      types.insert(it->first);
    }

    return types;
  }

  //_____________________________________________________________________________
  //                                                                operatorNames
  
  std::set<std::string> Operator::operatorNames ()
  {
    std::map<Operator::Types,std::string> m = operatorsMap();
    std::map<Operator::Types,std::string>::iterator it;
    std::set<std::string> names;

    for (it=m.begin(); it!=m.end(); ++it) {
      names.insert(it->second);
    }

    return names;
  }

  //_____________________________________________________________________________
  //                                                                         name
  
  /*!
    \param type  -- Operator \e type for which to retrieve the corresponding
           \e name.
    \return name -- Name corresponding to the provided operator \e type.
  */
  std::string Operator::name (Operator::Types const &type)
  {
    std::map<Operator::Types,std::string> m            = operatorsMap();
    std::map<Operator::Types,std::string>::iterator it = m.find(type);

    return it->second;
  }
  
} // Namespace DAL -- end
