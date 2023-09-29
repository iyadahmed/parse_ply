#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "parse_ply.hpp"

// File format references:
// https://en.wikipedia.org/wiki/PLY_(file_format)
// https://w3.impa.br/~diego/software/rply/
// https://www.loc.gov/preservation/digital/formats/fdd/fdd000501.shtml

struct PropertyDefinition {
  std::string rest_of_line;
};

struct ElementDefintion {
  std::string rest_of_line;
  std::vector<PropertyDefinition> property_definitions;
};

static std::string get_remaining_string(std::istringstream &iss) {
  // https://stackoverflow.com/a/13506596/8094047
  return iss.str().substr(iss.tellg());
}

Property parse_property_definition(const PropertyDefinition &pd, std::istringstream &data_iss) {
  Property result;
  double value; // Somewhere to store values temporarily
  std::istringstream pd_iss(pd.rest_of_line);
  std::string token;
  pd_iss >> token;
  if (token == "list") {
    pd_iss >> token; // Skip data type of count, we always use size_t
    pd_iss >> token; // Skip data type of elements, we always use double
    pd_iss >> result.name;

    std::size_t num_elems;
    data_iss >> num_elems;
    for (std::size_t i = 0; i < num_elems; i++) {
      data_iss >> value;
      result.values.push_back(value);
    }
  } else {
    pd_iss >> result.name;
    data_iss >> value;
    result.values.push_back(value);
  }
  return result;
}

int parse_ply(const std::string &filepath, PropertyCallbackFunc property_callback, bool enable_logging) {

  auto log_error = [enable_logging](const std::string &message) {
    if (enable_logging)
      std::cerr << message << std::endl;
  };

  std::ifstream ifs(filepath, std::ifstream::binary);
  std::string line;
  std::getline(ifs, line); // Expecting "ply"
  std::getline(ifs, line);
  std::istringstream ed_stream(line);
  std::string token;
  ed_stream >> token; // Expecting "format"
  ed_stream >> token;
  if (token != "ascii") {
    log_error("Not ASCII");
    return 1;
  }
  std::vector<ElementDefintion> element_definitions;
  // Reading file line by line: https://stackoverflow.com/a/15500876/8094047
  while (std::getline(ifs, line).good()) {
    // Reading line by line into a string stream:
    // https://stackoverflow.com/a/7868998/8094047
    ed_stream = std::istringstream(line);
    ed_stream >> token;
    ed_stream >> std::ws; // Get rid of white space after token
    if (token == "element") {
      element_definitions.push_back({get_remaining_string(ed_stream), {}});
    } else if (token == "property") {
      if (element_definitions.empty()) {
        log_error("Expected at least one element definition before property "
                  "definition");
        return 1;
      }
      element_definitions.back().property_definitions.push_back({get_remaining_string(ed_stream)});
    } else if (token == "end_header") {
      break;
    }
  }
  for (const auto &ed : element_definitions) {
    ed_stream = std::istringstream(ed.rest_of_line);
    std::string element_name;
    std::size_t element_count;
    ed_stream >> element_name;
    ed_stream >> element_count;
    for (size_t i = 0; i < element_count; i++) {
      std::getline(ifs, line);
      std::istringstream data_iss(line);
      Element e;
      e.name = element_name;
      for (const auto &pd : ed.property_definitions) {
        Property p = parse_property_definition(pd, data_iss);
        e.properties[p.name] = p;
      }
      property_callback(e);
    }
  }
  return 0;
}
