#pragma once

#include <corpus/corpus.h>
#include <tokenizer/tokenizer.h>

#include <memory>
#include <string>
#include <vector>

namespace wasabi::gpu {
class GPUTokenizer {
 public:
  GPUTokenizer(const std::shared_ptr<Corpus>& corpus, TokenizerType type)
      : corpus_(corpus), type_(type){};

  //==========================================================
  void Tokenize();
  __global__ void kTokenizeToWords(const char* corpus, char* outputs);
  __global__ void kTokenizeToSentences(const char* corpus, char* outputs);

  //==========================================================
  const std::vector<std::string> tokenized() const { return tokenized_; }

 private:
  // Make this static for now
  static constexpr int BLOCK_SIZE = 64;

  //==========================================================
  const TokenizerType type_;

  //==========================================================
  std::vector<std::string> tokenized_;

  //==========================================================
  const std::shared_ptr<Corpus> corpus_;
};
}  // namespace wasabi::gpu