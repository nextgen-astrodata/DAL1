
/*!
  \file pil.h

  \ingroup DAL
  \ingroup data_common

  \brief C/C++ API functions of the Parameter Interface Library (PIL)

  <ul>
    <li><a href="#IRAF parameter files">IRAF parameter files</a>
    <li><a href="#Parameter files">Naming and location of parameter files</a>
    <li><a href="#PIL include files">PIL C/C++ include files</a>
    <li><a href="#PIL API functions">C/C++ API functions of the PIL library</a>
  </ul>

  <h3>Abstract</h3>
  
  The Parameter Interface Library (PIL) is an ANSI C/C++ and Fortran 90 callable 
  library which manages access to IRAF/XPI compatible parameter files.
  In general each program (executable) written for ISDC will have its
  parameter file. PIL gives standard set of functions (API)
  which can be called by applications wanting to access parameter files.
  PIL functions allow for : reading/writing individual parameters from
  parameter file, automatic selection of server mode, (re)opening/closing of
  parameter file, querying information about parameter file.
  C/C++ bindings have (almost) one to one corresponding Fortran bindings.
  Minor differences are result of different calling conventions in C/C++ and
  Fortran 90.

  <a name="IRAF parameter files"><h3>IRAF parameter files</h3></a>

  The purpose of PIL library is to enable ISDC applications access IRAF
  compatible parameter files. IRAF parameter file is a text file.
  Every line describes single parameter. Format is as follows : 

  \verbatim
  name,type,mode,default,min,max,prompt 
  \endverbatim  

  <ul>
    <li>\b name : name of parameter 
    <li>\b type : type of parameter. Allowable values: 
    <ul>
      <li>\b b : means parameter of boolean type
      <li>\b i : means parameter of integer type
      <li>\b r : means parameter of real type
      <li>\b s : means parameter of string type
      <li>\b f : for parameter of filename type. \b f may be followed
      by any combination of r/w/e/n meaning test for read access, write
      access, presence of file, absence of file. Thus <tt>fw</tt> means test whether
      file given as a value of parameter is writable. 
    </ul>
    <li>\b mode : mode of parameter. Allowable value is any reasonable
    (it is: a/h/q/hl/ql) combination of :
    <ul>
      <li>\b a/auto : effective mode equals to the \e value of parameter named
      <tt>mode</tt> in parameter file. If that parameter is not found in parameter
      file or is found invalid then the effective mode is 'hidden'.
      <li>\b h/hidden : No questions are asked, unless default value is invalid
      for given data type. for example "qwerty" is specified as a value
      for integer parameter. Note that is PILOverrideQueryMode function
      was called with argument set to <tt>PIL_QUERY_OVERRIDE</tt> then no questions
      are asked even for parameters with invalid values.
      <li>\b l/learn : If application changed parameter value, new value will be
      written to parameter file when application terminates.
      Actually this takes places when application calls either <tt>PILClose()</tt>
      or <tt>PILFlushParameters()</tt> (or <tt>CommonExit()</tt> when writing
      ISDC's CTS compliant software).
      If this flag is not specified then any changes to the parameter 
      (via PILGetXXX or PILPutXXX) are lost and are not written to disk.
      <li>\b q/query : Always ask for parameter. The format of the prompt is :
      prompt field from parameter file (parameter name if prompt fielf
      is empty) followed by allowable range (if any) in <>, followed
      by default value (if any) in [], followed by colon. Pressing RETURN
      key alone accepts default value. If newly entered value (or default
      value in the case RETURN key alone is pressed) is unacceptable library
      prompts user to reenter value. If the value is overridden by command
      line argument, then the PIL library does not prompt for that parameter
      (if value is valid and within boudaries if any) 
      Note that is PILOverrideQueryMode function
      was called with argument set to <tt>PIL_QUERY_OVERRIDE</tt> then no questions
      are asked ever (even for parameters with invalid values).
    </ul>
    <li>\b default : default value for parameter [this field is optional]. This
     can be: yes/no/y/n for booleans, integer/real literals (ie. 123, -34567 for
     integers, 1.23, 1234, -45.3e-5 for reals) for integers/reals, and string
     literals for strings and filenames. String literals can be: abcdef, "abcdef",
     'abcdef'.
     <li>\b min : minimum value allowable for parameter [this field is optional].
     Range checking is enabled only if both <tt>min</tt> and <tt>max</tt> fields
     are nonempty. See also <tt>max</tt>.
     When <tt>max</tt> field is empty then <tt>min</tt> field can also contain pipe
     symbol (vertical bar)
     separated list of allowable values for given parameter. This works for
     integer, real, string and filename types. For instance :
     \verbatim
     OutFile,f,ql,"copy.o",/dev/null|copy.o|dest.o,,"Enter output file name"
     \endverbatim
     specifies that OutFile parameter can take only 3 distinct values, it is
     /dev/null, copy.o or dest.o. Furthermore, for string (and <tt>_ONLY_</tt>
     for string, this does <tt>_NOT_</tt> apply for filename parameter types)
     parameter types, case does not matter, and string returned by PILGetString
     is converted to uppercase. Note, that automatic conversion to uppercase
     is only done when enum list is specified for given parameter.
     <li>\b max : maximum value allowable for parameter [this field is optional].
     Works for integer, real string and filename data types. Both <tt>min</tt>
     and <tt>max</tt> must be specified for range checking to be active. Also
     <tt>min</tt> cannot be larger than <tt>max</tt>. In case of any format
     error in <tt>min</tt> or <tt>max</tt> PIL assumes that no range checking
     is requested. See also <tt>min</tt> for a description of enumerated value list.
     <li>\b prompt : short description of parameter to be displayed whenever
     library asks for value. If none is given library will display parameters name. 
  </ul>

  As an example :

  \verbatim
  pressure,r,ql,1013,,,"Enter atmospheric pressure in hPa" 
  \endverbatim
  describes parameter named pressure of type real, mode = query + learn, with
  default value = 1013, no range checking and prompt
  \verbatim
  "Enter atmospheric pressure in hPa" 
  \endverbatim

  Empty lines and lines beginning with '\#' are considered to be comment lines.

  <a name="Parameter files"><h3>Naming and location of parameter files</h3></a>

  Typically for every executable there is a corresponding parameter file. The
  name of parameter file is the name of executable file plus ".par" suffix.
  Thus executable :
  \verbatim
  isdc_copy
  \endverbatim
  will have parameter file named :
  \verbatim
  isdc_copy.par
  \endverbatim

  Parameter files are searched for in several locations: 
  <ul>
    <li><tt>PFILES</tt> enviromnent variable 

    This variable is used to specify where the parameters are looked for. The
    variable uses a ";" delimiter to
    separate 2 types of parameter directories: 
    \verbatim
    <path1>;<path2>
    \endverbatim
    where path 1 is one or more user/writeable parameter directories, and
    path 2 is one or more system/read-only parameter directories. When
    path 1 equals path 2 one can omit path 2 and semicolon. The PIL library
    allows multiple ":"-delimited directories in both portions of the
    PFILES variable. The default values from path 2 are used the first
    time task is run, or whenever the default values have been
    updated more recently than the user's copy of the parameters. The user's
    copy is created when a task terminates, and retains any learned
    changes to the parameters. 

    <li>Current working directory.

    It is equivalent of PFILES set to ".". 
  </ul>

  <a name="PIL include files"><h3>PIL C/C++ include files</h3></a>

  Applications calling PIL services from C/C++ source code should include
  <tt>pil.h</tt>. Alternatively they can include <tt>isdc.h</tt> which includes
  <tt>pil.h</tt> file. <tt>pil.h</tt> file in turn internally includes all other
  PIL relevant include files.

  <tt>pil.h</tt> file can be called from either C or C++ source code. It contains
  prototypes of all C/C++ API functions, definitions of constants,
  declarations of global variables and definitions of data structures.

  <a name="PIL API functions"><h3>C/C++ API functions of the PIL library</h3></a>

  This  section  describes  the  C language implementation of the
  Parameter Interface Library Application Programming Interfaces (PIL APIs)
  It also gives C/C++ languages specific information necessary to use those APIs.

  PIL library is standalone and can be compiled independently of other ISDC
  libraries.

  Unless stated otherwise all PIL API functions return status code of type int.
  This is either <tt>ISDC_OK</tt> which equals <tt>PIL_OK</tt> which equals 0,
  which means everything went perfectly or negative value (error code) meaning
  some error occured. List of error codes can be found in <tt>pil_error.h</tt>
  file. Functions given below are the "official" ones. Internally PIL library
  calls many more functions. 	
*/

