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

//_______________________________________________________________________________
//                                                          get_antenna_positions

/*!
  \brief Get antenna positions from file
  \retval positions - Map with the antenna IDs and their respective positions.
  \param filename   - Name of the input file from which the antenna positions
                      are being read.
  \return status    - Returns \e false in case an error was encountered (e.g. 
                      input file not found or read error).
*/
bool get_antenna_positions (std::map<std::string, casa::MPosition> *positions,
			    std::string filename)
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
      positions->insert(std::make_pair(name, casa::MPosition(casa::MVPosition(x, y, z),
							     casa::MPosition::ITRF)));
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

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  std::vector<std::string> files;

  DAL::TBB_Timeseries *tbb;

  std::map<std::string, casa::MPosition> *positions;

  //! Directory with antenna positions files
  std::string metadataDir = "";
  bool s_metadataDir = false;
  //! File creation date, YYYY-MM-DDThh:mm:ss.s
  std::string filedate;
  bool s_filedate = false;
  //! Name of the telescope ("LOFAR")
  std::string telescope;
  bool s_telescope = false;
  //! Name(s) of the observer(s)
  std::string observer;
  bool s_observer = false;
  //! Unique identifier for the project
  std::string projectID;
  bool s_projectID = false;
  //! Name of the project
  std::string projectTitle;
  bool s_projectTitle = false;
  //! Name of the project's principal investigator
  std::string projectPI;
  bool s_projectPI = false;
  //! Name(s) of the project's co-PI(s)
  std::string projectCoI;
  bool s_projectCoI = false;
  //! Names/Email-addresses of the project's primary contact person(s)
  std::string projectContact;
  bool s_projectContact = false;
  //! Unique identifier for the observation
  std::string observationID;
  bool s_observationID = false;
  //! Start date of the observation (MJD)
  std::string startMJD;
  bool s_startMJD = false;
  //! Start date of the observation (TAI)
  std::string startTAI;
  bool s_startTAI = false;
  //! Start date of the observation (UTC)
  std::string startUTC;
  bool s_startUTC = false;
  //! End date of the observation (MJD)
  std::string endMJD;
  bool s_endMJD = false;
  //! End date of the observation (TAI)
  std::string endTAI;
  bool s_endTAI = false;
  //! End date of the observation (UTC)
  std::string endUTC;
  bool s_endUTC = false;
  //! List of stations used during the observation
  std::vector<std::string> stationsList;
  bool s_stationsList = false;
  //! Observation minimum frequency
  double frequencyMin;
  bool s_frequencyMin = false;
  //! Observation maximum frequency
  double frequencyMax;
  bool s_frequencyMax = false;
  //! Observation center frequency
  double frequencyCenter;
  bool s_frequencyCenter = false;
  //! Observation frequency physical units
  std::string frequencyUnit;
  bool s_frequencyUnit = false;
  //! Clock frequency (LOFAR: 200.0 or 160.0)
  double clockFrequency;
  bool s_clockFrequency = false;
  //! Clock frequency physical unit
  std::string clockFrequencyUnit;
  bool s_clockFrequencyUnit = false;
  //! Antenna set specification of observation
  std::string antennaSet;
  bool s_antennaSet = false;
  //! Filter selection
  std::string filterSelection;
  bool s_filterSelection = false;
  //! Single or list of observation targets/sources
  std::string target;
  bool s_target = false;
  //! Processing system name/version
  std::string systemVersion;
  bool s_systemVersion = false;
  //! Processing pipeline name
  std::string pipelineName;
  bool s_pipelineName = false;
  //! Processing pipeline version
  std::string pipelineVersion;
  bool s_pipelineVersion = false;
  //! Interface Control Document (ICD) number 
  std::string icdNumber;
  bool s_icdNumber = false;
  //! Interface Control Document (ICD) document version
  std::string icdVersion;
  bool s_icdVersion = false;
  //! Notes or comments
  std::string notes;
  bool s_notes = false;

  bpo::options_description desc ("[AddMetadata] Available command line options");

  desc.add_options ()
    ("help,H", "Show help messages")
    ("station,s", bpo::value< std::vector<std::string> >(), "")
    ("metadataDir", bpo::value<std::string>(), "Directory containing static metadata")
    ("telescope", bpo::value<std::string>(), "telescope")
    ("observer", bpo::value<std::string>(), "observer")
    ("projectID", bpo::value<std::string>(), "projectID")
    ("projectTitle", bpo::value<std::string>(), "projectTitle")
    ("projectPI", bpo::value<std::string>(), "")
    ("projectCoI", bpo::value<std::string>(), "")
    ("projectContact", bpo::value<std::string>(), "")
    ("observationID", bpo::value<std::string>(), "")
    ("startMJD", bpo::value<std::string>(), "")
    ("startTAI", bpo::value<std::string>(), "")
    ("startUTC", bpo::value<std::string>(), "")
    ("endMJD", bpo::value<std::string>(), "")
    ("endTAI", bpo::value<std::string>(), "")
    ("endUTC", bpo::value<std::string>(), "")
    ("frequencyMin", bpo::value<double>(), "")
    ("frequencyMax", bpo::value<double>(), "")
    ("frequencyCenter", bpo::value<double>(), "")
    ("frequencyUnit", bpo::value<std::string>(), "")
    ("clockFrequency", bpo::value<double>(), "")
    ("clockFrequencyUnit", bpo::value<std::string>(), "")
    ("antennaSet", bpo::value<std::string>(), "")
    ("filterSelection", bpo::value<std::string>(), "")
    ("target", bpo::value<std::string>(), "")
    ("systemVersion", bpo::value<std::string>(), "")
    ("pipelineName", bpo::value<std::string>(), "")
    ("pipelineVersion", bpo::value<std::string>(), "")
    ("icdNumber", bpo::value<std::string>(), "")
    ("icdVersion", bpo::value<std::string>(), "")
    ("notes", bpo::value<std::string>(), "")
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

  if (vm.count("filedate"))
  {
    filedate = vm["filedate"].as<std::string>();
  }

  if (vm.count("telescope"))
  {
    telescope = vm["telescope"].as<std::string>();
    s_telescope = true;
  }

  if (vm.count("observer"))
  {
    observer = vm["observer"].as<std::string>();
    s_observer = true;
  }

  if (vm.count("projectID"))
  {
    projectID = vm["projectID"].as<std::string>();
    s_projectID = true;
  }

  if (vm.count("projectTitle"))
  {
    projectTitle = vm["projectTitle"].as<std::string>();
    s_projectTitle = true;
  }

  if (vm.count("projectPI"))
  {
    projectPI = vm["projectPI"].as<std::string>();
    s_projectPI = true;
  }

  if (vm.count("projectCoI"))
  {
    projectCoI = vm["projectCoI"].as<std::string>();
    s_projectCoI = true;
  }
  
  if (vm.count("projectContact"))
  {
    projectContact = vm["projectContact"].as<std::string>();
    s_projectContact = true;
  }

  if (vm.count("observationID"))
  {
    observationID = vm["observationID"].as<std::string>();
    s_observationID = true;
  }

  if (vm.count("startMJD"))
  {
    startMJD = vm["startMJD"].as<std::string>();
    s_startMJD = true;
  }

  if (vm.count("startTAI"))
  {
    startTAI = vm["startTAI"].as<std::string>();
    s_startTAI = true;
  }

  if (vm.count("startUTC"))
  {
    startUTC = vm["startUTC"].as<std::string>();
    s_startUTC = true;
  }

  if (vm.count("endMJD"))
  {
    endMJD = vm["endMJD"].as<std::string>();
    s_endMJD = true;
  }

  if (vm.count("endTAI"))
  {
    endTAI = vm["endTAI"].as<std::string>();
    s_endTAI = true;
  }

  if (vm.count("endUTC"))
  {
    endUTC = vm["endUTC"].as<std::string>();
    s_endUTC = true;
  }

  if (vm.count("station"))
  {
    stationsList = vm["station"].as< std::vector<std::string> >();
    s_stationsList = true;
  }

  if (vm.count("frequencyMin"))
  {
    frequencyMin = vm["frequencyMin"].as<double>();
    s_frequencyMin = true;
  }

  if (vm.count("frequencyMax"))
  {
    frequencyMax = vm["frequencyMax"].as<double>();
    s_frequencyMax = true;
  }

  if (vm.count("frequencyCenter"))
  {
    frequencyCenter = vm["frequencyCenter"].as<double>();
    s_frequencyCenter = true;
  }

  if (vm.count("frequencyUnit"))
  {
    frequencyUnit = vm["frequencyUnit"].as<std::string>();
    s_frequencyUnit = true;
  }

  if (vm.count("clockFrequency"))
  {
    clockFrequency = vm["clockFrequency"].as<double>();
    s_clockFrequency = true;
  }

  if (vm.count("clockFrequencyUnit"))
  {
    clockFrequencyUnit = vm["clockFrequencyUnit"].as<std::string>();
    s_clockFrequencyUnit = true;
  }

  if (vm.count("antennaSet"))
  {
    antennaSet = vm["antennaSet"].as<std::string>();
    s_antennaSet = true;
  }

  if (vm.count("filterSelection"))
  {
    filterSelection = vm["filterSelection"].as<std::string>();
    s_filterSelection = true;
  }

  if (vm.count("target"))
  {
    target = vm["target"].as<std::string>();
    s_target = true;
  }

  if (vm.count("systemVersion"))
  {
    systemVersion = vm["systemVersion"].as<std::string>();
    s_systemVersion = true;
  }

  if (vm.count("pipelineName"))
  {
    pipelineName = vm["pipelineName"].as<std::string>();
    s_pipelineName = true;
  }

  if (vm.count("pipelineVersion"))
  {
    pipelineVersion = vm["pipelineVersion"].as<std::string>();
    s_pipelineVersion = true;
  }

  if (vm.count("icdNumber"))
  {
    icdNumber = vm["icdNumber"].as<std::string>();
    s_icdNumber = true;
  }

  if (vm.count("icdVersion"))
  {
    icdVersion = vm["icdVersion"].as<std::string>();
    s_icdVersion = true;
  }

  if (vm.count("notes"))
  {
    notes = vm["notes"].as<std::string>();
    s_notes = true;
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
    if (s_telescope) c.setTelescope(telescope);
    if (s_filedate) c.setFiledate(filedate);
    if (s_observer) c.setObserver(observer);
    if (s_projectID) c.setProjectID(projectID);
    if (s_projectTitle) c.setProjectTitle(projectTitle);
    if (s_projectPI) c.setProjectPI(projectPI);
    if (s_projectCoI) c.setProjectCoI(projectCoI);
    if (s_projectContact) c.setProjectContact(projectContact);
    if (s_observationID) c.setObservationID(observationID);
    if (s_startMJD) c.setStartMJD(startMJD);
    if (s_startTAI) c.setStartTAI(startTAI);
    if (s_startUTC) c.setStartUTC(startUTC);
    if (s_endMJD) c.setEndMJD(endMJD);
    if (s_endTAI) c.setEndTAI(endTAI);
    if (s_endUTC) c.setEndUTC(endUTC);
    if (s_stationsList) c.setStationsList(stationsList);
    if (s_frequencyMin) c.setFrequencyMin(frequencyMin);
    if (s_frequencyMax) c.setFrequencyMax(frequencyMax);
    if (s_frequencyCenter) c.setFrequencyCenter(frequencyCenter);
    if (s_frequencyUnit) c.setFrequencyUnit(frequencyUnit);
    if (s_clockFrequency) c.setClockFrequency(clockFrequency);
    if (s_clockFrequencyUnit) c.setClockFrequencyUnit(clockFrequencyUnit);
    if (s_filterSelection) c.setFilterSelection(filterSelection);
    if (s_target) c.setTarget(target);
    if (s_systemVersion) c.setSystemVersion(systemVersion);
    if (s_pipelineName) c.setPipelineName(pipelineName);
    if (s_pipelineVersion) c.setPipelineVersion(pipelineVersion);
    if (s_icdNumber) c.setIcdNumber(icdNumber);
    if (s_icdVersion) c.setIcdVersion(icdVersion);
    if (s_notes) c.setNotes(notes);

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

