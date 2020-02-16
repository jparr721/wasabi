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
  // These help with reading code without signatures
  constexpr static bool kStartNodeTrue = true;
  constexpr static bool kStartNodeFalse = false;
  constexpr static bool kEndNodeTrue = true;
  constexpr static bool kEndNodeFalse = false;

  // Public Utility Nodes ====================================
  const Node NULL_NODE =
      Node::newNode("NULL_NODE", kStartNodeFalse, kEndNodeFalse);
  const Node ERROR_NODE =
      Node::newNode("ERROR_NODE", kStartNodeFalse, kEndNodeTrue);

  //==========================================================
  void AddNode(const Node& value);
  void AddEdge(const Node& left, const Node& right);
  void CompileFSA(bool drop_orphans);
  Node DeterminsticEvaluation(const std::vector<std::string>& phrase);
  Node NonDeterministicEvaluation(const std::vector<std::string>& phrase);

  //==========================================================
  bool is_deterministic() const { return is_deterministic_; }
  bool is_compiled() const { return is_compiled_; }

  std::vector<Node> const end_states() const { return end_states_; }

  Node start_state() const { return start_state_; }
  edge_map edges() const { return edges_; }

 private:
  //==========================================================
  bool start_set_ = false;
  bool end_set_ = false;
  bool error_set_ = false;
  bool is_deterministic_ = true;
  bool is_compiled_ = false;

  //==========================================================
  Node start_state_;
  std::vector<Node> end_states_;
  std::vector<Node> non_deterministic_nodes_;

  //==========================================================
  edge_map edges_;

  // TODO(jparr721) - Add this to utils module...
  //==========================================================
  template <typename Iter, typename T>
  bool is_in(Iter it, Iter end, T value) const {
    for (; it != end; ++it) {
      if (*it == value) return true;
    }

    return false;
  }

  template <typename Iter>
  Node node_find(Iter it, Iter end, const std::string& value) const {
    for (; it != end; ++it) {
      if (std::get<0>(*it).value == value) return std::get<0>(*it);
    }

    return ERROR_NODE;
  }
};
}  // namespace wasabi