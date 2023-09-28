#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include "parse_ply.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Expected arguments: /path/to/mesh.ply" << std::endl;
    return 1;
  }
  const char *filepath = argv[1];
  parse_ply(
      filepath,
      [](std::string_view element_name, const Property &prop) {
        std::cout << element_name << ' ' << prop.name << ' ' << prop.values[0] << std::endl;
      },
      false);
  return 0;
}
