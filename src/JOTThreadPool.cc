#include "JOT.h"

inline JOT::ThreadPool::ThreadPool(size_t numThreads) : m_stop(false) {
  for (size_t i = 0; i < numThreads; ++i)
    m_workers.emplace_back([this] {
      while (true) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->m_queue_Mutex);
          this->m_condition.wait(
              lock, [this] { return this->m_stop || !this->m_tasks.empty(); });
          if (this->m_stop && this->m_tasks.empty())
            return;
          task = std::move(this->m_tasks.front());
          this->m_tasks.pop();
        }

        task();
      }
    });
}

inline JOT::ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(m_queue_Mutex);
    m_stop = true;
  }
  m_condition.notify_all();
  for (std::thread &worker : m_workers) {
    worker.join();
  }
}

template <class F, class... Args>
inline auto JOT::ThreadPool::enqueue(F &&f, Args &&...args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(m_queue_Mutex);
    if (m_stop)
      throw std::runtime_error("enqueue on stopped ThreadPool");
    m_tasks.emplace([task]() { (*task)(); });
  }
  m_condition.notify_one();
  return res;
}