#pragma once

#include <string>

namespace wasabi {
struct Node {
  bool operator==(const Node& node) const {
    return this->value == node.value && this->start_state == node.start_state &&
           this->end_state == node.end_state;
  }

  std::string value;
  bool start_state;
  bool end_state;

  static Node newNode(const std::string& value, bool start_state = false,
                      bool end_state = false) {
    return Node{value, start_state, end_state};
  }
};

struct NodeHash {
  std::size_t operator()(const Node& node) const {
    return node.value.size() + sizeof(node.start_state) +
           sizeof(node.end_state);
  }
};
}  // namespace wasabi