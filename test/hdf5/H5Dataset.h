
#include <iostream>
#include <string>
#include <vector>

#include <dalCommon.h>

namespace DAL {
  
  /*!
    \class H5Dataset
    \ingroup DAL
    \brief A class to encapsulate the operations required to work with a dataset
    \author Lars B&auml;hren
    \date 2009/12/04
  */
  class H5Dataset {
    
    //! Name of the dataset
    std::string name_p;
    //! Shape of the dataset
    std::vector<hsize_t> shape_p;
    //! Dataspace identifier
    hid_t dataspaceID_p;
    //! Datatype identifier
    hid_t datatypeID_p;
    //! Dataset identifier
    hid_t datasetID_p;
    
  public:
    
    // === Construction =========================================================

    //! Default constuctor
    H5Dataset ();
    
    //! Argumented constructor
    H5Dataset (hid_t const &location,
	       std::string const &name,
	       std::vector<hsize_t> const &shape,
	       hid_t const &datatype=H5T_NATIVE_DOUBLE);

    // === Destruction ==========================================================

    // Destructor
    ~H5Dataset ();

    // === Parameter access =====================================================

    //! Get the name of the dataset
    inline std::string name () const {
      return name_p;
    }

    //! Get the shape of the dataset
    inline std::vector<hsize_t> shape () const {
      return shape_p;
    }

    //! Get the rank (i.e. the number of axes) of the dataset
    inline int rank () const {
      return shape_p.size();
    }

    //! Get the dataspace identifier
    inline hid_t dataspaceID () const {
      return dataspaceID_p;
    }

    //! Get the datatype identifier
    inline hid_t datatypeID () const {
      return datatypeID_p;
    }

    // === Methods ==============================================================

    //! Open the dataset
    bool open (hid_t const &location,
	       std::string const &name,
	       std::vector<hsize_t> const &shape,
	       hid_t const &datatype=H5T_NATIVE_DOUBLE);
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

  private:
    
    //! Initialize the internal parameters
    void init ();
    
  }; // end class H5Dataset
  
} // end namespace DAL
