#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Property {
  std::string name;
  std::vector<double> values; // Contains single value for non list properties
};

struct Element {
  std::string name;
  std::unordered_map<std::string, Property> properties; // Maps property name to the property
};

// Callback idea is inspired by https://w3.impa.br/~diego/software/rply/
using PropertyCallbackFunc = std::function<void(const Element &)>;
int parse_ply(const std::string &filepath, PropertyCallbackFunc property_callback, bool log_errors);
