#include <fstar/fsa.h>

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace wasabi {
FSA::FSA() {}

void FSA::AddNode(const Node& node) {
  if (node.start_state) {
    if (!start_set_) {
      start_state_ = node;
    } else {
      throw std::runtime_error("Start state already set");
    }
  }

  if (node.end_state) {
    end_states_.push_back(node);

    if (!end_set_) {
      end_set_ = true;
    }
  }

  if (node.e_state) {
    e_state_nodes_.push_back(node);
    is_deterministic_ = false;

    if (!e_state_set_) {
      e_state_set_ = true;
    }
  }

  const auto node_exists = edges_.find(node);

  if (node_exists == edges_.end()) {
    edges_.insert(std::make_pair(node, std::vector<Node>{NULL_NODE}));
  } else {
    node_exists->second.push_back(node);
  }

  if (node == ERROR_NODE) error_set_ = true;
}

void FSA::AddEdge(const Node& left, const Node& right) {
  const auto found = edges_.find(left);

  if (found != edges_.end()) {
    found->second.push_back(right);
    return;
  }

  // Check for loops in the graph
  if (left == right && !left.end_state) {
    // Now this is a non-deterministic FSA
    is_deterministic_ = false;

    // Keep track of these nodes
    non_deterministic_nodes_.push_back(left);
  }

  // Check for an incoming e-state
  if (left.e_state) {
    is_deterministic_ = false;

    // Add non deterministic and the e-state
    non_deterministic_nodes_.push_back(left);
    e_state_nodes_.push_back(left);
  }

  // Add the edge
  edges_.insert(std::make_pair(left, std::vector<Node>{right}));
}

/**
 * Compiles the finite state automata dropping orphan nodes
 */
void FSA::CompileFSA() {
  for (const auto& edge : edges_) {
    // If we do not have an edge here and its not an start/end state,
    if (edge.second[0] == NULL_NODE) {
      edges_.erase(edge.first);
    }
  }

  is_compiled_ = true;
}

Node FSA::DeterminsticEvaluation(const std::vector<std::string>& phrase) {
  assert(start_set_);
  assert(end_set_);
  assert(error_set_);
  assert(is_deterministic_);
  assert(is_compiled_);

  Node current_state = start_state_;

  if (current_state.value != phrase.at(0)) {
    return ERROR_NODE;
  }

  for (size_t i = 1; i < phrase.size(); ++i) {
    const std::string word = phrase.at(i);

    // If the word does not exist in the fsa, return error node
    auto child_nodes = edges_.at(current_state);
    auto found = node_find(child_nodes, word);

    if (found == ERROR_NODE) {
      return found;
    }

    current_state = found;

    // Now, verify the node
    if (current_state.end_state) {
      return current_state;
    }
  }

  return ERROR_NODE;
}

Node FSA::NonDeterministicEvaluation(const std::vector<std::string>& phrase) {
  assert(start_set_);
  assert(end_set_);
  assert(error_set_);
  assert(!is_deterministic_);
  assert(is_compiled_);

  Node current_state = start_state_;

  if (current_state.value != phrase.at(0)) {
    return ERROR_NODE;
  }

  for (size_t i = 0; i < phrase.size(); ++i) {
    const std::string word = phrase.at(i);
  }

  return ERROR_NODE;
}  // namespace wasabi

void FSA::HasEdge(const Node& left, const Node& right) {
  const auto left_node = edges_.find(left);

  if (left_node == edges_.end()) {
    throw std::runtime_error("No node found");
  }
}
}  // namespace wasabi
