#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace vf::pipeline {

/**
 * @brief Bounded queue between pipelines 
 *
 * Drop policy:
 *  - if full → drop new frame  (push fail)
 */
template <typename T>
class StageQueue {
public:
    explicit StageQueue(size_t capacity)
        : m_capacity(capacity) {}

    bool push(const T& item) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.size() >= m_capacity) {
            return false; // drop newest
        }
        m_queue.push(item);
        m_cv.notify_one();
        return true;
    }

    bool pop(T& out) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        out = m_queue.front();
        m_queue.pop();
        return true;
    }

private:
    size_t m_capacity;
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

} // namespace vf::pipeline

