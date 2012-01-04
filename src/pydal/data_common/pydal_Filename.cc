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

/*!
  \file pydal_Filename.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::Filename class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <data_common/Filename.h>

// namespace usage
using std::string;
using DAL::Filename;

void export_Filename ()
{

  /* Enumeration: File type */
  boost::python::enum_<Filename::Type>("Type")
    .value("uv",      Filename::uv)
    .value("sky",     Filename::sky)
    .value("rm",      Filename::rm)
    .value("nfi",     Filename::nfi)
    .value("dynspec", Filename::dynspec)
    .value("bf",      Filename::bf)
    .value("tbb",     Filename::tbb)
    ;
  
  /* Enumeration: File extension */
  boost::python::enum_<Filename::Extension>("Extension")
    .value("MS",     Filename::MS)
    .value("h5",     Filename::h5)
    .value("fits",   Filename::fits)
    .value("log",    Filename::log)
    .value("parset", Filename::parset)
    .value("lsm",    Filename::lsm)
    .value("IM",     Filename::IM)
    .value("PD",     Filename::PD)
    .value("vds",    Filename::vds)
    .value("gds",    Filename::gds)
    .value("conf",   Filename::conf)
    ;

  //________________________________________________________
  // Specialisation of overloaded methods

  void (Filename::*summary1)() 
    = &Filename::summary;
  void (Filename::*summary2)(std::ostream &) 
    = &Filename::summary;
  
  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<Filename>("Filename")
    // Construction
    .def( boost::python::init<>())
    .def( boost::python::init<string,string,Filename::Type,Filename::Extension>())
    // Parameters
    .def( "observationID", &Filename::observationID,
	  "Get the unique observation ID.")
    .def( "setObservationID", &Filename::setObservationID,
	  "Set the unique observation ID.")
    .def( "optionalDescription", &Filename::optionalDescription,
	  "Get optional descriptors.")
    .def( "setOptionalDescription", &Filename::setOptionalDescription,
	  "Set optional descriptors.")
    .def( "filetype", &Filename::filetype,
	  "Get the file type.")
    .def( "filetypeName", &Filename::filetypeName,
	  "Get the file type name.")
    .def( "setFiletype", &Filename::setFiletype,
	  "Set the file type.")
    .def( "extension", &Filename::extension,
	  "Get the file extension type.")
    .def( "extensionName", &Filename::extensionName,
	  "Get the file extension name.")
    .def( "setExtension", &Filename::setExtension,
	  "Set the file extension type.")
    .def( "filename", &Filename::filename,
	  "Get the name of the file.")
    // Methods
    .def("summary", summary1)
    .def("summary", summary2)
    ;

}
