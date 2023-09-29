#include <iostream>

#include "parse_ply.hpp"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Expected arguments: /path/to/mesh.ply" << std::endl;
    return 1;
  }
  const char *filepath = argv[1];
  parse_ply(
      filepath,
      [](const Element &e) {
        if (e.name == "vertex") {
          std::cout << e.properties.at("x").values[0] << ' ' << e.properties.at("y").values[0] << ' '
                    << e.properties.at("z").values[0] << std::endl;
        }
      },
      false);
  return 0;
}
