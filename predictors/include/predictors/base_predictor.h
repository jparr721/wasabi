#pragma once

#include <predictors/parameters.h>
#include <tokenizer/tokenizer.h>

#include <memory>

/**
 * BasePredictor is an abstract class for prediction models
 */
namespace wasabi {
class BasePredictor {
 public:
  BasePredictor(const std::shared_ptr<Tokenizer> tokenizer)
      : tokenizer_(tokenizer){};
  virtual ~BasePredictor();

  //==========================================================
  virtual void Fit() = 0;
  virtual Parameters Parameters() { return model_parameters_; }

  //==========================================================
  bool is_init() const { return is_init_; }

 private:
  //==========================================================
  std::unique_ptr<Parameters> model_parameters_;
  const std::shared_ptr<Tokenizer> tokenizer_;

  //==========================================================
  bool it_init_ = false;
}
}  // namespace wasabi