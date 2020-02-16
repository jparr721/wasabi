#pragma once

#include <algorithm>
#include <iterator>
#include <thread>
#include <vector>

namespace wasabi {
template <class F>
class Pool {
 private:
  bool stop;
  const int num_threads_;

  //==========================================================
  std::condition_variable condition;

  //==========================================================
  std::vector<std::thread<F>> workers(num_threads);

 public:
  Pool(size_t pool_size) : num_threads_(pool_size), stop(false){};
  ~Pool() {
    condition.notify_all();

    for (auto& thread : workers) {
      thread.join();
    }
  }

  //==========================================================
  inline void FromThreadPool(const std::vector<std::thread<F>>& pool) {
    std::copy(workers.begin(), workers.end(), std::back_inserter(pool));
  }

  void AddJob(std::thread) {}
};
}  // namespace wasabi