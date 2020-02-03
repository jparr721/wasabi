#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace wasabi {
class Vocabulary {
 public:
  explicit Vocabulary(const std::string &unknown_token = "<UNK>")
      : unknown_token_(unknown_token){};
  ~Vocabulary();

  //==========================================================
  /** Trivial Getters **/
  int size() { return token_to_index_.size(); }

  inline std::string unkown_token() { return unknown_token_; }

  inline std::unordered_map<std::string, int> token_to_index() {
    return token_to_index_;
  }

  inline std::unordered_map<int, std::string> index_to_token() {
    return index_to_token_;
  }

  //==========================================================
  void AddToken(const std::string &token);
  void BulkAddToken(const std::vector<std::string> &tokens);

  //==========================================================
  std::optional<std::string> LookupToken(int index);
  std::optional<int> LookupIndex(const std::string &token);

  //==========================================================
  /** Overrides **/
  friend std::ostream &operator<<(std::ostream &os, Vocabulary const &v);

 private:
  const std::string unknown_token_;

  std::unordered_map<std::string, int> token_to_index_;
  std::unordered_map<int, std::string> index_to_token_;
};
}  // namespace wasabi
