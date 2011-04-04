/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#ifndef ACCESSMODE_H
#define ACCESSMODE_H

#include <dal_config.h>
#include <core/HDF5Object.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class AccessMode
    
    \ingroup DAL
    \ingroup core
    
    \brief Book-keeping of access properties
    
    \author Lars B&auml;hren

    \date 2010/10/19

    \test tAccessMode.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>

    Available file access property flags:

    <ul>
      <li>\b HDF5 <br>
      These are the bits that can be passed to the `flags' argument of
      \c H5Fcreate() and \c H5Fopen(). Use the bit-wise OR operator (|) to combine
      them as needed.  As a side effect, they call H5check_version() to make sure
      that the application is compiled with a version of the hdf5 header files
      which are compatible with the library to which the application is linked.
      We're assuming that these constants are used rather early in the hdf5
      session.
      \code
      //____________________________________________________
      // H5Fpublic.h

      #define H5F_ACC_RDONLY  (H5CHECK 0x0000u)     //  absence of rdwr => rd-only
      #define H5F_ACC_RDWR    (H5CHECK 0x0001u)     //  open for read and write
      #define H5F_ACC_TRUNC   (H5CHECK 0x0002u)     //  overwrite existing files
      #define H5F_ACC_EXCL    (H5CHECK 0x0004u)     //  fail if file already exists
      #define H5F_ACC_DEBUG   (H5CHECK 0x0008u)     //  print debug info
      #define H5F_ACC_CREAT   (H5CHECK 0x0010u)     //  create non-existing files
      \endcode
      Four access modes address these concerns, with H5Fcreate and H5Fopen each
      accepting two of them:
      <ul>
        <li>\c H5Fcreate accepts \c H5F_ACC_TRUNC or \c H5F_ACC_EXCL. All newly
	created files are opened for both reading and writing. 
	<li>\c H5Fopen accepts \c H5F_ACC_RDONLY or \c H5F_ACC_RDWR.
      </ul>
      For more detailed information see chapter 3 ("The HDF5 File") of the
      <i>HDF5 User's Guide</i>.
      
      <li>\b CFITSIO <br>
      The iomode parameter defines the read/write access allowed in the file and can
      have values of:
      \code
      #define READONLY 0
      #define READWRITE 1
      \endcode

      <li>\b casacore

      <li>\b mirlib

      \code
      //____________________________________________________
      // dio.c
      
      if     (!strcmp(status,"read"))    flags = O_RDONLY;
      else if(!strcmp(status,"write"))   flags = O_CREAT|O_TRUNC|O_RDWR;
      else if(!strcmp(status,"append"))  flags = O_CREAT|O_RDWR;

      //____________________________________________________
      // hio.c
      
      #define ITEM_READ     0x1
      #define ITEM_WRITE    0x2
      #define ITEM_SCRATCH  0x4
      #define ITEM_APPEND   0x8
      #define ACCESS_MODE (ITEM_READ|ITEM_WRITE|ITEM_SCRATCH|ITEM_APPEND)
      
      #define RDWR_UNKNOWN 0
      #define RDWR_RDONLY  1
      #define RDWR_RDWR    
      \endcode
      
      <li><b>GNU C Library</b>

      The file access modes allow a file descriptor to be used for reading,
      writing, or both. (In the GNU system, they can also allow none of these,
      and allow execution of the file as a program.) The access modes are chosen
      when the file is opened, and never change.

      <table border=0 width=95%>
        <tr valign=top>
	 <td class="indexkey" width=20%>File Access Modes</td>
	 <td class="indexkey" width=75%>Description</td>
	</tr>
        <tr valign=top>
	  <td>int O_RDONLY</td>
	  <td>Open the file for read access.</td>
	</tr>
        <tr valign=top>
	  <td>int O_WRONLY</td>
	  <td>Open the file for write access.</td>
	</tr>
        <tr valign=top>
	  <td>int O_RDWR</td>
	  <td>Open the file for both reading and writing.</td>
	</tr>
        <tr valign=top>
	 <td class="indexkey" width=20%>Open-time Flags</td>
	 <td class="indexkey" width=75%>Description</td>
	</tr>
        <tr valign=top>
	  <td>int O_CREAT</td>
	  <td>If set, the file will be created if it doesn't already exist. </td>
	</tr>
        <tr valign=top>
	  <td>int O_EXCL</td>
	  <td>If both \c O_CREAT and \c O_EXCL are set, then open fails if the
	  specified file already exists. This is guaranteed to never clobber an
	  existing file. </td>
	</tr>
        <tr valign=top>
	  <td>int O_TRUNC</td>
	  <td>Truncate the file to zero length. This option is only useful for
	  regular files, not special files such as directories or FIFOs. POSIX.1
	  requires that you open the file for writing to use O_TRUNC. In BSD and
	  GNU you must have permission to write the file to truncate it, but you
	  need not open for write access. </td>
	</tr>
      </table>

      In the GNU system (and not in other systems), \c O_RDONLY and
      \c O_WRONLY are independent bits that can be bitwise-ORed together, and
      it is valid for either bit to be set or clear. This means that \c O_RDWR
      is the same as \c O_RDONLY|\c O_WRONLY. A file access mode of zero is
      permissible; it allows no operations that do input or output to the file,
      but does allow other operations such as fchmod. On the GNU system, since
      “read-only” or “write-only” is a misnomer, \c fcntl.h defines additional
      names for the file access modes. These names are preferred when writing
      GNU-specific code. But most programs will want to be portable to other
      POSIX.1 systems and should use the POSIX.1 names above instead. 

      <li><b>.NET Framework</b> (System.IO)

      <a href="http://msdn.microsoft.com/en-us/library/system.io.filemode.aspx">FileMode
      parameters</a> control whether a file is overwritten, created, or
      opened, or some combination thereof. Use Open to open an existing file.
      To append to a file, use Append. To truncate a file or to create it if it
      does not exist, use Create.

      A <a href="http://msdn.microsoft.com/en-us/library/4z36sx0f.aspx">FileAccess
      parameter</a> is specified in many of the constructors for File,
      FileInfo, FileStream, and other constructors where it is important to
      control the kind of access users have to a file. Defines constants for
      read, write, or read/write access to a file.

      <table border=0 width=95%>
        <tr valign=top>
	 <td class="indexkey" width=20%>FileMode parameter</td>
	 <td class="indexkey" width=75%>Description</td>
	</tr>
        <tr valign=top>
	  <td>\c CreateNew</td>
	  <td>Specifies that the operating system should create a new file. This
	  requires FileIOPermissionAccess::Write. If the file already exists, an
	  IOException is thrown.</td>
	</tr>
        <tr valign=top>
	  <td>\c Create</td>
	  <td>Specifies that the operating system should create a new file. If
	  the file already exists, it will be overwritten. This requires
	  FileIOPermissionAccess::Write. System.IO.FileMode.Create is equivalent
	  to requesting that if the file does not exist, use CreateNew; otherwise,
	  use Truncate. If the file already exists but is a hidden file, an
	  UnauthorizedAccessException is thrown.</td>
	</tr>
        <tr valign=top>
	  <td>\c Open</td>
	  <td>Specifies that the operating system should open an existing file.
	  The ability to open the file is dependent on the value specified by
	  FileAccess. A System.IO::FileNotFoundException is thrown if the file
	  does not exist.</td>
	</tr>
        <tr valign=top>
	  <td>\c OpenOrCreate</td>
	  <td>Specifies that the operating system should open a file if it
	  exists; otherwise, a new file should be created. If the file is
	  opened with FileAccess.Read, FileIOPermissionAccess::Read is required.
	  If the file access is FileAccess.Write then FileIOPermissionAccess::Write
	  is required. If the file is opened with FileAccess.ReadWrite, both
	  FileIOPermissionAccess::Read and FileIOPermissionAccess::Write are
	  required. If the file access is FileAccess.Append, then
	  FileIOPermissionAccess::Append is required.</td>
	</tr>
        <tr valign=top>
	  <td>\c Truncate</td>
	  <td>Specifies that the operating system should open an existing file.
	  Once opened, the file should be truncated so that its size is zero
	  bytes. This requires FileIOPermissionAccess::Write. Attempts to read
	  from a file opened with Truncate cause an exception.</td>
	</tr>
        <tr valign=top>
	  <td>\c Append</td>
	  <td>Opens the file if it exists and seeks to the end of the file, or
	  creates a new file. FileMode.Append can only be used in conjunction
	  with FileAccess.Write. Attempting to seek to a position before the
	  end of the file will throw an IOException and any attempt to read
	  fails and throws an NotSupportedException.</td>
	</tr>
        <tr valign=top>
	 <td class="indexkey" width=20%>FileAccess parameter</td>
	 <td class="indexkey" width=75%>Description</td>
	</tr>
        <tr valign=top>
	  <td>\c Read</td>
	  <td>Read access to the file. Data can be read from the file. Combine
	  with \c Write for read/write access.</td>
	</tr>
        <tr valign=top>
	  <td>\c Write</td>
	  <td>Write access to the file. Data can be written to the file. Combine
	  with \c Read for read/write access.</td>
	</tr>
        <tr valign=top>
	  <td>\c ReadWrite</td>
	  <td>Read and write access to the file. Data can be written to and read
	  from the file.</td>
	</tr>
      </table>

      <li>\b C#

      <table border=0 width=95%>
        <tr valign=top>
	 <td class="indexkey" width=20%>FileMode parameter</td>
	 <td class="indexkey" width=75%>Description</td>
	</tr>
        <tr valign=top>
	  <td>\c Append</td>
	  <td>Opens the file and adds data. This should be used with the
	  FileAccess Write Enumeration value.</td>
	</tr>
        <tr valign=top>
	  <td>\c Create</td>
	  <td>Creates a new file. Overwrites any existing file.</td>
	</tr>
        <tr valign=top>
	  <td>\c CreateNew</td>
	  <td>Creates a new file. If the file already exists, IOException is
	  thrown.</td>
	</tr>
        <tr valign=top>
	  <td>\c Open</td>
	  <td>Opens an existing file</td>
	</tr>
        <tr valign=top>
	  <td>\c OpenOrCreate</td>
	  <td>Opens a new file. If there is no file, it creates a new file.</td>
	</tr>
        <tr valign=top>
	  <td>\c Truncate</td>
	  <td>Truncates an existing file</td>
	</tr>
        <tr valign=top>
	 <td class="indexkey" width=15%>FileAccess parameter</td>
	 <td class="indexkey" width=80%>Description</td>
	</tr>
        <tr valign=top>
	  <td>\c Read</td>
	  <td>Data can be read (retrieved) from the file</td>
	</tr>
        <tr valign=top>
	  <td>\c ReadWrite</td>
	  <td>Data can be added to and retrieved from the file</td>
	</tr>
        <tr valign=top>
	  <td>\c Write</td>
	  <td>Data can be added to the file</td>
	</tr>
      </table>

    </ul>
    
    <h3>Example(s)</h3>
    
  */  
  class AccessMode {
    
  public:
    
    //! Object mode parameter
    enum Mode {
      //! Creates a new object. Overwrites any existing object.
      Create,
      //! Creates a new object. If the object already exists, an exception is thrown.
      CreateNew,
      //! Opens an existing object.
      Open,
      //! Opens a new object. If there is no object, it creates a new object. 
      OpenOrCreate
    };
    
    //! Object access parameter
    enum Access {
      //! Read access to the object
      Read,
      //! Read and write access to the object
      ReadWrite,
      //! Write access to the object
      Write
    };

    // === Construction =========================================================
    
    //! Default constructor
    AccessMode ();
    
    //! Copy constructor
    AccessMode (AccessMode const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~AccessMode ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another AccessMode object from which to make a copy.
    */
    AccessMode& operator= (AccessMode const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, AccessMode.
    */
    inline std::string className () const {
      return "AccessMode";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    
    
  private:
    
    AccessMode::Mode itsMode;
    AccessMode::Access itsAccess;

    //! Unconditional copying
    void copy (AccessMode const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class AccessMode -- end
  
} // Namespace DAL -- end

#endif /* ACCESSMODE_H */
  
