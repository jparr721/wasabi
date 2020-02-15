#pragma once

#include <fstar/node.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace wasabi {
using edge_map = std::unordered_map<Node, Node, NodeHash>;
class FSA {
 public:
  FSA();

  //==========================================================
  void AddNode(const Node& value, bool start_state, bool end_state);
  void AddEdge(const Node& left, const Node& right);
  bool Evaluate(const std::string& phrase);

  //==========================================================
  bool is_deterministic() const { return is_deterministic_; }
  Node start_state() const { return start_state_; }
  std::vector<Node> const end_states() const { return end_states_; }
  edge_map edges() const { return edges_; }

 private:
  //==========================================================
  bool start_set_ = false;
  bool end_set_ = false;
  bool is_deterministic_ = true;

  //==========================================================
  Node start_state_;
  std::vector<Node> end_states_;

  //==========================================================
  edge_map edges_;
};
}  // namespace wasabi