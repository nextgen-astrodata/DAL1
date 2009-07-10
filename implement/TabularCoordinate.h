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

#ifndef TABULARCOORDINATE_H
#define TABULARCOORDINATE_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <Coordinate.h>

namespace DAL   // Namespace DAL -- begin
  {

  /*!
    \class TabularCoordinate

    \ingroup DAL

    \brief Brief description for class TabularCoordinate

    \author Lars B&auml;hren

    \date 2009/06/24

    \test tTabularCoordinate.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>[start filling in your text here]
    </ul>

    <h3>Synopsis</h3>

    <h3>Example(s)</h3>

  */
  class TabularCoordinate : public Coordinate
    {

      //! List of pixel values
      std::vector<double> pixelValues_p;
      //! List of world values
      std::vector<double> worldValues_p;

    public:

      // ------------------------------------------------------------- Construction

      //! Default constructor
      TabularCoordinate ();
      //! Argumented constructor
      TabularCoordinate (std::vector<std::string> const &axisNames,
                         std::vector<std::string> const &axisUnits,
                         std::vector<double> const &pixelValues,
                         std::vector<double> const &worldValues);
      //! Copy constructor
      TabularCoordinate (TabularCoordinate const &other);

      // -------------------------------------------------------------- Destruction

      //! Destructor
      ~TabularCoordinate ();

      // ---------------------------------------------------------------- Operators

      /*!
        \brief Overloading of the copy operator

        \param other -- Another TabularCoordinate object from which to make a copy.
      */
      TabularCoordinate& operator= (TabularCoordinate const &other);

      // --------------------------------------------------------------- Parameters

      //! Get the pixel values
      inline std::vector<double> pixelValues ()
      {
        return pixelValues_p;
      }
      //! Set the pixel values
      bool setPixelValues (std::vector<double> const &pixelValues);
      //! Get the world values
      inline std::vector<double> worldValues ()
      {
        return worldValues_p;
      }
      //! Set the world values
      bool setWorldValues (std::vector<double> const &worldValues);
      //! Set the values along the pixel and world axis
      bool setAxisValues (std::vector<double> const &pixelValues,
                          std::vector<double> const &worldValues);

      /*!
        \brief Get the name of the class

        \return className -- The name of the class, TabularCoordinate.
      */
      inline std::string className () const
        {
          return "TabularCoordinate";
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

      //! Write the coordinate object to a HDF5 file
      void h5write (hid_t const &groupID);

      //! Write the coordinate object to a HDF5 file
      void h5write (hid_t const &locationID,
                    std::string const &name);

      //! Read the coordinate object from a HDF5 file
      void h5read (hid_t const &groupID);

      //! Read the coordinate object from a HDF5 file
      void h5read (hid_t const &locationID,
                   std::string const &name);

    private:

      //! Initialize internal parameters
      void init ()
      {
        pixelValues_p.resize(nofAxes_p);
        worldValues_p.resize(nofAxes_p);
        //
        pixelValues_p = std::vector<double>(nofAxes_p,0.0);
        worldValues_p = std::vector<double>(nofAxes_p,0.0);
      }

      //! Unconditional copying
      void copy (TabularCoordinate const &other);

      //! Unconditional deletion
      void destroy(void);

    }; // Class TabularCoordinate -- end

} // Namespace DAL -- end

#endif /* TABULARCOORDINATE_H */

