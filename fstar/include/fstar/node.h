#pragma once

#include <sstream>
#include <string>

namespace wasabi {
struct Node {
  std::string value;
  bool start_state;
  bool end_state;
  bool e_state;

  bool operator==(const Node& node) const {
    return value == node.value && start_state == node.start_state &&
           end_state == node.end_state && e_state == node.e_state;
  }
  bool operator!=(const Node& node) const { return !(node == *this); }

  friend std::ostream& operator<<(std::ostream& os, const Node& node) {
    const std::string start_state_string =
        node.start_state ? "is a start state" : "is not a start state";
    const std::string end_state_string =
        node.end_state ? "is an end state" : "is not an end state";
    const std::string e_state_string =
        node.e_state ? "is an e state" : "is not an e state";

    os << "Value: " << node.value << "which " << start_state_string << " and "
       << end_state_string << " and " << e_state_string;

    return os;
  }

  static Node newNode(const std::string& value, bool start_state = false,
                      bool end_state = false, bool e_state = false) {
    return Node{value, start_state, end_state, e_state};
  }
};

struct NodeHash {
  std::size_t operator()(const Node& node) const {
    return node.value.size() + sizeof(node.start_state) +
           sizeof(node.end_state) + sizeof(node.e_state);
  }
};
}  // namespace wasabi