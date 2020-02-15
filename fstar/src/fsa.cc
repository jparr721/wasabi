#include <fstar/fsa.h>

#include <stdexcept>

namespace wasabi {
void FSA::AddNode(const Node& value, bool start_state = false,
                  bool end_state = false) {
  if (start_state) {
    if (!start_set_) {
      start_state_ = value;
    } else {
      throw std::runtime_error("Start state already set");
    }
  }

  if (end_state) {
    end_states_.push_back(value);

    if (!end_set_) {
      end_set_ = true;
    }
  }

  edges_.insert(std::make_pair(value, NULL_NODE));
}

void FSA::AddEdge(const Node& left, const Node& right) {
  const auto found = edges_.find(left);

  if (found != edges_.end()) {
    found->second = right;
    return;
  }

  edges_.insert(std::make_pair(left, right));
}
}  // namespace wasabi