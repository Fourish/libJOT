#ifndef JOTTHREADPOOL_H
#define JOTTHREADPOOL_H 1

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace JOT {
class ThreadPool {
public:
  ThreadPool(size_t numThreads);
  ~ThreadPool();
  template <class F, class... Args>
  auto enqueue(F &&f, Args &&...args)
      -> std::future<typename std::result_of<F(Args...)>::type>;

private:
  std::vector<std::thread> m_workers;
  std::queue<std::function<void()>> m_tasks;
  std::mutex m_queue_Mutex;
  std::condition_variable m_condition;
  bool m_stop;
};
} // namespace JOT

#endif // JOTTHREADPOOL_H