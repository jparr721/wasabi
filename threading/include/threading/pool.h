#pragma once

#include <algorithm>
#include <functional>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace wasabi {
class Pool {
 public:
  Pool(size_t pool_size) : num_threads_(pool_size), stop(false) {
    for (size_t i = 0; i < num_threads_; ++i) {
      workers.emplace_back([this] {
        for (;;) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop || tasks.empty()) {
              return;
            }

            task = std::move(tasks.front());
            tasks.pop();
          }

          task();
        }
      });
    }
  }
  ~Pool() {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      stop = true;
    }

    condition.notify_all();

    for (auto& thread : workers) {
      thread.join();
    }
  }

  //==========================================================
  template <class F, class... Args>
  inline std::future<typename std::result_of<F(Args...)>::type> AddJob(
      F&& f, Args&&... args) {
    auto task = std::make_unique<
        std::packaged_task<typename std::result_of<F(Args...)>::type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<typename std::result_of<F(Args...)>::type> res =
        task->get_future();
    {
      std::unique_lock<std::mutex> lock(queue_mutex);

      tasks.emplace([task]() { (*task)(); });
    }

    condition.notify_one();
    return res;
  }

 private:
  bool stop;
  const int num_threads_;

  //==========================================================
  std::condition_variable condition;
  std::mutex queue_mutex;

  //==========================================================
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
};
}  // namespace wasabi