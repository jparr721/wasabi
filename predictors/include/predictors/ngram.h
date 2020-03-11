#pragma once

#include <predictors/base_predictor.h>
#include <tokenizer/tokenizer.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace wasabi {
class NGram : private BasePredictor {
 public:
  void Fit(bool pad = true);

  std::vector<std::string> Sequence(int size = -1);

 private:
  void PadSequence(std::vector<std::string> sequence);
};
}  // namespace wasabi
