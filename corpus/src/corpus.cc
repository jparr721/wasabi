#include <corpus/corpus.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace wasabi {
void Corpus::Ingest() {
  if (!file_exists(this->file_path_)) {
    throw std::runtime_error("File path does not exist");
  }

  // Open the stream to the file
  std::ifstream file(this->file_path_.c_str());

  // Make our string stream
  std::stringstream corpus;

  // Read into the stream
  corpus << file.rdbuf();

  // Assign it to the class member
  this->corpus_blob_ = corpus.str();
}
}  // namespace wasabi
