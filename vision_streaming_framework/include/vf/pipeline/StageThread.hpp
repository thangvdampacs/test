#pragma once

#include <thread>
#include <atomic>
#include <memory>

#include "vf/pipeline/IStage.hpp"
#include "vf/pipeline/StageQueue.hpp"
#include "vf/core/Frame.hpp"

namespace vf::pipeline {

/**
 * @brief Thread wrapper for 1 stage
 */
class StageThread {
public:
    StageThread(
        std::shared_ptr<IStage> stage,
        std::shared_ptr<StageQueue<vf::core::Frame>> input,
        std::shared_ptr<StageQueue<vf::core::Frame>> output
    );

    ~StageThread();

    void start();
    void stop();

private:
    void run();

    std::shared_ptr<IStage> m_stage;
    std::shared_ptr<StageQueue<vf::core::Frame>> m_input;
    std::shared_ptr<StageQueue<vf::core::Frame>> m_output;

    std::thread m_thread;
    std::atomic<bool> m_running{false};
};

} // namespace vf::pipeline

