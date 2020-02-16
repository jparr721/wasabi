#include <fstar/fsa.h>

#include <cassert>
#include <stdexcept>

namespace wasabi {
FSA::FSA() {}

void FSA::AddNode(const Node& value) {
  if (value.start_state) {
    if (!start_set_) {
      start_state_ = value;
    } else {
      throw std::runtime_error("Start state already set");
    }
  }

  if (value.end_state) {
    end_states_.push_back(value);

    if (!end_set_) {
      end_set_ = true;
    }
  }

  edges_.insert(std::make_pair(value, NULL_NODE));

  if (value == ERROR_NODE) error_set_ = true;
}

void FSA::AddEdge(const Node& left, const Node& right) {
  const auto found = edges_.find(left);

  if (found != edges_.end()) {
    found->second = right;
    return;
  }

  // Check for loops in the graph
  if (left == right && !left.end_state) {
    // Now this is a non-deterministic FSA
    is_deterministic_ = false;

    // Keep track of these nodes
    non_deterministic_nodes_.push_back(left);
  }

  edges_.insert(std::make_pair(left, right));
}

/**
 * Compiles the finite state automata dropping orphan nodes
 */
void FSA::CompileFSA(bool drop_orphans = true) {
  for (const auto& edge : edges_) {
    // If we do not have an edge here and its not an start/end state,
    if (edge.second == NULL_NODE && !edge.second.start_state &&
        !edge.second.end_state) {
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

  for (const auto& word : phrase) {
    // If the word does not exist in the fsa, return error node
    const auto found = node_find(edges_.begin(), edges_.end(), word);
    if (found == ERROR_NODE) return ERROR_NODE;

    // If found, return the new state
    const Node new_state = found;

    // Now, verify the node
    if (new_state.end_state) {
      return new_state;
    }
  }

  return ERROR_NODE;
}
}  // namespace wasabi
