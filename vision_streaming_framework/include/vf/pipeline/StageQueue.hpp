#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace vf::pipeline {

template<typename T>
class StageQueue {
public:
    explicit StageQueue(size_t max_depth)
        : max_depth_(max_depth) {}

    bool push(const T& item) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.size() >= max_depth_)
            queue_.pop(); // drop oldest
        queue_.push(item);
        cv_.notify_one();
        return true;
    }

    bool pop(T& out) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]{ return !queue_.empty() || stopped_; });

        if (stopped_)
            return false;

        out = queue_.front();
        queue_.pop();
        return true;
    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty())
            return false;
        out = queue_.front();
        queue_.pop();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stopped_ = true;
        }
        cv_.notify_all();
    }

private:
    size_t max_depth_;
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stopped_{false};
};

} // namespace vf::pipeline