//_______________________________________________________________________________
//                                                                        PILInit

/*!
  \brief Initialize the PIL library.
  
  \param argc -- number of command line arguments.
  \param argv -- array of pointers to command line arguments. argv[0] is
         typically name of the executable.

  \return status -- If success returns ISDC_OK. Error code otherwise.

  This function initializes PIL library. This function has to be called before
  any other PIL function (there are some exceptions to this rule). It does the
  following :

  Based on PILModuleName (or argv[0] if PILModuleName is empty) calculates
  name of parameter file. Usually name of parameter file equals argv[0] +
  ".par" suffix but this can be overriden by calling PILSetModuleName and/or
  PILSetModuleVersion before calling PILInit. After successful termination
  parameter file is opened and read in, and global variable PILRunMode is
  set to one of the following values : 
  \verbatim
  ISDC_SINGLE_MODE 
  ISDC_SERVER_MODE 
  \endverbatim
  
  ISDC_SERVER_MODE is set whenever there is parameter "ServerMode" and its
  value is "yes" or "y". In any other case PILRunMode is set
  to ISDC_SINGLE_MODE.

  \b Note: <i>Only one parameter file can be open at a time. Parameter file
  remains open until PILClose is called. When writing applications for ISDC
  one should not use PILInit directly. Instead, one should call CommonInit
  function (from Common library) which calls PILInit.
*/
int PILInit (int argc, char **argv); 

