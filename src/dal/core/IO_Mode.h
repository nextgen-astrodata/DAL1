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

#ifndef IO_MODE_H
#define IO_MODE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <dal_config.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class IO_Mode
    
    \ingroup DAL
    \ingroup core
    
    \brief Book-keeping of access properties
    
    \author Lars B&auml;hren

    \date 2010/10/19

    \test tIO_Mode.cc
    
    <h3>Prerequisite</h3>

    Given the intention of making the DAL an abstraction layer on top of a
    number of lower-level access libraries, one of the issues need to be
    addressed is the question of access flags/modes.

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
      //____________________________________________________________________
      // H5Fpublic.h

      #define H5F_ACC_RDONLY  (H5CHECK 0x0000u)     //  Absence of rdwr => rd-only
      #define H5F_ACC_RDWR    (H5CHECK 0x0001u)     //  Open for read and write
      #define H5F_ACC_TRUNC   (H5CHECK 0x0002u)     //  Overwrite existing files
      #define H5F_ACC_EXCL    (H5CHECK 0x0004u)     //  Fail if file already exists
      #define H5F_ACC_DEBUG   (H5CHECK 0x0008u)     //  Print debug info
      #define H5F_ACC_CREAT   (H5CHECK 0x0010u)     //  Create non-existing files
      \endcode
      Four access modes address these concerns, with \c H5Fcreate and \c H5Fopen
      each accepting two of them:
      <ul>
        <li>\c H5Fcreate is the primary function for creating HDF5 files; it
	creates a new HDF5 file with the specified name and property lists and
	specifies whether an existing file of same name should be overwritten.

	The name parameter specifies the name of the new file.

	The flags parameter specifies whether an existing file is to be
	overwritten. It should be set to either \c H5F_ACC_TRUNC to overwrite an
	existing file or \c H5F_ACC_EXCL, instructing the function to fail if the
	file already exists.

	New files are always created in read-write mode, so the read-write and
	read-only flags, \c H5F_ACC_RDWR and \c H5F_ACC_RDONLY, respectively,
	are not relevant in this function. Further note that a specification of
	\c H5F_ACC_RDONLY will be ignored; the file will be created in read-write
	mode, regardless.

	<li>\c H5Fopen is the primary function for accessing existing HDF5
	files. This function opens the named file in the specified access mode
	and with the specified access property list.

	Note that \c H5Fopen does not create a file if it does not already exist.
	The \e name parameter specifies the name of the file to be opened.

	The flags parameter specifies whether the file will be opened in
	read-write or read-only mode, \c H5F_ACC_RDWR or \c H5F_ACC_RDONLY,
	respectively. More complex behaviors of file access are controlled
	through the file-access property list.
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

      Mirlib build on top of the GNU C Library file access flags:

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

      A (possible minor) variation on the above can be found on Darwin systems:
      \code
      #define O_CREAT         0x0200          // create if nonexistant
      #define O_TRUNC         0x0400          // truncate to zero length
      #define O_EXCL          0x0800          // error if already exists
      \endcode
      
    <li>\b Qt
      
      \code
      // File I/O ---------------------------------------------------------
      #define _O_RDONLY       0x0001
      #define _O_RDWR         0x0002
      #define _O_WRONLY       0x0004
      #define _O_CREAT        0x0008
      #define _O_TRUNC        0x0010
      #define _O_APPEND       0x0020
      #define _O_EXCL         0x0040
      \endcode
      
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
	  requires <tt>FileIOPermissionAccess::Write</tt>. If the file already exists, an
	  IOException is thrown.</td>
	</tr>
        <tr valign=top>
	  <td>\c Create</td>
	  <td>Specifies that the operating system should create a new file. If
	  the file already exists, it will be overwritten. This requires
	  <tt>FileIOPermissionAccess::Write</tt>. <tt>System.IO.FileMode.Create</tt>
	  is equivalent to requesting that if the file does not exist, use
	  \c CreateNew; otherwise, use \c Truncate. If the file already exists
	  but is a hidden file, an \c UnauthorizedAccessException is thrown.</td>
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
	  opened with <tt>FileAccess.Read</tt>, <tt>FileIOPermissionAccess::Read</tt> is required.
	  If the file access is <tt>FileAccess.Write</tt> then <tt>FileIOPermissionAccess::Write</tt>
	  is required. If the file is opened with <tt>FileAccess.ReadWrite</tt>, both
	  <tt>FileIOPermissionAccess::Read</tt> and <tt>FileIOPermissionAccess::Write</tt> are
	  required. If the file access is <tt>FileAccess.Append</tt>, then
	  <tt>FileIOPermissionAccess::Append</tt> is required.</td>
	</tr>
        <tr valign=top>
	  <td>\c Truncate</td>
	  <td>Specifies that the operating system should open an existing file.
	  Once opened, the file should be truncated so that its size is zero
	  bytes. This requires <tt>FileIOPermissionAccess::Write</tt>. Attempts to read
	  from a file opened with Truncate cause an exception.</td>
	</tr>
        <tr valign=top>
	  <td>\c Append</td>
	  <td>Opens the file if it exists and seeks to the end of the file, or
	  creates a new file. <tt>FileMode.Append</tt> can only be used in conjunction
	  with <tt>FileAccess.Write</tt>. Attempting to seek to a position before the
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

    <h3>Synopsis</h3>

    \image html DAL_IO_Mode.png

    <h3>Example(s)</h3>
    
  */  
  class IO_Mode {

  public:

    /*!
      \brief Enumeration with file modes
      
      <center>
      <table>
        <tr>
          <td class="indexkey">DAL</td>
	  <td class="indexkey">GNU</td>
	  <td class="indexkey">Qt</td>
	  <td class="indexkey">HDF5</td>
	  <td class="indexkey">C#</td>
	</tr>
        <tr>
	  <td>DAL::IO::Create</td>
	  <td>O_CREAT | O_TRUNC</td>
	  <td></td>
	  <td>H5Fcreate(...,ACC_TRUNC)</td>
	  <td>Create</td>
	</tr>
        <tr>
	  <td>DAL::IO::CreateNew</td>
	  <td>O_CREAT | O_EXCL</td>
	  <td></td>
	  <td>H5Fcreate(...,ACC_CREAT)</td>
	  <td>CreateNew</td>
	</tr>
        <tr>
	  <td>DAL::IO::Open</td>
	  <td></td>
	  <td></td>
	  <td></td>
	  <td>Open</td>
	</tr>
        <tr>
	  <td>DAL::IO::OpenOrCreate</td>
	  <td></td>
	  <td></td>
	  <td></td>
	  <td>OpenOrCreate</td>
	</tr>
        <tr>
	  <td>DAL::IO::ReadOnly</td>
	  <td>O_RDONLY</td>
	  <td>_O_RDONLY</td>
	  <td>ACC_RDONLY</td>
	  <td>Read</td>
	</tr>
        <tr>
	  <td>DAL::IO::WriteOnly</td>
	  <td>O_WRONLY</td>
	  <td>_O_WRONLY</td>
	  <td>---</td>
	  <td>Write</td>
	</tr>
        <tr>
	  <td>DAL::IO::ReadWrite</td>
	  <td>O_RDWR</td>
	  <td>_O_RDWR</td>
	  <td>ACC_RDWR</td>
	  <td>ReadWrite</td>
	</tr>
      </table>
      </center>
    */
    enum Flags {
      //! Opens an existing object.
      Open         = 1,
      //! Opens a new object. If there is no object, it creates a new object. 
      OpenOrCreate = 2,
      //! Creates a new object. Overwrites any existing object.
      Create       = 4,
      //! Creates a new object. If the object already exists, an exception is thrown.
      CreateNew    = 8,
      //! Truncates an existing file.
      Truncate     = 16,
      //! Read access to the object.
      ReadOnly     = 32,
      //! Write access to the object.
      WriteOnly    = 64,
      //! Read and write access to the object.
      ReadWrite    = 128
    };

  private:

    //! Object I/O mode flags
    int itsFlags;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    IO_Mode ();
    
    //! Argumented constructor
    IO_Mode (IO_Mode::Flags const &flag);
    
    //! Argumented constructor
    IO_Mode (int const &flags);
    
    //! Copy constructor
    IO_Mode (IO_Mode const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~IO_Mode ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another IO_Mode object from which to make a copy.
    */
    IO_Mode& operator= (IO_Mode const &other); 
    
    // === Parameter access =====================================================
    
    //! Get object I/O mode flags
    inline int flags () const {
      return itsFlags;
    }

    /*!
      \brief Set object I/O mode flag
      \param mode    -- Object I/O mode flag
      \return status -- Status of the operation.
    */
    inline bool setFlag (IO_Mode::Flags const &flag) {
      itsFlags = flag;
      return verifyFlags(itsFlags,true);
    }

    /*!
      \brief Set object I/O mode flags
      \param mode    -- Object I/O mode flag
      \return status -- Status of the operation.
    */
    inline bool setFlags (int const &flags) {
      itsFlags = flags;
      return verifyFlags(itsFlags,true);
    }

    //! Add flag to the current seetings
    bool addFlag (IO_Mode::Flags const &flag);

    //! Remove flag from the current seetings
    bool removeFlag (IO_Mode::Flags const &flag);

    //! Reset the object I/O mode flags
    bool resetFlags ();

    // === Public methods =======================================================

    //! Get array containing the available flag names
    std::vector<std::string> names ();

    //! Check if a given \e flag is part of the I/O mode settings
    bool haveFlag (IO_Mode::Flags const &which);

#ifdef DAL_WITH_HDF5
    inline hid_t flagH5Fcreate () {
      return flagH5Fcreate (itsFlags);
    }
#endif

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, IO_Mode.
    */
    inline std::string className () const {
      return "IO_Mode";
    }

    // === Static methods =======================================================

    //! Get map containing the available flags
    static std::map<IO_Mode::Flags,std::string> flagsMap ();

    //! Get array containing the available flag values
    static std::vector<IO_Mode::Flags> flagsType ();

    //! Get array containing the available flag names
    static std::vector<std::string> flagsName ();

    //! Get array containing the flag names encoded in \c flags
    static std::vector<std::string> names (IO_Mode::Flags const &flags);
    
    //! Get array containing the flag names encoded in \c flags
    static std::vector<std::string> names (int const &flags);
    
    //! Verify the I/O mode flags
    static bool verifyFlags (int &flags,
			     bool const &correctFlags=false);
    
#ifdef DAL_WITH_HDF5
    static hid_t flagH5Fcreate (int const &flags);
#endif
    
  private:

    //! Initialize internal parameters
    inline void init () {
      itsFlags = IO_Mode::Open | IO_Mode::ReadOnly;
    }
    
    //! Unconditional copying
    void copy (IO_Mode const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class IO_Mode -- end
  
#ifdef DAL_WITH_HDF5
    //! Return flags of currently open HDF5 file
    bool h5get_flags(IO_Mode &flags, hid_t const &object_id);
#endif

} // Namespace DAL -- end

#endif /* IO_MODE_H */
  
