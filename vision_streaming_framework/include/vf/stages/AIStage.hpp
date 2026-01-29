#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <chrono>

#include "vf/pipeline/IStage.hpp"
#include "vf/core/Frame.hpp"
#include "vf/dag/Graph.hpp"
#include "vf/dag/DagResult.hpp"

namespace vf::stages {

/**
 * @brief AI result aligned with frame_id
 */
struct AlignedAIResult {
    uint64_t frame_id{0};
    std::chrono::steady_clock::time_point timestamp;
    vf::dag::DagResult dag_result;
    bool valid{false};
};

class AIStage : public vf::pipeline::IStage {
public:
    explicit AIStage(std::shared_ptr<vf::dag::Graph> dag);
    ~AIStage();

    vf::core::Frame process(const vf::core::Frame& frame) override;

    /* Query latest aligned AI result (non-blocking) */
    std::shared_ptr<AlignedAIResult> latestResult() const;

private:
    void aiWorkerLoop();

private:
    std::shared_ptr<vf::dag::Graph> m_dag;

    /* -------- frame submission -------- */
    std::atomic<bool> m_hasNewFrame{false};
    std::shared_ptr<vf::core::Frame> m_pendingFrame;
    mutable std::mutex m_frameMutex;

    /* -------- aligned AI result cache -------- */
    std::shared_ptr<AlignedAIResult> m_latestResult;
    mutable std::mutex m_resultMutex;

    /* -------- worker thread -------- */
    std::atomic<bool> m_running{true};
    std::thread m_worker;
};

} // namespace vf::stages
