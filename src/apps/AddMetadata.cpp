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
  std::map<std::string, casa::MPosition> *positions;
  DAL::TBB_Timeseries *tbb;
  std::vector<std::string> files;
  std::string metadataDir = "";
  std::string antenna_set;
  std::string antennaSet;
  bool change_antenna_set = false;

  bpo::options_description desc ("[AddMetadata] Available command line options");

  desc.add_options ()
    ("help,H", "Show help messages")
    ("antennaSet", bpo::value<std::string>(), "Antenna set")
    ("metadataDir", bpo::value<std::string>(), "Directory containing static metadata")
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

  if (vm.count("antennaSet"))
  {
    antennaSet = vm["antennaSet"].as<std::string>();
    change_antenna_set = true;
  }

  if (vm.count("metadataDir"))
  {
    metadataDir = vm["metadataDir"].as<std::string>();
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

    if (change_antenna_set)
    {
      DAL::CommonAttributes c = tbb->commonAttributes();
      c.setAntennaSet(antennaSet);
      tbb->setCommonAttributes(c);
    }
    antenna_set = (tbb->commonAttributes()).antennaSet();

    // Get antenna postitions from coordinates file
    positions = new std::map<std::string, casa::MPosition>();

    if (get_antenna_positions(positions, metadataDir+"LOFAR_ITRF_" + antenna_set))
    {
      std::cout << "obtained antenna positions from: " << metadataDir+"LOFAR_ITRF_" + antenna_set << std::endl;
    }
    else
    {
      std::cout << "failed to obtain antenna positions from: " << metadataDir+"LOFAR_ITRF_" + antenna_set << std::endl;
    }

    // Set antenna positions
    tbb->set_antenna_position(*positions);

    delete positions;
    delete tbb;
  }
}

