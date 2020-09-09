#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace wasabi {
class Counter {
 public:
  explicit Counter(const std::vector<std::string>& text_sequence);

  void TopN(int n, std::vector<std::pair<std::string, unsigned>>& results);

  unsigned CountOf(const std::string& key) { return counts.at(key); }

 private:
  std::map<std::string, unsigned> counts;
};
}  // namespace wasabi
