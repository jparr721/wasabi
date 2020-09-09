#include <vocabulary/counter.h>

#include <algorithm>

namespace wasabi {
Counter::Counter(const std::vector<std::string>& text_sequence) {
  for (const auto token : text_sequence) {
    const auto token_exists = counts.find(token);

    if (token_exists == counts.end()) {
      counts[token] = 1;
    } else {
      ++counts[token];
    }
  }
}

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
