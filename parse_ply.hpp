#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

struct Property {
  std::string name;
  std::vector<double> values; // Contains single value for non list properties
};

// Callback idea is inspired by https://w3.impa.br/~diego/software/rply/
using PropertyCallbackFunc = std::function<void(std::string_view element_name, const Property &property)>;
int parse_ply(const std::string &filepath, PropertyCallbackFunc property_callback, bool log_errors);
