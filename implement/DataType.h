/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 2286 2009-02-03 10:50:48Z baehren                    $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#ifndef DATATYPE_H
#define DATATYPE_H

// Standard library header files
#include <iostream>
#include <string>

namespace DAL   // Namespace DAL -- begin
  {

  /*!
    \class DataType

    \ingroup DAL

    \brief Type descriptors for standard LOFAR data products

    \author Lars B&auml;hren

    \date 2009/04/18

    \test tDataType.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>[start filling in your text here]
    </ul>

    <h3>Synopsis</h3>

    Naming convention for standard LOFAR data products:
    \verbatim
    lofar_<ObsID>_<Date>_{processing}_<Type>.[h5,fits,casa]
    \endverbatim
    where the name is composed of the following elements:
    <ul>
      <li>\b lofar – identifies the telescope
      <li>\b ObsID – unique identifier of the observation
      <li>\b Date – most likely file creation date, but this also might be the
      pipeline processing date → start of the observation, yyyymmdd
      <li>\b processing – optional identifier to e.g mark splitting of an image
      into sub-bands
      <li>\b Type – descriptor for the file type
      <ul>
        <li>VIS – Visibility data
  <li>SKY – (multi-dimensional) sky image
  <li>RMC – Rotation measure cube
  <li>RMM – Rotation measure map
  <li>DYS – Dynamic spectrum
  <li>CRC – Cosmic ray skymap cube
  <li>BFS – Beamformed station data
  <li>BFT – Beamformed tied-array data
  <li>TBB – Transient buffer board dump (base-band and frequency data)
      </ul>
    </ul>

    <h3>Example(s)</h3>

  */
  class DataType
    {

    public:

      //! Type descriptors for standard LOFAR data products
      enum Types
      {
        //! Beamformed station data
        BFS,
        //! Beamformed tied-array data
        BFT,
        //! Cosmic ray skymap (hyper-)cube, typically [Lon,Lat,Radius,Freq,Time]
        CRC,
        //! Dynamic spectrum, typically [Time,Freq]
        DYS,
        //! Rotation measure cube
        RMC,
        //! Rotation measure map
        RMM,
        //! (multi-dimensional) sky image, typically [Ra,Dec,Freq]
        SKY,
        //! Transient buffer board dump (base-band and frequency data)
        TBB,
        //! Visibility data
        VIS
      };

    private:

      //! Data type
      DataType::Types type_p;

    public:

      // ------------------------------------------------------------- Construction

      //! Default constructor
      DataType (DataType::Types const &type=DataType::SKY);

      /*!
        \brief Copy constructor

        \param other -- Another DataType object from which to create this new
               one.
      */
      DataType (DataType const &other);

      // -------------------------------------------------------------- Destruction

      //! Destructor
      ~DataType ();

      // ---------------------------------------------------------------- Operators

      /*!
        \brief Overloading of the copy operator

        \param other -- Another DataType object from which to make a copy.
      */
      DataType& operator= (DataType const &other);

      // --------------------------------------------------------------- Parameters

      //! Get the data type
      inline DataType::Types type ()
      {
        return type_p;
      }

      //! Set the data type
      inline void setType (DataType::Types const &type)
      {
        type_p = type;
      }

      //! Get the name of the data type
      std::string name ();

      /*!
        \brief Get the name of the class

        \return className -- The name of the class, DataType.
      */
      inline std::string className () const
        {
          return "DataType";
        }

      /*!
        \brief Provide a summary of the internal status
      */
      inline void summary ()
      {
        summary (std::cout);
      }

      /*!
        \brief Provide a summary of the internal status

        \param os -- Output stream to which the summary is written.
      */
      void summary (std::ostream &os);

      // ------------------------------------------------------------------ Methods

      bool isVisibility ();

    private:

      //! Unconditional copying
      void copy (DataType const &other);

      //! Unconditional deletion
      void destroy(void);

    }; // Class DataType -- end

} // Namespace DAL -- end

#endif /* DATATYPE_H */

