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
#include <sys/stat.h>	// needed to check for existence of a file

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
    lattice=NULL;
  }


  /*! 
    \brief Constructor with associated filename

    \param &filename - filename of FITS file
    \param iomode - I/O Read () OR Write 
  */
  dalFITS::dalFITS(const string &filename, int iomode) 
  {
    fptr=NULL; 		// initialise FITS filepointer
    fitsstatus=0;	// initialise FITS status
    lattice=NULL;	// initialise casa lattice
  
    // Check if file exists: if it exists open in iomode
    if(fileExists(filename))
    {
      // Depending on iomode: OPEN for READING,WRITING, RW or CREATE a FITS file
      if(fits_open_file(&fptr, filename.c_str(), iomode, &fitsstatus))
      {
	throw "dalFITS::open";	// get fits error from fitsstatus property later
      }
      
    }
    else
    {
      // if file didnt exist, create a new one ...
      if(fits_create_file(&fptr, const_cast<char *>(filename.c_str()), &fitsstatus))
      {
	throw "dalFITS::open could not create file";
      }
      
      // ... and open it
      if(fits_open_file(&fptr, filename.c_str(), iomode, &fitsstatus))
      {
	throw "dalFITS::open";	// get fits error from fitsstatus property later
      }
    }
  }


  /*!
    \brief Copy constructor that copies the whole FITS file

    \param other - dalFITS object to be copied to
  */
  dalFITS::dalFITS(dalFITS const &other)
  {
    if(fits_copy_file(fptr, other.fptr, 1, 1, 1, &fitsstatus))
    {
      throw "dalFITS::dalFITS copy constructor";
    }
  }
  

  /*! 
    \brief Copy constructor that copies a complete dalFITS object to another

    \param &other - other dalFITS object to be copied to
    \param previous - copy previous (before CHDU) HDUs to other dalFITS object
    \param current - copy current HDU to other dalFITS object
    \param following - copy following (after CHDU) HDUs to other dalFITS object
  */
  dalFITS::dalFITS(dalFITS const &other, bool previous, bool current, bool following)
  {
    int previousInt=0, currentInt=0, followingInt=0;	// copy prev, current, following to other FITS file
  
    if(previous)	// if previous bool is true...
      previousInt=1;	// ... set int logic variable to true
    if(current)		// and so on...
      currentInt=1;
    if(following)
      followingInt=1;
  
    if(fits_copy_file(fptr, other.fptr, previousInt, currentInt, followingInt, &fitsstatus))
    {
      throw "dalFITS::dalFITS copy constructor";
    }
  }
  
  
  /*!
    \brief Copy constructor that copies only one HDU and appends it to another dalFITS object
    
    \param &other - other dalFITS object to append HDU to
    \param hdu - HDU number to copy
  */
  void dalFITS::copyCHDU(dalFITS const &other)
  {
    int morekeys=0;	// Don't reserve space for more keys
  
    if(fits_copy_hdu(fptr, other.fptr, morekeys, &fitsstatus))
    {
      throw "dalFITS::dalFITS copy constructor";
    }
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
 

  /*! 
    \brief open a FITS file for read or readwrite and move to first HDU with significant data
    
    \param iomode - I/O-mode: READONLY, READWRITE
  */
  void dalFITS::openData (const std::string &filename, int iomode)
  { 
    if(fits_open_data(&fptr, const_cast<char *>(filename.c_str()), iomode , &fitsstatus))
    {
      throw "dalFITS::openData";
    }
 
    if(readHDUType()==IMAGE_HDU)	 // if it is an image extension
    {
      updateImageDimensions();		// Update dimensions-vector, 
    }

  }
  

  /*! 
    \brief open a FITS file for read or readwrite and move to first HDU with an image
    
    \param iomode - I/O-mode: R, RW
  */
  void dalFITS::openImage(const std::string &filename, int iomode)
  {
    if(fits_open_image(&fptr, const_cast<char *>(filename.c_str()), iomode, &fitsstatus))
    {
      throw "dalFITS::openImage";
    }
    
    if(readHDUType()==IMAGE_HDU)	 // if it is an image extension
    {
      updateImageDimensions();		// Update dimensions-vector, 
    }
  }


  /*! 
    \brief open a FITS file for read or readwrite and move to first HDU with a table
    
    \param iomode - I/O-mode: R, RW
  */
  void dalFITS::openTable(const std::string &filename, int iomode)
  {
    if(fits_open_table(&fptr, const_cast<char *>(filename.c_str()), iomode, &fitsstatus))
    {
      throw "dalFITS::openTable";
    }
    
  }

 
  /*!
    \brief Get a (casa) Lattice<Float> to access the fits file
  */
  void dalFITS::getLattice()
  //casa::Lattice<Float>* dalFITS::getLattice () // don't return, but set in object instead
  {
    casa::LatticeBase *latticeBase;	// generic lattice variable for casa lattice
    std::string filename;		// local string to hold filename

    FITSImage::registerOpenFunction();	// Register the FITS and Miriad image types.

    // Get filename of dalFITS object
    

    latticeBase=ImageOpener::openImage (filename);	// try open the file with generic casa function
    
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


  /*!
      \brief close a FITS file handle
  */
  void dalFITS::close ()
  {
    if(fits_close_file(fptr, &fitsstatus))
    {
      throw "dalFITS::close";
    }
  }


  /*! 
    \brief Get the corresponding cfits error message to the current fitsstatus
  
    \return fitserrormsg - Complete string with all FITS errors on the error stack
  */
  std::string dalFITS::getFitsError ()
  {
    char* fits_error_message;
    std::string complete_error_message;
  
    fits_get_errstatus(fitsstatus, fits_error_message);
 
    cerr << fits_error_message << endl;		// print error message on cerr
 
    while(fits_read_errmsg(fits_error_message)!=NULL)
    {
      cerr << fits_error_message << endl;		// print further messages
      complete_error_message += fits_error_message;	// concat complete error msg
    }
 
    /* Martin's code
    void fitshandle::check_errors() const
   {
    if (status==0) return;
    char msg[81];
    fits_get_errstatus (status, msg);
    cerr << msg << endl;
    while (firead_errmsg(msg)) cerr << msg << endl;
    planck_fail("FITS error");
    }
   */
    
    return (string) complete_error_message;
  }


  /*! 
    \brief Get the number of HDUs in the FITS file

    \return numHDUS - number of HDUs in FITS file
  */
  int dalFITS::readNumHDUs()
  {
    int hdunum=0;	// number of hdus in FITS file
  
    if(fits_get_num_hdus(fptr, &hdunum ,&fitsstatus))
    {
      throw "dalFITS::readNumHDUs";
    }
    
    return hdunum;
  }


  /*!	
    \brief Move to HDU unit given by hdu
  
    \param hdu - move to HDU number
  
    \return hdutype - type of HDU moved to
  */
  void dalFITS::moveAbsoluteHDU(int hdu)
  {
    if(fits_movabs_hdu(fptr, hdu, NULL, &fitsstatus))
    {
      throw "dalFITS::moveAbsoluteHDU";
    }

    if(readHDUType()==IMAGE_HDU)	 // if it is an image extension
    {
      updateImageDimensions();		// Update dimensions-vector, 
    }
  }


  /*! 
    \brief Move relative by hdu units
    
    \param hdu - move relative number of HDUs
  */  
  void dalFITS::moveRelativeHDU(int nhdu)
  {
    if(fits_movrel_hdu(fptr, nhdu, NULL, &fitsstatus))	// try to move nhdu
    {
      throw "dalFITS::moveRelativeHDU";
    }
    
    if(readHDUType()==IMAGE_HDU)	 // if it is an image extension
    {
      updateImageDimensions();		// Update dimensions-vector, 
    }

  }


  /*! 
    \brief Read the current header postion (HDU) in FITS file
    
    \return chdu - current HDU in FITS file
  */
  int dalFITS::readCurrentHDU()
  {
    int hdupos=0;		// local variable to hold chdu
    
    if(fits_get_hdu_num(fptr, &hdupos))
    {
      throw "dalFITS::readCurrentHDU";
    }
  
    return hdupos;		// return to caller
  }


  /*! 
    \brief Move to hdu extension with name, extname
    
    \param extname - extension name to move to

    \return hdutype - Type of HDU moved to
  */
  int dalFITS::moveNameHDU(const std::string &extname)
  {
    int hdutype=0;			// type of HDU

    // ignoring the version number of the extension
    if(fits_movnam_hdu(fptr, hdutype, const_cast<char*>(extname.c_str()) , NULL, &fitsstatus))
    {
      throw "dalFITS::moveNameHDU";
    }
    
    if(readHDUType()==IMAGE_HDU)	 // if it is an image extension
    {
      updateImageDimensions();		// Update dimensions-vector, 
    }
 
    return hdutype;
  }


  /*!
    \brief Update the information contained in the dimensions-vector of the dalFITS object
  */
  void dalFITS::updateImageDimensions()
  {
    int i=0;
    int naxis=0;
    long *naxes=0;

    if(readHDUType()==IMAGE_HDU)
    {
      naxis=getImgDim();			// get number of axis
      naxes=(long*) calloc(sizeof(long), naxis);// allocate memory for axis dimensions
    
      dimensions.resize(naxis);		// resize dimensions vector
      getImgSize(naxis, naxes);		// get the size of each axis
      
      for(i=0; i<naxis; i++)
      {
	dimensions[i]=naxes[i];
      }
    }  
  }
  

  /*!	
    \brief Read type of HDU (IMAGE_HDU, ASCII_TBL, or BINARY_TBL)

    \return hduype - Type of current HDU
  */
  int dalFITS::readHDUType()
  { 
    int hdutype=0;
  
    if(fits_get_hdu_type(fptr,  &hdutype, &fitsstatus))
    {
      throw "dalFITS::readHDUType";
    }
    
    return hdutype;
  }


  /*!
    \brief Read the filename of the currently opened FITS file

    \return filename - Name of FITS file currently opened in dalFITS object
  */
  std::string dalFITS::readFilename()
  {  
    std::string filename;	// local variable to hold FITS filename

    if(fits_file_name(fptr, const_cast<char *>(filename.c_str()), &fitsstatus))
    {
      throw "dalFITS::readFilename";
    }
  
    return filename;
  }


  /*! 
    \brief Read the IO mode of the currently opened FITS file  
  */    
  int dalFITS::readFileMode()
  {
    int mode;
  
    if(fits_file_mode(fptr, &mode, &fitsstatus))
    {
      throw "dalFITS::readFileMode";
    }

    return mode;
  }


  /*! 
    \brief Read the url type, e.g. file://, ftp:// of the currently opened FITS file
  */
  std::string dalFITS::readURLType()
  {
    string urltype;
  
    if(fits_url_type(fptr, const_cast<char *> (urltype.c_str()), &fitsstatus))
    {
      throw "dalFITS::readURLType";
    }

    return urltype;
  }

 
  /*!
    \brief Delete the fitsfile of the dalFITS object
  */
  void dalFITS::deleteFITSfile()
  {
    if(fits_delete_file(fptr, &fitsstatus))
    {
      throw "dalFITS::deleteFITSfile";
    }
  }


  /*!
    \brief Flush the FITS file, close and reopen
  */
  void dalFITS::flushFITSfile()
  {
    if(fits_flush_file(fptr, &fitsstatus))
    {
      throw "dalFITS::fits_flush_file";
    }
  }


  /*!
    \brief Flush buffer (without proper closing and reopening)
  */
  void dalFITS::flushFITSBuffer()
  {
    if(fits_flush_buffer(fptr, 0, &fitsstatus))
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

    if(fits_get_img_type(fptr, &bitpix, &fitsstatus))
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

    if(fits_get_img_dim(fptr, &naxis,  &fitsstatus))
    {
      throw "dalFITS::getImgDim";
    }

    dimensions.resize(naxis);	// resize dimensions vector in dalFITS object

    return naxis;	// return number of axes 
  }


  /*!
      \brief Get image size of the FITS image
  
      \param maxdim - Maximum number of dimensions
      \param &naxes - Array to hold axes lengths
  */
  void dalFITS::getImgSize(int maxdim,  long *naxes)
  { 
    unsigned int i=0;		// loop variable
  
    if(fits_get_img_size(fptr, maxdim, naxes , &fitsstatus))
    {
      throw "dalFITS::getImageSize";
    }

    for(i=0; i<dimensions.size(); i++)
      dimensions[i]=naxes[i];
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
    if(fits_get_img_param(fptr, maxdim, &bitpix, &naxis, &naxes, &fitsstatus))
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
    if(naxis==0 || naxes==NULL)
    {
      throw "dalFITS::createImg naxis or naxes NULL";
    }
  
    if(fits_create_img(fptr, bitpix, naxis , naxes, &fitsstatus))
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
    if(fits_write_pix(fptr, datatype, fpixel, nelements, array, &fitsstatus))
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
    if(fits_write_pixnull(fptr, datatype, fpixel , nelements, array, nulval, &fitsstatus))
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
    if(fits_read_pix(fptr, datatype, fpixel, nelements, nulval, array, anynul, &fitsstatus))
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
    if(fits_read_subset(fptr, datatype, fpixel, lpixel, inc, nulval, array, anynul, &fitsstatus))
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
    if(fits_write_subset(fptr, datatype, fpixel, lpixel, array, &fitsstatus))
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
  void dalFITS::readPlane(double *plane, const unsigned long z, void *nulval)
  {
    long fpixel[3]; 
    int nelements=0;
    int anynul=0;
  
    // Check if z is within the FITS cube
    if(z > dimensions[2])
    {
      throw "dalFITS::readPlane out of range";
    }
    
    if(readHDUType()!=IMAGE_HDU)	// Check if current HDU is an image extension
    {
      throw "dalFITS::readLine CHDU is not an image";
    }
 
    // Read from FITS file one plane
    fpixel[0]=1;
    fpixel[1]=1;
    fpixel[2]=z;  
    
    nelements=dimensions[0]*dimensions[1];	// compute number of elements in plane
    
 
    if(plane!=NULL)	// only if valid pointer is given
    {
      readPix(TDOUBLE, fpixel, nelements, nulval, plane, &anynul);
    }
    else
    {
      throw "dalFITS::readLine NULL pointer";
    }
  }


  /*!
    \brief Read a line from a FITS image
 
    \param line - pointer to array holding the data read from the image
    \param x - x axis lower left corner position to read line from
    \param y - y axis lower left corner position to read line from
  */
  void dalFITS::readLine(double *line, const unsigned long x, const unsigned long y, void* nulval)
  {
    long fpixel[3];
    long lpixel[3];
    int anynul=0;
  
    // Check if vector has right dimension, same as z dimension of cube
    if(readHDUType()!=IMAGE_HDU)	  // Check if current HDU is an image extension 
    {
      throw "dalFITS::readLine CHDU is not an image";
    }
 
    // Define first pixel to read, read along one line of sight
    fpixel[0]=x;
    fpixel[1]=y;
    fpixel[2]=1;
    lpixel[0]=x;
    lpixel[1]=y;
    lpixel[2]=dimensions[2];
    

    // Read subset from FITS file
    if(line==NULL)
    {
      throw "dalFITS::readLine NULL pointer";
    }
    else
    {
      readSubset(TDOUBLE, fpixel, lpixel, NULL, line, nulval, &anynul);
    }
  }


  /*!
    \brief Read a subCube from a FITS image
 
    \param *subCube - pointer to array holding the data read from the image
    \param x_pos - lower left corner x position of subCube
    \param y_pos - lower left corner y position of subCube
    \param x_size - size in x direction in pixels
    \param y_size - size in y direction in pixels
  */
  void dalFITS::readSubCube(double *subCube, unsigned long x_pos, unsigned long y_pos, unsigned long x_size, unsigned long y_size)
  {
    long fpixel[3];	// first pixel definition
    long lpixel[3];	// last pixel definition

    fpixel[0]=1;	

    if(readHDUType()!=IMAGE_HDU)    // Check if current HDU is an image extension 
    {
      throw "dalFITS::readLine CHDU is not an image";
    }

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
    \param nulval - pointer to data to be substituted for any 0 values encountered 
  */
  void dalFITS::writePlane(double *plane, const long x, const long y, const long z, void *nulval)
  {
    long fpixel[3]; 	// first pixel position to read
    long nelements=0;	// number of elements to write
  
    // Check if Faraday plane has the same x-/y-dimensions as naxes dimensions of FITS
    if(x!=dimensions[0] || y!=dimensions[1])
    {
      throw "dalFITS::writePlane dimensions do not match";  
    }
 
    // check if plane counter is above limit
    if(z > dimensions[2])
    {
      throw "dalFITS::writePlane z out of range";
    }
    
    if(readHDUType()!=IMAGE_HDU)	// Check if current HDU is an image extension
    {
      throw "dalFITS::writePlane CHDU is not an image";
    }
 
    // Read from FITS file one plane
    fpixel[0]=1;
    fpixel[1]=1;
    fpixel[2]=z;  
 
    nelements=dimensions[0]*dimensions[1];	// compute nelements in plane
 
      // Write to FITS file
    if(plane==NULL)
    {
      throw "dalFITS::writePlane NULL pointer";
    }
    else if(nulval==NULL)
    {
      writePix(TDOUBLE, fpixel, nelements, plane);
    } 
    else	// write pixels with substituting null value
    { 
      writePixNull(TDOUBLE, fpixel, nelements, plane, nulval); 
    }
  }


  /*!
    \brief Write a line of sight to a FITS file

    \param faradayLine - contains line of sight along Faraday Depths with RM intensity
    \param x - x position in pixels to write line to
    \param y - y position in pixels to write line to
  */
  void dalFITS::writeLine(double *line, const long x, const long y, void *nulval)
  {
    
  
  }


  /*! 
    \brief Write a tile to a FITS file
  
    \param tile - buffer containing tile data
    \param x_pos - x position in pixels to write tile to
    \param y_pos - y position in pixels to write tile to
    \param x_size - horizontal size of tile
    \param y_size - vertical size of tile
  */
  void dalFITS::writeTile(double* tile,
			  const long x_pos,
			  const long y_pos,
			  const long x_size,
			  const long y_size
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
  void dalFITS::writeSubCube( double* subcube,
			      long x_size,
			      long y_size,
			      long x_pos,
			      long y_pos)
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


  /*!
    \brief Get size of Header space of current HDU
    
    \param &keysexist - number of existing keywords (without END)
    \param &morekeys - morekeys=-1 if the header has not yet been closed
  */
  void dalFITS::getHDRspace(int &keysexist,
			   int &morekeys)
  {
    if(fits_get_hdrspace(fptr, &keysexist, &morekeys, &fitsstatus))
    {
      throw "dalFITS::getHDRspace";
    }
  }


  /*!
    \brief Get a record from the current HDU
    
    \param keynum - nth key to read, first keyword in the header is at keynum=1
    \param record - write the entire 80-character header record into this string
  */
  void dalFITS::readRecord(int keynum,
			  std::string record)
  {
    if(fits_read_record(fptr, keynum, const_cast<char *>(record.c_str()), &fitsstatus))
    {
      throw "dalFITS::readRecord";
    }
  }

  /*!
    \brief Get the record card from the current HDU
    
    \param keyname - name of key to read
    \param record - write the entire 80-character header record into this string
  */
  void dalFITS::readCard(std::string keyname, std::string record)
  {
    if(fits_read_card(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(record.c_str()), &fitsstatus))
    {
      throw "dalFITS::readCard";
    }
  }


  /*!
    \brief Read the nth header record in the CHU

    \param keynum - nth key to read, first keyword in the header is at keynum=1
    \param keyname - string that will contain keyname
    \param value - string that will contain the key's value
    \param comment - string that will contain the corresponding comment
  */
  void dalFITS::readKeyn(int keynum, std::string keyname, std::string value, std::string comment)
  {
    if(fits_read_keyn(fptr, keynum, const_cast<char *>(keyname.c_str()), const_cast<char *>(value.c_str()), const_cast<char *>(comment.c_str()), &fitsstatus))
    {
      throw "dalFITS::readKeyn";
    }
  }
  

  /*!
    \brief Return the specified keyword
    
    \param datatype - data type of the keyword (TSTRING, TLOGICAL==int, TBYTE, TSHORT, TUSHORT, TINT, TUINT, TLONG, TULONG, TLONGLONG, TFLOAT, TDOUBLE, TCOMPLEX, TDBLCOMPLEX
    \param keyname -name of key to look for
    \param value - value of key to look for
    \param comment - comment of key to look for
  */
  void dalFITS::readKey(int datatype,
		       std::string keyname,
		       void *value,
		       std::string comment)
  {
    if(fits_read_key(fptr, datatype, const_cast<char *>(keyname.c_str()), value, const_cast<char *>(comment.c_str()), &fitsstatus))
    {
      throw "dalFITS::readKey";
    }
  }


  /*!
    \brief Find the next key in the header whose name matches one of the strings in inclist
    
    \param **inclist - array of character strings with names to be included in search
    \param ninc - number of included keys in search
    \param **exclist - array of character strings with names to be excluded in search
    \param nexc - number of excluded keys, may be set to 0 if there are no keys excluded
    \param card - string to write card to
  */
  void dalFITS::findNextKey(char **inclist,
			   int ninc,
			   char **exclist,
			   int nexc,
			   std::string card)
  {
    if(fits_find_nextkey(fptr, inclist, ninc, exclist, nexc, const_cast<char *>(card.c_str()), &fitsstatus))
    {
      throw "dalFITS::findNextKey";
    }
  }


  /*!
    \brief Read the Unit string of a key
  
    \param keyname - name of key
    \param unit - string to write the physical unit to
  */
  void dalFITS::readKeyUnit(std::string keyname,
			   std::string unit)
  {
    if(fits_read_key_unit(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(unit.c_str()), &fitsstatus))
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
    \brief Write a key to the header
  
    \param datatype - data type of key
    \param keyname - name of key
    \param *value - value to write into field
    \param comment - comment string
  */
  void dalFITS::writeKey(int datatype,
			std::string keyname,
			void *value,
			std::string comment)
  {
    if(fits_write_key(fptr, datatype, const_cast<char 
				*>(keyname.c_str()), value, const_cast<char *>(comment.c_str()), &fitsstatus))
      {
	throw "dalFITS::writeKey";
      }
  }


  /*!
    \brief Update a key in the header
  
    \param datatype - data type of key
    \param keyname - name of key
    \param value - value to write into field
    \param comment - comment string
  */
  void dalFITS::updateKey(int datatype,
			 std::string &keyname,
			 void *value,
			 std::string &comment)
  {
  
    if(fits_update_key(	fptr,
			datatype,
			const_cast<char *>(keyname.c_str()),
			value,
			const_cast<char *>(comment.c_str()),
			&fitsstatus))
      {
	throw "dalFITS::updateKey";
      }
  }


  /*!
    \brief Write a record "card" to the FITS header of the CHDU
    
    \param card - string containing card information
  */
  void dalFITS::writeRecord(std::string &card)
  {
    if(fits_write_record(fptr, const_cast<char *>(card.c_str()), &fitsstatus))
    {
      throw "dalFITS:writeRecord";
    }
  }
  

  /*!
    \brief Modify the comment of &keyname in the FITS header of the CHDU
    
    \param keyname - name of key to modify key of
    \param comment - new comment string
  */
  void dalFITS::modifyComment(std::string &keyname, std::string &comment)
  {
    if(fits_modify_comment(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(comment.c_str()), &fitsstatus))
    {
      throw "dalFITS::modifyComment";
    }
  }


  /*!
    \brief Write a Unit to the key corresponding to &keyname in the CHDU
  
    \param keyname - name of key to modify unit of
    \param unit - string with physical unit
  */
  void dalFITS::writeKeyUnit(std::string &keyname, std::string &unit)
  {
    if(fits_write_key_unit(fptr, const_cast<char *>(keyname.c_str()), const_cast<char *>(unit.c_str()), &fitsstatus))
    {
      throw "dalFITS::writeKeyUnit";
    }
  }

 
  /*!
    \brief Write a comment to the CHDU
  
    \param &comment - write a comment to the CHU
  */
  void dalFITS::writeComment(std::string &comment)
  {
    if(fits_write_comment(fptr, const_cast<char *>(comment.c_str()),  &fitsstatus))
    {
      throw "dalFITS::writeComment";
    }
  }


  /*!
    \brief Write a HISTORY entry to the CHDU
      
    \param history - History text
  */
  void dalFITS::writeHistory(std::string &history)
  {
    if(fits_write_history(fptr, const_cast<char *>(history.c_str()),  &fitsstatus))
    {
      throw "dalFITS::writeHistory";
    }
  }


  /*!
    \brief Write the current date to the CHDU
  */
  void dalFITS::writeDate()
  {
    if(fits_write_date(fptr,  &fitsstatus))
    {
      throw "dalFITS::writeDate";
    }
  }


  /*!
    \brief Delete a record from the CHDU
    
    \param keynum - number of key to delete
  */
  void dalFITS::deleteRecord(int keynum)
  {
    if(fits_delete_record(fptr, keynum,  &fitsstatus))
    {
      throw "dalFITS::deleteRecord";
    }
  }


  /*!
    \brief Delete the key referenced by &keyname from the CHDU 
    
    \param keyname - name of key to delete
  */
  void dalFITS::deleteKey(std::string keyname)
  {
    if(fits_delete_key(fptr, const_cast<char *>(keyname.c_str()),  &fitsstatus))
    {
      throw "dalFITS::deleteKey";
    }
  }


  /*!
    \brief Copy the Header of this FITS to the &other dalFITS object
      
    \param &other - other dalFITS object to copy header from
  */
  void dalFITS::copyHeader(dalFITS &other)
  {
    if(fits_copy_header(fptr, other.fptr, &fitsstatus))
    {
      throw "dalFITS::copyHeader";
    }
  }


  /*!
    \brief Delete the CHDU

    \param *hdutype - Stores the HDUType of the deleted HDU (can be NULL if not needed)
  */
  void dalFITS::deleteHDU(int *hdutype=NULL)
  {
    if(fits_delete_hdu(fptr, hdutype ,&fitsstatus))
    {
      throw "dalFITS::deleteHDU";
    }
  }
  

  /*!
    \brief Write the Header checksum to the CHDU
  */
  void dalFITS::writeChecksum()
  {
    if(fits_write_chksum(fptr, &fitsstatus))
    {
      throw "dalFITS::writeChecksum";
    }
  }


  /*!
    \brief Verify checksum of the CHDU
      
    \param dataok - bool indicating that data part of hdu is ok
    \param hduok - bool indicating that hdu is ok
  */
  void dalFITS::verifyChecksum(bool &dataok, bool &hduok)
  {
    int dataok_int=0, hduok_int=0;
  
    if(fits_verify_chksum(fptr, &dataok_int, &hduok_int, &fitsstatus))
    {
      throw "dalFITS::verifyChecksum";
    }

    // convert int dataok and int hduk to bools
    if(dataok_int)
      dataok=true;
    if(hduok_int)
      hduok=true;
  }


  /*!
      \brief Parse a record card into value and comment
      
      \param card - card to be parsed
      \param &value - string to hold content of value
      \param &comment - string to hold content of comment
    */
  void dalFITS::parseValue(std::string &card, std::string &value, std::string &comment)
  {
    if(fits_parse_value(const_cast<char *>(card.c_str()), const_cast<char *>(value.c_str()), const_cast<char *>(comment.c_str()), &fitsstatus))
    {
      throw "dalFITS::parseValue";
    }
  }


  /*!
    \brief Get the type of a key with &value in the CHDU
    
    Parses the input 80-character header keyword value string to determine its datatype. It returns with a value of 'C', 'L', 'I', 'F' or 'X', for character string, logical integer, floating point, or complex, respectively.
    
    \param value - value of key to look for
    \param dtype - data type of that key/value
  */
  char dalFITS::getKeytype(std::string &value)
  {   
    char dtype;
  
    if(fits_get_keytype(const_cast<char *>(value.c_str()), &dtype, &fitsstatus))
    {
      throw "dalFITS::getKeytype";
    }
    
    return dtype;
  }


  /*!
    \brief Get the class of the key in &card
    
    \param card - Card to determine class of
  */
  int dalFITS::getKeyclass(std::string &card)
  {
    int keyclass=0;
  
    if(fits_get_keyclass(const_cast<char *>(card.c_str())))
    {
      throw "dalFITS::getKeyclass";
    }
    
    return keyclass;
  }


  /*!
    \brief Parse the &templatec of &card
    
    \param templatec - Template card
    \param card -
    
    \return keytype - determines if the keyword is a COMMENT or not
  */
  int dalFITS::parseTemplate(std::string &templatec, std::string &card)
  {
    int keytype=0;
  
    if(fits_parse_template(const_cast<char *>(templatec.c_str()), const_cast<char *>(card.c_str()), &keytype, &fitsstatus))
    {
      throw "dalFITS::parseTemplate";
    }
    
    return keytype;
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
   
    // Copy dalFITS image header

    // Write individual header keywords to FITS
    
    // faraday low
    // faraday high
    // algorithm used to create RM cube
  }


  /*!
    \brief Check if a file exists
    
    \param filename - Name of the file to check for its existence
  */
  bool dalFITS::fileExists(const std::string &filename) 
  {
    struct stat stFileInfo;
    bool blnReturn;
    int intStat;

    // Attempt to get the file attributes
    intStat = stat(filename.c_str(),&stFileInfo);
    if(intStat == 0) {
      // We were able to get the file attributes
      // so the file obviously exists.
      blnReturn = true;
    } else {
      // We were not able to get the file attributes.
      // This may mean that we don't have permission to
      // access the folder which contains this file. If you
      // need to do that level of checking, lookup the
      // return values of stat which will give you
      // more details on why stat failed.
      blnReturn = false;
    }
    
    return(blnReturn);
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
