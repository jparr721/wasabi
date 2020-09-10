#pragma once

#include <vocabulary/counter.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace wasabi {
struct BayesDataLoader {
  int n_classes;

  // value, label
  std::vector<std::pair<std::string, std::string>> data;

  std::unique_ptr<Counter> prior_probabilities;
};
}  // namespace wasabi
