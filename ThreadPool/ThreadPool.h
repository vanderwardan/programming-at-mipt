#pragma once

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <chrono>

template<class Value>
class ThreadPool {
 public:
  explicit ThreadPool(size_t worker_count);

  ~ThreadPool();

  //add new function in pool
  std::future<Value> submit(std::function<Value()> task);

  //close pool
  void shutdown();

 private:
  void functionForThread();

  //false, if pool had shut down
  bool isWork;
  std::mutex mutex;
  std::condition_variable cv;
  //tasks, that should be done in the pool
  std::queue<std::pair<std::promise<Value>, std::function<Value()>>> tasks;
  std::vector<std::thread> threads;
};
//------------ThreadPool----------------
template<class Value>
ThreadPool<Value>::ThreadPool(size_t worker_count) : isWork(true) {
  for (size_t i = 0; i < worker_count; ++i) {
    threads.emplace_back(std::move([&]() -> void { functionForThread(); }));
  }
}

template<class Value>
ThreadPool<Value>::~ThreadPool() {
  if (isWork) {
    shutdown();
  }
}

template<class Value>
std::future<Value> ThreadPool<Value>::submit(std::function<Value()> task) {
  std::unique_lock<std::mutex> lock(mutex);
  if (!isWork) {
    throw std::runtime_error("Submitted task to shut down pool");
  }

  std::promise<Value> promise;
  std::future<Value> future = promise.get_future();

  tasks.emplace(make_pair(std::move(promise), task));
  cv.notify_one();

  return future;
}

template<class Value>
void ThreadPool<Value>::shutdown() {
  isWork = false;
  cv.notify_all();

  for (auto &worker : threads) {
    worker.join();
  }
}

template<class Value>
void ThreadPool<Value>::functionForThread() {
  while (isWork) {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this]() { return !isWork || !tasks.empty(); });

    //pop task and run it
    while (!tasks.empty()) {

      std::promise<Value> promise = std::move(tasks.front().first);
      std::function<Value()> f = tasks.front().second;
      tasks.pop();
      lock.unlock();

      try {
        promise.set_value(f());
      }
      catch (...) {
        promise.set_exception(std::current_exception());
      }
      lock.lock();
    }
  }
}