//_______________________________________________________________________________
//                                                                       PILClose

/*!
  \brief Close open files, and writes all learned parameters to the disk files.

  \param status -- PIL_OK/ISDC_OK. 0 means normal return, any other value means
         abnormal termination. In this case changes to parameters made during
	 runtime are NOT written to parameter files.

  \return status -- If success returns ISDC_OK. Error code otherwise.
  
  This function has to be called before application terminates. It closes open
  files, and writes all learned parameters to the disk files (only when status
  == ISDC_OK). Once this function is called one cannot call any other PIL
  functions. One can however call PILInit to reinitialize PIL library.

  Function also clears PILRunMode, PILModuleName and PILModuleVersion global
  variables. 

  \b Note: This function does not terminate process. It simply shuts down PIL
  library. When writing applications for ISDC one should not use PILClose
  directly. Instead, one should call CommonExit function (from Common library) 
  which calls PILClose.
 */
int PILClose (int status); 

//_______________________________________________________________________________
//                                                            PILReloadParameters

/*!
  \brief Reload parameters from parameter file.
  
  \return status -- If success returns ISDC_OK. Error code otherwise.
  
  This function reloads parameters from parameter file. It is called internally
  by PILInit. It should be called explicitly by applications running in
  ISDC_SERVER_MODE to rescan parameter file and reload parameters from it.
  Current parameter list in memory (including any modifications) is deleted.
  PIL library locks whole file for exclusive access when reading from parameter
  file. 

  \b Note: parameter file remains open until PILClose is called. Function
  internally DOES NOT close/reopen parameter file. Application should call
  PILFlushParameters before calling this function, otherwise all changes
  made to parameters so far are lost.
*/
int PILReloadParameters (void);

//_______________________________________________________________________________
//                                                             PILFlushParameters

/*!
  \brief Flush changes made to parameter list (in memory) to disk.
  
  \return status -- If success returns ISDC_OK. Error code otherwise.
  
  This function flushes changes made to parameter list (in memory) to disk.
  Current contents of parameter file is overwritten. PIL library locks whole
  file for exclusive access when writing to parameter file.

  \b Note: Parameter file remains open until PILClose is called.
*/
int PILFlushParameters (void);

//_______________________________________________________________________________
//                                                               PILSetModuleName

/*!
  \brief Set name of the module which uses PIL services.

  \param name -- New module name.

  \return status -- If success returns ISDC_OK. Error code otherwise.
  
  Sets name of the module which uses PIL services. Result is stored in global
  variable PILModuleName. Usually name of parameter file equals argv[0] +
  ".par" suffix but this can be overriden by calling PILSetModuleName and/or
  PILSetModuleVersion before calling PILInit.
*/
int PILSetModuleName (char *name);

//_______________________________________________________________________________
//                                                            PILSetModuleVersion

/*!
  \brief Set version of the module which uses PIL services.

  \param version -- New module version. If NULL pointer is passed version is
  set to "version unspecified" string

  \return status -- If success returns ISDC_OK. Error code otherwise.
  
  Sets version of the module which uses PIL services. Result is stored in
  global variable PILModuleVersion.
*/
int PILSetModuleVersion (char *version);

//_______________________________________________________________________________
//                                                              PILGetParFilename

/*!
  \brief Retrieve full path of used parameter file.

  \param fname -- Pointer to name/path of the parameter file.

  \return status -- If success returns ISDC_OK. Error code otherwise.

  This function retrieves full path of used parameter file. Absolute path is
  returned only when PFILES environment variable contains absolute paths. If
  parameter file is taken from current dir then only filename is returned.
  Pointer returned points to a statically allocated buffer, applications should
  copy data from it using strcpy.
*/
int PILGetParFilename (char **fname);
