#include <vocabulary/counter.h>

#include <algorithm>

namespace wasabi {
Counter::Counter(const std::vector<std::string>& text_sequence) {
  AppendSequence(text_sequence);
}

void Counter::AppendSequence(const std::vector<std::string>& text_sequence) {
  for (const auto token : text_sequence) {
    ++counts[token];
  }
}

void Counter::Increment(const std::string& key) { ++counts[key]; }

void Counter::TopN(int n,
                   std::vector<std::pair<std::string, unsigned>>& results) {
  std::partial_sort_copy(counts.begin(), counts.end(), results.begin(),
                         results.end(),
                         [](const std::pair<std::string, unsigned>& lhs,
                            const std::pair<std::string, unsigned>& rhs) {
                           return lhs.second > rhs.second;
                         });
}
}  // namespace wasabi
