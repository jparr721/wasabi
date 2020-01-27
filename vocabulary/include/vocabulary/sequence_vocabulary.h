#pragma once

#include <vocabulary/vocabulary.h>

#include <string>

namespace wasabi {
class SequpenceVocabulary : public Vocabulary {
 public:
  SequenceVocabulary(const std::string& unknown_token = "<UNK>",
                     const std::string& mask_token = "<MASK>",
                     const std::string& begin_seq_token = "<BEGIN>",
                     const std::string& end_seq_token = "<END>")
      : mask_token_(mask_token),
        begin_seq_token_(begin_seq_token),
        end_seq_token_(end_seq_token),
        Vocabulary(unknown_token){};

 private:
  const std::string mask_token_;
  const std::string begin_seq_token_;
  const std::string end_seq_token_;
};
}  // namespace wasabi
