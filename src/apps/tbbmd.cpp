#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/detail/cmdline.hpp>

#include <dal_config.h>
#include <data_hl/TBB_Timeseries.h>

#include <measures/Measures/MPosition.h>

namespace bpo = boost::program_options;

bool get_antenna_positions(std::map<std::string, casa::MPosition> *positions, std::string filename)
{
  std::ifstream file;
  std::string name;
  double x, y, z;

  file.open(filename.c_str());

  if (file.is_open())
  {
    while (!file.eof())
    {
      // Read position from file
      file >> name;
      file >> x;
      file >> y;
      file >> z;

      // Add to output
      positions->insert(std::make_pair(name, casa::MPosition(casa::MVPosition(x, y, z), casa::MPosition::ITRF)));
    }
  }
  else
  {
    file.close();
    return false;
  }

  file.close();
  return true;
}

int main (int argc, char *argv[])
{
  std::vector<std::string> files;

  DAL::TBB_Timeseries *tbb;

  std::map<std::string, casa::MPosition> *positions;

  //! Directory with antenna positions files
  std::string metadataDir = "";
  bool s_metadataDir = false;
  //! Antenna set
  std::string antennaSet;
  bool s_antennaSet = false;
  //! File creation date, YYYY-MM-DDThh:mm:ss.s
  std::string itsFiledate;
  bool s_itsFiledate = false;
  //! Name of the telescope ("LOFAR")
  std::string itsTelescope;
  bool s_itsTelescope = false;
  //! Name(s) of the observer(s)
  std::string itsObserver;
  bool s_itsObserver = false;
  //! Unique identifier for the project
  std::string itsProjectID;
  bool s_itsProjectID = false;
  //! Name of the project
  std::string itsProjectTitle;
  bool s_itsProjectTitle = false;
  //! Name of the project's principal investigator
  std::string itsProjectPI;
  bool s_itsProjectPI = false;
  //! Name(s) of the project's co-PI(s)
  std::string itsProjectCoI;
  bool s_itsProjectCoI = false;
  //! Names/Email-addresses of the project's primary contact person(s)
  std::string itsProjectContact;
  bool s_itsProjectContact = false;
  //! Unique identifier for the observation
  std::string itsObservationID;
  bool s_itsObservationID = false;
  //! Start date of the observation (MJD)
  std::string itsStartMJD;
  bool s_itsStartMJD = false;
  //! Start date of the observation (TAI)
  std::string itsStartTAI;
  bool s_itsStartTAI = false;
  //! Start date of the observation (UTC)
  std::string itsStartUTC;
  bool s_itsStartUTC = false;
  //! End date of the observation (MJD)
  std::string itsEndMJD;
  bool s_itsEndMJD = false;
  //! End date of the observation (TAI)
  std::string itsEndTAI;
  bool s_itsEndTAI = false;
  //! End date of the observation (UTC)
  std::string itsEndUTC;
  bool s_itsEndUTC = false;
  //! List of stations used during the observation
  std::vector<std::string> itsStationsList;
  bool s_itsStationsList = false;
  //! Observation minimum frequency
  double itsFrequencyMin;
  bool s_itsFrequencyMin = false;
  //! Observation maximum frequency
  double itsFrequencyMax;
  bool s_itsFrequencyMax = false;
  //! Observation center frequency
  double itsFrequencyCenter;
  bool s_itsFrequencyCenter = false;
  //! Observation frequency physical units
  std::string itsFrequencyUnit;
  bool s_itsFrequencyUnit = false;
  //! Clock frequency (LOFAR: 200.0 or 160.0)
  double itsClockFrequency;
  bool s_itsClockFrequency = false;
  //! Clock frequency physical unit
  std::string itsClockFrequencyUnit;
  bool s_itsClockFrequencyUnit = false;
  //! Antenna set specification of observation
  std::string itsAntennaSet;
  bool s_itsAntennaSet = false;
  //! Filter selection
  std::string itsFilterSelection;
  bool s_itsFilterSelection = false;
  //! Single or list of observation targets/sources
  std::string itsTarget;
  bool s_itsTarget = false;
  //! Processing system name/version
  std::string itsSystemVersion;
  bool s_itsSystemVersion = false;
  //! Processing pipeline name
  std::string itsPipelineName;
  bool s_itsPipelineName = false;
  //! Processing pipeline version
  std::string itsPipelineVersion;
  bool s_itsPipelineVersion = false;
  //! Interface Control Document (ICD) number 
  std::string itsIcdNumber;
  bool s_itsIcdNumber = false;
  //! Interface Control Document (ICD) document version
  std::string itsIcdVersion;
  bool s_itsIcdVersion = false;
  //! Notes or comments
  std::string itsNotes;
  bool s_itsNotes = false;

  bpo::options_description desc ("[AddMetadata] Available command line options");

  desc.add_options ()
    ("help,H", "Show help messages")
    ("station,s", bpo::value< std::vector<std::string> >(), "")
    ("metadataDir", bpo::value<std::string>(), "Directory containing static metadata")
    ("antennaSet", bpo::value<std::string>(&antennaSet)->default_value("UNDEFINED"), "antennaSet")
    ("itsTelescope", bpo::value<std::string>(), "itsTelescope")
    ("itsObserver", bpo::value<std::string>(), "itsObserver")
    ("itsProjectID", bpo::value<std::string>(), "itsProjectID")
    ("itsProjectTitle", bpo::value<std::string>(), "itsProjectTitle")
    ("itsProjectPI", bpo::value<std::string>(), "")
    ("itsProjectCoI", bpo::value<std::string>(), "")
    ("itsProjectContact", bpo::value<std::string>(), "")
    ("itsObservationID", bpo::value<std::string>(), "")
    ("itsStartMJD", bpo::value<std::string>(), "")
    ("itsStartTAI", bpo::value<std::string>(), "")
    ("itsStartUTC", bpo::value<std::string>(), "")
    ("itsEndMJD", bpo::value<std::string>(), "")
    ("itsEndTAI", bpo::value<std::string>(), "")
    ("itsEndUTC", bpo::value<std::string>(), "")
    ("itsFrequencyMin", bpo::value<double>(), "")
    ("itsFrequencyMax", bpo::value<double>(), "")
    ("itsFrequencyCenter", bpo::value<double>(), "")
    ("itsFrequencyUnit", bpo::value<std::string>(), "")
    ("itsClockFrequency", bpo::value<double>(), "")
    ("itsClockFrequencyUnit", bpo::value<std::string>(), "")
    ("itsAntennaSet", bpo::value<std::string>(), "")
    ("itsFilterSelection", bpo::value<std::string>(), "")
    ("itsTarget", bpo::value<std::string>(), "")
    ("itsSystemVersion", bpo::value<std::string>(), "")
    ("itsPipelineName", bpo::value<std::string>(), "")
    ("itsPipelineVersion", bpo::value<std::string>(), "")
    ("itsIcdNumber", bpo::value<std::string>(), "")
    ("itsIcdVersion", bpo::value<std::string>(), "")
    ("itsNotes", bpo::value<std::string>(), "")
    ("input-file", bpo::value< vector<string> >(), "Input file")
    ;

  bpo::positional_options_description p;
  p.add("input-file", -1);

  bpo::variables_map vm;
  bpo::store(bpo::command_line_parser(argc, argv).
      options(desc).positional(p).run(), vm);
  bpo::notify(vm);

  if (vm.count("help") || argc == 1)
  {
    cout << "\n" << desc << endl;
    return 0;
  }

  if (vm.count("metadataDir"))
  {
    metadataDir = vm["metadataDir"].as<std::string>();
    s_metadataDir = true;
  }

  if (vm.count("antennaSet"))
  {
    antennaSet = vm["antennaSet"].as<std::string>();
    s_antennaSet = true;
  }

  if (vm.count("itsFiledate"))
  {
    itsFiledate = vm["itsFiledate"].as<std::string>();
  }

  if (vm.count("itsTelescope"))
  {
    itsTelescope = vm["itsTelescope"].as<std::string>();
    s_itsTelescope = true;
  }

  if (vm.count("itsObserver"))
  {
    itsObserver = vm["itsObserver"].as<std::string>();
    s_itsObserver = true;
  }

  if (vm.count("itsProjectID"))
  {
    itsProjectID = vm["itsProjectID"].as<std::string>();
    s_itsProjectID = true;
  }

  if (vm.count("itsProjectTitle"))
  {
    itsProjectTitle = vm["itsProjectTitle"].as<std::string>();
    s_itsProjectTitle = true;
  }

  if (vm.count("itsProjectPI"))
  {
    itsProjectPI = vm["itsProjectPI"].as<std::string>();
    s_itsProjectPI = true;
  }

  if (vm.count("itsProjectCoI"))
  {
    itsProjectCoI = vm["itsProjectCoI"].as<std::string>();
    s_itsProjectCoI = true;
  }
  
  if (vm.count("itsProjectContact"))
  {
    itsProjectContact = vm["itsProjectContact"].as<std::string>();
    s_itsProjectContact = true;
  }

  if (vm.count("itsObservationID"))
  {
    itsObservationID = vm["itsObservationID"].as<std::string>();
    s_itsObservationID = true;
  }

  if (vm.count("itsStartMJD"))
  {
    itsStartMJD = vm["itsStartMJD"].as<std::string>();
    s_itsStartMJD = true;
  }

  if (vm.count("itsStartTAI"))
  {
    itsStartTAI = vm["itsStartTAI"].as<std::string>();
    s_itsStartTAI = true;
  }

  if (vm.count("itsStartUTC"))
  {
    itsStartUTC = vm["itsStartUTC"].as<std::string>();
    s_itsStartUTC = true;
  }

  if (vm.count("itsEndMJD"))
  {
    itsEndMJD = vm["itsEndMJD"].as<std::string>();
    s_itsEndMJD = true;
  }

  if (vm.count("itsEndTAI"))
  {
    itsEndTAI = vm["itsEndTAI"].as<std::string>();
    s_itsEndTAI = true;
  }

  if (vm.count("itsEndUTC"))
  {
    itsEndUTC = vm["itsEndUTC"].as<std::string>();
    s_itsEndUTC = true;
  }

  if (vm.count("station"))
  {
    itsStationsList = vm["station"].as< std::vector<std::string> >();
    s_itsStationsList = true;
  }

  if (vm.count("itsFrequencyMin"))
  {
    itsFrequencyMin = vm["itsFrequencyMin"].as<double>();
    s_itsFrequencyMin = true;
  }

  if (vm.count("itsFrequencyMax"))
  {
    itsFrequencyMax = vm["itsFrequencyMax"].as<double>();
    s_itsFrequencyMax = true;
  }

  if (vm.count("itsFrequencyCenter"))
  {
    itsFrequencyCenter = vm["itsFrequencyCenter"].as<double>();
    s_itsFrequencyCenter = true;
  }

  if (vm.count("itsFrequencyUnit"))
  {
    itsFrequencyUnit = vm["itsFrequencyUnit"].as<std::string>();
    s_itsFrequencyUnit = true;
  }

  if (vm.count("itsClockFrequency"))
  {
    itsClockFrequency = vm["itsClockFrequency"].as<double>();
    s_itsClockFrequency = true;
  }

  if (vm.count("itsClockFrequencyUnit"))
  {
    itsClockFrequencyUnit = vm["itsClockFrequencyUnit"].as<std::string>();
    s_itsClockFrequencyUnit = true;
  }

  if (vm.count("itsAntennaSet"))
  {
    itsAntennaSet = vm["itsAntennaSet"].as<std::string>();
    s_itsAntennaSet = true;
  }

  if (vm.count("itsFilterSelection"))
  {
    itsFilterSelection = vm["itsFilterSelection"].as<std::string>();
    s_itsFilterSelection = true;
  }

  if (vm.count("itsTarget"))
  {
    itsTarget = vm["itsTarget"].as<std::string>();
    s_itsTarget = true;
  }

  if (vm.count("itsSystemVersion"))
  {
    itsSystemVersion = vm["itsSystemVersion"].as<std::string>();
    s_itsSystemVersion = true;
  }

  if (vm.count("itsPipelineName"))
  {
    itsPipelineName = vm["itsPipelineName"].as<std::string>();
    s_itsPipelineName = true;
  }

  if (vm.count("itsPipelineVersion"))
  {
    itsPipelineVersion = vm["itsPipelineVersion"].as<std::string>();
    s_itsPipelineVersion = true;
  }

  if (vm.count("itsIcdNumber"))
  {
    itsIcdNumber = vm["itsIcdNumber"].as<std::string>();
    s_itsIcdNumber = true;
  }

  if (vm.count("itsIcdVersion"))
  {
    itsIcdVersion = vm["itsIcdVersion"].as<std::string>();
    s_itsIcdVersion = true;
  }

  if (vm.count("itsNotes"))
  {
    itsNotes = vm["itsNotes"].as<std::string>();
    s_itsNotes = true;
  }

  if (vm.count("input-file"))
  {
    files = vm["input-file"].as< vector<string> >();
  }

  // Loop through files
  for (unsigned int i=0; i<files.size(); i++)
  {
    std::cout<<files[i]<<std::endl;
    tbb = new DAL::TBB_Timeseries(files[i]);

    DAL::CommonAttributes c = tbb->commonAttributes();

    if (s_antennaSet) c.setAntennaSet(antennaSet);
    if (s_itsTelescope) c.setTelescope(itsTelescope);
    if (s_itsFiledate) c.setFiledate(itsFiledate);
    if (s_itsObserver) c.setObserver(itsObserver);
    if (s_itsProjectID) c.setProjectID(itsProjectID);
    if (s_itsProjectTitle) c.setProjectTitle(itsProjectTitle);
    if (s_itsProjectPI) c.setProjectPI(itsProjectPI);
    if (s_itsProjectCoI) c.setProjectCoI(itsProjectCoI);
    if (s_itsProjectContact) c.setProjectContact(itsProjectContact);
    if (s_itsObservationID) c.setObservationID(itsObservationID);
    if (s_itsStartMJD) c.setStartMJD(itsStartMJD);
    if (s_itsStartTAI) c.setStartTAI(itsStartTAI);
    if (s_itsStartUTC) c.setStartUTC(itsStartUTC);
    if (s_itsEndMJD) c.setEndMJD(itsEndMJD);
    if (s_itsEndTAI) c.setEndTAI(itsEndTAI);
    if (s_itsEndUTC) c.setEndUTC(itsEndUTC);
    if (s_itsStationsList) c.setStationsList(itsStationsList);
    if (s_itsFrequencyMin) c.setFrequencyMin(itsFrequencyMin);
    if (s_itsFrequencyMax) c.setFrequencyMax(itsFrequencyMax);
    if (s_itsFrequencyCenter) c.setFrequencyCenter(itsFrequencyCenter);
    if (s_itsFrequencyUnit) c.setFrequencyUnit(itsFrequencyUnit);
    if (s_itsClockFrequency) c.setClockFrequency(itsClockFrequency);
    if (s_itsClockFrequencyUnit) c.setClockFrequencyUnit(itsClockFrequencyUnit);
    if (s_itsFilterSelection) c.setFilterSelection(itsFilterSelection);
    if (s_itsTarget) c.setTarget(itsTarget);
    if (s_itsSystemVersion) c.setSystemVersion(itsSystemVersion);
    if (s_itsPipelineName) c.setPipelineName(itsPipelineName);
    if (s_itsPipelineVersion) c.setPipelineVersion(itsPipelineVersion);
    if (s_itsIcdNumber) c.setIcdNumber(itsIcdNumber);
    if (s_itsIcdVersion) c.setIcdVersion(itsIcdVersion);
    if (s_itsNotes) c.setNotes(itsNotes);

    if (vm.count("input-file"))
    {
      files = vm["input-file"].as< vector<string> >();
    }

    if (s_metadataDir)
    {
      antennaSet = c.antennaSet();

      // Get antenna postitions from coordinates file
      positions = new std::map<std::string, casa::MPosition>();

      if (get_antenna_positions(positions, metadataDir+"LOFAR_ITRF_" + antennaSet))
      {
        std::cout << "obtained antenna positions from: " << metadataDir+"LOFAR_ITRF_" + antennaSet << std::endl;
      }
      else
      {
        std::cout << "failed to obtain antenna positions from: " << metadataDir+"LOFAR_ITRF_" + antennaSet << std::endl;
      }

      // Set antenna positions
      tbb->set_antenna_position(*positions);

      delete positions;
    }

    tbb->setCommonAttributes(c);
    delete tbb;
  }
}

