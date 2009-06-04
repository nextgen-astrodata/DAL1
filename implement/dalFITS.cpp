/*-------------------------------------------------------------------------*
 | $Id:: dalFITS.h 2126 2008-11-07 13:31:59Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Sven Duscha (sduscha@mpa-garching.mpg.de)                                                        *
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

#include "dalFITS.h"

using namespace std;
using namespace casa;

namespace DAL
{
  // ============================================================================
  //
  //  Construction / Destruction
  //
  // ============================================================================
  
  /*! 
    \brief Default constructor, creating an initialised, but empty object
  */
  dalFITS::dalFITS()
  {
    // initialise critical pointers
    fptr=NULL;
    fitsstatus=0;
    filename="";
    lattice=NULL;
    chdu=0;  
  }


  /*! 
    \brief Constructor with associated filename
    
    \param &filename --
  */
  dalFITS::dalFITS(const string &filename) 
  {
    this->filename=filename;	// assign filename to object 
  }

  
  /*!
    \brief Destructor
  */
  dalFITS::~dalFITS() 
  {
    this->close();		// close the FITS file
    
    // deallocate memory: not necessary at the moment, since no memory allocated in constructor
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
 
  /*
    Functions to get FITS administrative handles (fileptr, lattice, hdu etc.)
  */
  
  
  /*
    File access functions open/close etc.  
  */

  /*!	Open a FITS file in mode: r, w, rw
  
    \param string --
  
    \return fitsret --
  */
  void dalFITS::open (int iomode)
  {
    // Open fits file, status will be forwarded to object variable fitsstatus
    // need std::string.cstr()-method to get const char* which is needed by fits_open_file
    if(!(fits_open_file(&fptr, filename.c_str(), iomode, &fitsstatus)))
    {
      throw "dalFITS::open";	// get fits error from fitsstatus property later
    }
  }
  

  void dalFITS::getLattice()
  //casa::Lattice<Float>* dalFITS::getLattice () // don't return, but set in object instead
  {
    casa::LatticeBase *latticeBase;	// generic lattice variable for casa lattice

    FITSImage::registerOpenFunction();	// Register the FITS and Miriad image types.


    latticeBase=ImageOpener::openImage (this->filename);	// try open the file with generic casa function
    
    if(lattice==NULL)			// on error	
    {
	throw "dalFITS::getLattice ";
    }

    // Currently only support double lattices:
    lattice=dynamic_cast<ImageInterface<Float>*>(latticeBase);

    /*
    // determine data type of lattice
    switch(lattice->dataType()){
	    case TpFloat:
		    lattice_float=dynamic_cast<ImageInterface<Float>*>(lattice_Q);
		    break;
	    case TpDouble:
		    lattice_float=dynamic_cast<ImageInterface<Float>*>(lattice_Q);
		    break;
	    case TpComplex:
		    lattice_complex=dynamic_cast<ImageInterface<Complex>*>(lattice_Q);
		    break;
	    case TpDComplex:
		    lattice_dcomplex=dynamic_cast<ImageInterface<DComplex>*>(lattice_Q);
		    break;
	    default:
		    throw AipsError("Image has an invalid data type");
		    break;
    }
    */
    
    // check if data type is in accordance with FITS header entry
  }


  void dalFITS::close ()
  {
    if(!(fits_close_file(fptr, &fitsstatus)))
    {
      throw "dalFITS::close";
    }
  }


  std::string dalFITS::getFitsError ()
  {
    char* fits_error_message;
  
    fits_get_errstatus(fitsstatus, fits_error_message);
    
    /* Martin's code
    void fitshandle::check_errors() const
   {
    if (status==0) return;
    char msg[81];
    fits_get_errstatus (status, msg);
    cerr << msg << endl;
    while (fits_read_errmsg(msg)) cerr << msg << endl;
    planck_fail("FITS error");
    }
   */
    
    return (string) fits_error_message;
  }


  void dalFITS::moveAbsoluteHDU(int hdu)
  {    
    if(!(fits_movabs_hdu(fptr, hdu, &hdutype, &fitsstatus)))
    {
      throw "dalFITS::moveAbsoluteHDU";
    }
    
    this->chdu=hdu; // Update chdu in dalFITS object
  }
  

  void dalFITS::moveRelativeHDU(int nhdu)
  {
    int hdupos=0;		// local variable to hold current hdu
  
    if(!(fits_movrel_hdu(fptr, nhdu, NULL, &fitsstatus)))	// try to move nhdu
    {
      throw "dalFITS::moveRelativeHDU";
    }
    
    fits_get_hdu_num(fptr, &hdupos);
    
    this->chdu=hdupos;		// update chdu in dalFITS object
  }


  int dalFITS::readCurrentHDU()
  {
    int hdupos=0;		// local variable to hold chdu
    
    if(!(fits_get_hdu_num(fptr, &hdupos)))
    {
      throw "dalFITS::readCurrentHDU";
    }
  
    this->chdu=hdupos;	// update chdu in dalFITS object
  
    return chdu;	// return to caller
  }


  int dalFITS::moveNameHDU(const std::string &extname)
  {
    int hdupos=0;		// local variable to hold chdu
    int hdutype=0;		// type of HDU

    // ignoring the version number of the extension
    if(!(fits_movnam_hdu(fptr, hdutype, const_cast<char*>(extname.c_str()) , NULL, &fitsstatus)))
    {
      throw "dalFITS::moveNameHDU";
    }
    
    fits_get_hdu_num(fptr, &hdupos);

    this->chdu=hdupos;		// update chdu in dalFITS object
    this->hdutype=hdutype;	// update hdutype in dalFITS object
  
    return hdutype;
  }


  int dalFITS::readHDUType()
  { 
    int hdutype=0;
  
    if(!(fits_get_hdu_type(fptr,  &hdutype, &fitsstatus)))
    {
      throw "dalFITS::readHDUType";
    }
    
    this->hdutype=hdutype; // update hdutype in dalFITS object
    
    return hdutype;
  }


  std::string dalFITS::readFilename()
  {  
    char hdutype=0;	// local variable to hold hdutype
  
    if(!(fits_file_name(fptr, &hdutype, &fitsstatus)))
    {
      throw "dalFITS::readFilename";
    }
    
    this->filename=filename; 	// update filename in dalFITS object
    this->hdutype=hdutype;	// update hdutype in dalFITS object
  
    return filename;
  }


  int dalFITS::readFileMode()
  {
    int mode;
  
    if(!(fits_file_mode(fptr, &mode, &fitsstatus)))
    {
      throw "dalFITS::readFileMode";
    }

    return mode;
  }

 
  std::string dalFITS::readURLType()
  {
    string urltype;
  
    if(!(fits_url_type(fptr, const_cast<char *> (urltype.c_str()), &fitsstatus)))
    {
      throw "dalFITS::readURLType";
    }

    return urltype;
  }


  int dalFITS::deleteFITSfile()
  {
    int fitsret=0;
    
    if(!(fitsret=fits_delete_file(fptr, &fitsstatus)))
    {
      throw "dalFITS::deleteFITSfile";
    }
    
    return fitsret;
  }

 
  void dalFITS::flushFITSfile()
  {
    if(!(fits_flush_file(fptr, &fitsstatus)))
    {
      throw "dalFITS::fits_flush_file";
    }
  }


  void dalFITS::flushFITSBuffer()
  {
    if(!(fits_flush_buffer(fptr, 0, &fitsstatus)))
    {
      throw "dalFITS::fitsFITSBbuffer";
    }
  }


  // ============================================================================
  //
  //	Image access functions
  //
  // ============================================================================

  /*! 
      \brief Get image type of the FITS image
    
      \return bitpix - Bits per pixel
   */
  int dalFITS::getImgType()
  {
    int bitpix=0;

    if(!(fits_get_img_type(fptr, &bitpix, &fitsstatus)))
    {
      throw "dalFITS::getImgType";
    }

    return bitpix;	// pass on cfitsio return value
  }


  /*! 
    \brief Get image dimensions of the FITS image
    
    \return naxis - Number of axis in image
  */
  int dalFITS::getImgDim()
  {
    int naxis=0;

    if(!(fits_get_img_dim(fptr, &naxis,  &fitsstatus)))
    {
      throw "dalFITS::getImgDim";
    }

    return naxis;	// return number of axes 
  }


  /*!
      \brief Get image size of the FITS image
  
      \param maxdim - Maximum number of dimensions
      \param &naxes - Array to hold axes lengths
  */
  void dalFITS::getImgSize(int maxdim,  long &naxes)
  { 
    if(!(fits_get_img_size(fptr, maxdim, &naxes , &fitsstatus)))
    {
      throw "dalFITS::getImageSize";
    }
  }


  /*!
    \brief Get image parameters: maxdim, bitpix, naxis, naxes

    \param maxdim - Maximum number of dimensions returned
    \param *bitpix - Bits per pixel
    \param *naxis - Number of axes
    \param *naxes - Array with length of each axis
  */
  void dalFITS::getImgParam(int maxdim,  int &bitpix, int &naxis, long &naxes)
  {
    if(!(fits_get_img_param(fptr, maxdim, &bitpix, &naxis, &naxes, &fitsstatus)))
    {
      throw "dalFITS::getImageParam";
    }
  }

 
  /*!
    \brief Create an image extension

    \param bitpix - Bits per pixel
    \param naxis - Number of axes
    \param *naxes - Array with length of each axis
  */
  void dalFITS::createImg(int bitpix, int naxis, long *naxes)
  {
    if(!(fits_create_img(fptr, bitpix, naxis , naxes, &fitsstatus)))
    {
      throw "dalFITS::createImg";
    }
  }


  /*!
    \brief Write nelements pixels to FITS image extension
    
    \param datatype - Datatype of elements in array
    \param *fpixel - Starting pixel location
    \param nelements - Number of elements to write
    \param *array - Array containing data
  */
  void dalFITS::writePix(int datatype, long *fpixel, long nelements, void *array)
  {
    if(!(fits_write_pix(fptr, datatype, fpixel, nelements, array, &fitsstatus)))
    {
      throw "dalFITS::writePix";
    }
  }


  /*!
    \brief Write pixels to FITS image (any undefined value is replaced by nulval)
  
    \param datatype - Datatype of elements in array
    \param *fpixel - Starting pixel location
    \param nelements - Number of elements to write
    \param *array - Array containing data
    \param *nulval - Nullvalue to be written to file
  */
  void dalFITS::writePixNull(int datatype, long *fpixel, long nelements, void *array, void *nulval)
  {
    if(!(fits_write_pixnull(fptr, datatype, fpixel , nelements, array, nulval, &fitsstatus)))
    {
      throw "dalFITS::writePix";
    }
  }

  /*! 
    \brief Read pixels from an FITS image
    
    \param datatype - Datatype of elements in array
    \param *fpixel - Axes direction definition
    \param nelements - Number of elements to write
    \param *nulval - Nullvalue to be written to file
    \param *array - Array containing data
    \param *anynul - If any null value was encountered
  */  
  void dalFITS::readPix(int datatype, long *fpixel, long nelements, void *nulval, void *array, int *anynul)
  {
    if(!(fits_read_pix(fptr, datatype, fpixel, nelements, nulval, array, anynul, &fitsstatus)))
    {
      throw "dalFITS::readPix";
    }
  }


  /*!
    \brief Read a subset of a FITS image
    
    \param datatype - datatype contained in array
    \param *fpixel - array giving lower left corner of reading
    \param *lpixel - array giving upper right corner of reading
    \param *inc - increment step
    \param *nulval - nulval used if NULL pixels are read
    \param *array - array to read into
    \param *anynul - if any null vallue was encountered
  */
  void dalFITS::readSubset(int  datatype, long *fpixel,
	  long *lpixel, long *inc, void *nulval,  void *array,
	  int *anynul)
  {
    if(!(fits_read_subset(fptr, datatype, fpixel, lpixel, inc, nulval, array, anynul, &fitsstatus)))
    {
      throw "dalFITS::readSubset";
    }
  }


  /*! 
      \brief Write a subset to a FITS image
      
      \param datatype - datatype contained in array
      \param *fpixel - array giving lower left corner of reading
      \param *lpixel - array giving upper right corner of writing
      \param *array - array containing data
  */
  void dalFITS::writeSubset(int datatype, long *fpixel, long *lpixel, double *array)
  {
    if(!(fits_write_subset(fptr, datatype, fpixel, lpixel, array, &fitsstatus)))
    {
      throw "dalFITS::writeSubset";
    }
  }




  // ============================================================================
  //
  //  RM-Cube Input functions
  //
  // ============================================================================  

  /*!
    \brief Read an imageplane from a FITS image
 
    \param *plane - pointer to array holding the data read from the image
    \param z - z axis position to read plane from
  */
  void dalFITS::readPlane(double *plane, int z)
  {
    // Check if z is within the FITS cube
    if(z >= dimensions[2])
    {
      throw "dalFITS::readPlane out of range";
    }
    
    // Check if current HDU is an image extension
    
    // Read from FITS file

  }


  /*!
    \brief Read a line from a FITS image
 
    \param line - pointer to array holding the data read from the image
    \param x - x axis lower left corner position to read line from
    \param y - y axis lower left corner position to read line from
  */
  void dalFITS::readLine(vector<double> &line, int x, int y)
  {
    // Check if vector has right dimension, same as z dimension of cube
    if(line.size()!=dimensions[2])
    {
      throw "dalFITS::readLine size does not match";
    }
   
    // Check if current HDU is an image extension and of type double
    if(getImgType()!=32)
    {
      throw "dalFITS::readLine chdu is no 32bpix image";
    }
   
    // Read subset from FITS file
    /*readSubset(int  datatype, long *fpixel,
             long *lpixel, long *inc, void *nulval,  void *array,
             int *anynul);
    */
  }


  /*!
    \brief Read a subCube from a FITS image
 
    \param *subCube - pointer to array holding the data read from the image
    \param x_pos - lower left corner x position of subCube
    \param y_pos - lower left corner y position of subCube
    \param x_size - size in x direction in pixels
    \param y_size - size in y direction in pixels
  */
  void dalFITS::readSubCube(double *subCube, int x_pos, int y_pos, int x_size, int y_size)
  {
    long fpixel[3];	// axis definition

    // Check if current HDU is an image extension
    
    
    // Read subset from FITS Image
 
  
  }

  
  // ============================================================================
  //
  //  RM-Cube output functions
  //
  // ============================================================================  

  /*!
    \brief Write an imageplane to a FITS file
    
    \param *plane - array containing imageplane data
    \param x - horizontal dimension in pixels
    \param y - vertical dimension in pixels
    \param z - z position to write plane to
  */
  void dalFITS::writePlane(double *plane, int x, int y, int z)
  {
    // Check if Faraday plane has the same x-/y-dimensions as naxes dimensions of FITS
    if(x!=dimensions[0] || y!=dimensions[1])
    {
      throw "dalFITS::writePlane dimensions do not match";  
    }
 
    // check if plane counter is below limit
      
      
    // Write to FITS file
 
 
  }

  /*!
    \brief Write a line of sight to a FITS file

    \param faradayLine - contains line of sight along Faraday Depths with RM intensity
    \param x - x position in pixels to write line to
    \param y - y position in pixels to write line to
  */
  void dalFITS::writeLine(vector<double> line, int x, int y)
  {
    // Check if vector has equal length to z-dimension of image cube
    if(line.size() != dimensions[2])
    {
      throw "dalFITS::writeLine size does not match";
    }
    
  
  }


  /*! 
    \brief Write a tile to a FITS file
  
    \param tile --
    \param x --
    \param y --
  */
  void dalFITS::writeTile(double* tile,
			  int x_pos,
			  int y_pos,
			  int x_size,
			  int y_size
			  )
  {


  }
  
  
  /*! 
    \brief Write a SubCube to a FITS file
    
    \param cube - Array that contains data 
    \param x_pos - x position in pixels to write cube to
    \param y_pos - y position in pixels to write cube to
    \param x_size - x-dimension of cube in pixels
    \param y_size - y-dimension of cube in pixels
  */
  void dalFITS::writeSubCube(double* subcube,
			  int x_size,
			  int y_size,
			  int x_pos,
			  int y_pos)
  {
    // Check if position is valid
    
    // check if size goes beyond limits of cube

    // Write to FITS file
  }




  // ============================================================================
  //
  //  Header access functions (keywords etc.)
  //
  // ============================================================================


  // Methods for reading keywords and records from a dalFITS file
  void dalFITS::getHDRspace(int &keysexist,
			   int &morekeys)
  {
    if(!(fits_get_hdrspace(fptr, &keysexist, &morekeys, &fitsstatus)))
    {
      throw "dalFITS::getHDRspace";
    }
  }


  void dalFITS::readRecord(int keynum,
			  std::string record)
  {
    if(!(fits_read_record(fptr, keynum, const_cast<char *>(record.c_str()), &fitsstatus)))
    {
      throw "dalFITS::readRecord";
    }
  }
  
  
  void dalFITS::readCard(std::string keyname, std::string record)
  {
    if(!(fits_read_card(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(record.c_str()), &fitsstatus)))
    {
      throw "dalFITS::readCard";
    }
  }

  /*!
    \param datatype
    \param keyname
    \param value
    \param comment
  */
  void dalFITS::readKey(int datatype,
		       char *keyname,
		       void *value,
		       char *comment)
  {
    if(!(fits_read_key(fptr, datatype, keyname, value, comment, &fitsstatus)))
    {
      throw "dalFITS::readKey";
    }
  }
    
  /*!
    \param inclist
    \param ninc
    \param exclist
    \param nexc
    \param card
  */
  void dalFITS::findNextKey(char **inclist,
			   int ninc,
			   char **exclist,
			   int nexc,
			   char *card)
  {
    if(!(fits_find_nextkey(fptr, inclist, ninc, exclist, nexc, card, &fitsstatus)))
    {
      throw "dalFITS::findNextKey";
    }
  }

  /*!
    \param keyname
    \param unit
  */
  void dalFITS::readKeyUnit(std::string keyname,
			   std::string unit)
  {
    if(!(fits_read_key_unit(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(unit.c_str()), &fitsstatus)))
    {
      throw "dalFITS::readKeyUnit";
    }
  }


  //===============================================================
  //
  // Methods for writing keywords and records to a dalFITS file
  //
  //===============================================================

  /*!
    \param 
  */
  void dalFITS::writeKey(int datatype,
			std::string keyname,
			void *value,
			std::string comment)
  {
    if(!(fits_write_key(fptr, datatype, const_cast<char 
				*>(keyname.c_str()), value, const_cast<char *>(comment.c_str()), &fitsstatus)))
      {
	throw "dalFITS::writeKey";
      }
  }
  
  /*!
    \param datatype
    \param keyname
    \param value
    \param comment
  */
  void dalFITS::updateKey(int datatype,
			 std::string keyname,
			 void *value,
			 std::string comment)
  {
  
    if(!(fits_update_key(fptr,
				 datatype,
				 const_cast<char *>(keyname.c_str()),
				 value,
				 const_cast<char *>(comment.c_str()),
				 &fitsstatus)))
      {
	throw "dalFITS::updateKey";
      }
  }
  
  
  void dalFITS::writeRecord(std::string card)
  {
    if(!(fits_write_record(fptr, const_cast<char *>(card.c_str()), &fitsstatus)))
    {
      throw "dalFITS:writeRecord";
    }
  }
  

  void dalFITS::modifyComment(std::string keyname, std::string comment)
  {
    if(!(fits_modify_comment(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(comment.c_str()), &fitsstatus)))
    {
      throw "dalFITS::modifyComment";
    }
  }


  void dalFITS::writeKeyUnit(std::string keyname, std::string unit)
  {
    if(!(fits_write_key_unit(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(unit.c_str()), &fitsstatus)))
    {
      throw "dalFITS::writeKeyUnit";
    }
  }


  void dalFITS::writeComment(std::string comment)
  {
    if(!(fits_write_comment(fptr, const_cast<char *>(comment.c_str()),  &fitsstatus)))
    {
      throw "dalFITS::writeComment";
    }
  }


  void dalFITS::writeHistory(std::string history)
  {
    if(!(fits_write_history(fptr, const_cast<char *>(history.c_str()),  &fitsstatus)))
    {
      throw "dalFITS::writeHistory";
    }
  }
  

  void dalFITS::writeDate()
  {
    if(!(fits_write_date(fptr,  &fitsstatus)))
    {
      throw "dalFITS::writeDate";
    }
  }


  void dalFITS::deleteRecord(int keynum)
  {
    if(!(fits_delete_record(fptr, keynum,  &fitsstatus)))
    {
      throw "dalFITS::deleteRecord";
    }
  }


  void dalFITS::deleteKey(std::string keyname)
  {
    if(!(fits_delete_key(fptr, const_cast<char *>(keyname.c_str()),  &fitsstatus)))
    {
      throw "dalFITS::deleteKey";
    }
  }


  // Header utility routines
  
  void dalFITS::copyHeader(dalFITS &other)
  {
    if(!(fits_copy_header(fptr, other.fptr, &fitsstatus)))
    {
      throw "dalFITS::copyHeader";
    }
  }


  void dalFITS::writeChecksum()
  {
    if(!(fits_write_chksum(fptr, &fitsstatus)))
    {
      throw "dalFITS::writeChecksum";
    }
  }

  
  void dalFITS::verifyChecksum(bool &dataok, bool &hduok)
  {
    int dataok_int=0, hduok_int=0;
  
    if(!(fits_verify_chksum(fptr, &dataok_int, &hduok_int, &fitsstatus)))
    {
      throw "dalFITS::verifyChecksum";
    }

    // convert int dataok and int hduk to bools
    if(dataok_int)
      dataok=true;
    if(hduok_int)
      hduok=true;
  }


  void dalFITS::parseValue(std::string card, std::string value, std::string comment)
  {
    if(!(fits_parse_value(const_cast<char *>(card.c_str()), const_cast<char *>(value.c_str()), const_cast<char *>(comment.c_str()), &fitsstatus)))
    {
      throw "dalFITS::parseValue";
    }
  }


  void dalFITS::getKeytype(std::string value, char &dtype)
  {   
    if(!(fits_get_keytype(const_cast<char *>(value.c_str()), &dtype, &fitsstatus)))
    {
      throw "dalFITS::getKeytype";
    }
  }


  void dalFITS::getKeyclass(std::string card)
  {
    if(!(fits_get_keyclass(const_cast<char *>(card.c_str()))))
    {
      throw "dalFITS::getKeyclass";
    }
  }
  
  
  void dalFITS::parseTemplate(std::string templatec, std::string card, int &keytype)
  {
    if(!(fits_parse_template(const_cast<char *>(templatec.c_str()), const_cast<char *>(card.c_str()), &keytype, &fitsstatus)))
    {
      throw "dalFITS::parseTemplate";
    }
  }


  // ============================================================================
  //
  //  Header access functions (specific to RM)
  //
  // ============================================================================

  /*!
    \brief Write header information specific to a RM cube
    
    \param hdu to write to (default 1)
  */
  void dalFITS::writeRMHeader(int hdu)
  {    
    moveAbsoluteHDU(hdu);

    // Check if it is an image extension

    // Write individual header keywords to FITS
    
    // faraday low
    // faraday high
    // algorithm used to create RM cube
  }


  // ============================================================================
  //
  //	Table access functions (not necessary for RM cubes at the moment)
  //
  // ============================================================================

  // Implement these in a dalFITSTable.h/.cpp class?


  // ============================================================================
  //
  //  Class summary functions & Co.
  //
  // ============================================================================

  /*!
      \param os -- Output stream to which the summary is going to be written
  */

  void dalFITS::summary (std::ostream &os)
  {
    os << "[dalFITS] Summary of object properties" << std::endl;
    os << "-- Filename : " << std::endl;
  }
  
}
