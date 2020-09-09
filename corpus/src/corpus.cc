#include <corpus/corpus.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace wasabi {
void Corpus::Ingest() {
  if (!file_exists(file_path_)) {
    throw std::runtime_error("File path does not exist");
  }
  std::ifstream file(file_path_.c_str());
  std::stringstream corpus;
  corpus << file.rdbuf();
  corpus_blob_ = corpus.str();
}
}  // namespace wasabi
