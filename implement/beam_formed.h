/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

/*!
   \class beam_formed
    
   \ingroup DAL
    
   \brief High-level interface between beam-formed data and the DAL
    
   \author Joseph Masters

 */
 
 class beam_formed {

    //! HDF5 file handle ID
    hid_t H5fileID_p;

    //! Identifier for this group within the HDF5 file
    hid_t H5groupID_p;

    //! Group object of the Data Access Library 
    dalGroup *group_p;

    //! Filename of the dataset
    string filename_p;

    //! DAL Dataset object to handle the basic I/O
    dalDataset *dataset_p;
 
   public:
   
    /*!
      \brief Argumented constructor

      \param filename -- Name of the file from which to read in the data
    */
	beam_formed(string const &filename);
	

    /*!
      \brief Destructor
    */
    ~beam_formed();
    

    /*!
      \brief Get the name of the data file
      
      \return filename -- The name of the data file
    */
    inline string filename () const {
      return filename_p;
    }
	
    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () {
      summary (cout);
    }

    /*!
      \brief Provide a summary of the internal status
    */
    void summary (ostream &os);    

    /*!
      \brief Get the name of the telescope
      
      \return telescope -- The name of the telescope with which the data were
              recorded; returns an empty string in case no keyword value could
	      be extracted.
    */
    std::string telescope ();

    /*!
      \brief Get the name of the observer
      
      \return observer -- The name of the observer; returns an empty string in
              case no keyword value could be extracted.
    */
    std::string observer ();

 }