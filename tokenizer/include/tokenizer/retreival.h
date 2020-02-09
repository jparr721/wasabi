#pragma once

#include <tokenizer/tokenizer.h>

#include <memory>
#include <unordered_map>

/**
 * Retreival models are used for text reading and classification
 */
namespace wasabi {
namespace search {
class Retreival {
 public:
  Retreival(const std::shared_ptr<Tokenizer>& tokenizer)
      : tokenizer_(tokenizer){};
  virtual ~Retreival();

  //==========================================================
  void Initialize();

  //==========================================================
  const bool is_built() const { return built_; }
  std::unordered_map<std::string, int> counter() const { return counter_; }

 protected:
  const std::shared_ptr<Tokenizer> tokenizer_;

  void CountWords();

 private:
  bool built_;

  std::unordered_map<std::string, int> counter_;
};
}  // namespace search
}  // namespace wasabi