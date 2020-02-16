#pragma once

#include <fstar/node.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace wasabi {
using edge_map = std::unordered_map<Node, std::vector<Node>, NodeHash>;
class FSA {
 public:
  FSA();

  //==========================================================
  // These help with reading code without signatures
  constexpr static bool kNodeTrue = true;
  constexpr static bool kNodeFalse = false;

  // Public Utility Nodes ====================================
  const Node NULL_NODE =
      Node::newNode("NULL_NODE", kNodeFalse, kNodeFalse, kNodeFalse);
  const Node ERROR_NODE =
      Node::newNode("ERROR_NODE", kNodeFalse, kNodeTrue, kNodeFalse);

  //==========================================================
  void AddNode(const Node& value);
  void AddEdge(const Node& left, const Node& right);
  void CompileFSA();
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
  // State-aware configs
  bool start_set_ = false;
  bool end_set_ = false;
  bool error_set_ = false;
  bool e_state_set_ = false;
  bool is_deterministic_ = true;
  bool is_compiled_ = false;

  //==========================================================
  Node start_state_;
  std::vector<Node> end_states_;
  std::vector<Node> e_state_nodes_;
  std::vector<Node> non_deterministic_nodes_;

  //==========================================================
  edge_map edges_;

  //==========================================================
  void HasEdge(const Node& left, const Node& right);

  // TODO(jparr721) - Add this to utils module...
  //==========================================================
  template <typename Iter, typename T>
  bool is_in(Iter it, Iter end, T value) const {
    for (; it != end; ++it) {
      if (*it == value) return true;
    }

    return false;
  }

  Node node_find(const std::vector<Node>& nodes,
                 const std::string& value) const {
    for (const auto& node : nodes) {
      if (node.value == value) {
        return node;
      }
    }

    return ERROR_NODE;
  }
};
}  // namespace wasabi