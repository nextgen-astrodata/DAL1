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

#ifndef OPERATOR_H
#define OPERATOR_H

/* Standard library header files */
#include <iostream>
#include <map>
#include <set>
#include <string>

/* DAL header files */
#include <core/dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Operator
    
    \ingroup DAL
    \ingroup core
    
    \brief Definition of operator types
    
    \author Lars B&auml;hren

    \date 2011-08-18

    \test tOperator.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class Operator {
    
  public:

    /*!
      \brief Operator types

      The various available operators roughly can be divided into three groups:
      <table>
        <tr>
	  <td class="indexkey">Combination / Comparison</td>
	  <td>And, Or, Xor, Not, Equal, NotEqual, Greater, GreaterEqual</td>
	</tr>
        <tr>
	  <td class="indexkey">Bitwise combination</td>
	  <td>BitAnd, BitOr, BitXor, BitNot, BitEqual</td>
	</tr>
        <tr>
	  <td class="indexkey">Mathematical operation</td>
	  <td>Plus, Minus, Times, Divide</td>
	</tr>
      </table>
    */
    enum Types {
      /*____________________________________________________
	Combination / Comparison
      */
      //! Logical AND (\c &&)
      And,
      //! Logical OR (\c ||)
      Or,
      //! Logical XOR
      Xor,
      //! Logical NOT (\c !=)
      Not,
      //! (Logical) EQUAL (\c ==)
      Equal,
      //! Not equal (\c !=)
      NotEqual,
      //! Greater (\c >)
      Greater,
      //! Greater equal (\c >=)
      GreaterEqual,
      //! Lesser (\c <)
      Lesser,
      //! Lesser equal (\c <=)
      LesserEqual,
      /*____________________________________________________
	Bitwise combination
      */
      //! Bitwise AND
      BitAnd,
      //! Bitwise OR
      BitOr,
      //! Bitwise XOR
      BitXor,
      //! Bitwise negate
      BitNot,
      /*____________________________________________________
	Mathematical operations
      */
      //! Addition (\c +)
      Plus,
      //! Substraction (\c -)
      Minus,
      //! Multiplication (\c *)
      Times,
      //! Division (\c /)
      Divide
    };

  private:

    //! Operator type
    Operator::Types itsType;

  public:
    
    // === Construction =========================================================
    
    //! Argumented constructor
    Operator (Operator::Types const &op) {
      itsType = op;
    }
    
    //! Copy constructor
    Operator (Operator const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~Operator ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    Operator& operator= (Operator const &other); 
    
    // === Parameter access =====================================================
    
    //! Get the type of the operator
    inline Operator::Types type () const {
      return itsType;
    }

    //! Get the name of the operator
    inline std::string name () {
      return name(itsType);
    }

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, Operator.
    */
    inline std::string className () const {
      return "Operator";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    
    
    // === Static methods =======================================================
    
    //! Get set of operator types available
    static std::map<Operator::Types,std::string> operatorsMap ();

    //! Get set of operator types available
    static std::set<Operator::Types> operatorTypes ();

    //! Get set of operator type names available
    static std::set<std::string> operatorNames ();

    //! Get set of operator types available
    static std::set<std::string> operatorSymbols ();

    //! Get name corresponding to operator type
    static std::string name (Operator::Types const &type);
    
  private:
    
    //! Unconditional copying
    void copy (Operator const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Operator -- end
  
} // Namespace DAL -- end

#endif /* OPERATOR_H */
  
