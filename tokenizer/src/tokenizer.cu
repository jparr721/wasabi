#include <tokenizer/tokenizer.cuh>

namespace wasabi::gpu {
void GPUTokenizer::Tokenize() {
  const std::string corpus_raw = this->corpus_->corpus_blob();
}

__global__ void GPUTokenizer::kTokenizeToWords(const char* corpus,
                                               char* outputs) {
  int block_row = blockIdx.y;
  int block_col = blockIdx.x;

  // Splice into threads of the char array
}
}  // namespace wasabi::gpu
}  // namespace wasabi::gpu