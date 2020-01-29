#pragma once

#include <sys/stat.h>

#include <string>
#include <vector>

namespace wasabi {
class Corpus {
 public:
  Corpus(const std::string& file_path) : file_path_(file_path){};

  //==========================================================
  void Ingest();

  //==========================================================
  inline bool file_exists(const std::string& name) const {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
  }

  const std::string corpus_blob() const { return corpus_blob_; }

 private:
  const std::string file_path_;

  std::string corpus_blob_;
};
}  // namespace wasabi
