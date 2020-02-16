#pragma once

#include <algorithm>
#include <functional>
#include <future>
#include <iterator>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

namespace wasabi {
class Pool {
 public:
  Pool(size_t pool_size) : num_threads_(pool_size), stop(false) {
    for (size_t i = 0; i < this->num_threads_; ++i) {
      this->workers.emplace_back([this] {
        for (;;) {
          std::function<void> task;
          {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(
                lock, [this] { return this->stop || !this->tasks.empty() });
            if (this->stop && this->tasks.empty()) {
              return;
            }

            task = std::move(this->tasks.front());
            this->tasks.pop();
          }

          task();
        }
      });
    }
  }
  ~Pool() {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      this->stop = true;
    }

    condition.notify_all();

    for (auto& thread : this->workers) {
      thread.join();
    }
  }

  //==========================================================
  template <class F, class... Args>
  inline std::future<typename std::result_of<F(Args...)>::type> AddJob(
      F&& f, Args&&... args) {
    auto task = std::make_shared<
        std::packaged_task<typename std::result_of<F(Args...)>::type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<typename std::result_of<F(Args...)>::type> res =
        task->get_future();
    {
      std::unique_lock<std::mutex> lock(queue_mutex);

      if (stop) {
        throw std::runtime_error("Cannot add job to stopped queue");
      }

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