
#include <iostream>
#include <set>
#include <string>
#include <hdf5.h>

// ==============================================================================
//
//  class H5Base
//
// ==============================================================================

/*!
  \class H5Base
  \ingroup DAL
  \author Lars B&auml;hren
*/
class H5Base {
  
protected:
  
  //! HDF5 identifier for the location
  hid_t location_p;
  //! Names of the attributes attached to the structure
  std::set<std::string> attributes_p;

public:

  //! Destructor
  virtual ~H5Base ();

  // === Parameter access ========================
  
  //! Get the HDF5 location identifier
  inline hid_t location () const {
    return location_p;
  }
  
  //! Get the attributes attached to the HDF5 object
  inline std::set<std::string> attributes () const {
    return attributes_p;
  }
  
  //! Set the attributes attached to the HDF5 object
  bool setAttributes (std::set<std::string> const &attr);

  // === Methods =================================

  //! Open HDF5 opbject
  virtual bool open (hid_t const &location,
		     std::string const &name,
		     bool const &create) = 0;
  
};

// ==============================================================================
//
//  class H5File
//
// ==============================================================================

/*!
  \class H5File
  \ingroup DAL
  \author Lars B&auml;hren
*/
class H5File : public H5Base {
  
  std::string filename_p;
  
 public:
  
  //! Argumented constructor
  H5File (std::string const &filename);
  
  //! Get the filename
  inline std::string filename () const {
    return filename_p;
  }
  
  //! Open HDF5 opbject
  bool open (hid_t const &location,
	     std::string const &name,
	     bool const &create);
  
};
