#include "vf/stages/AIStage.hpp"
#include <chrono>

using namespace std::chrono;

namespace vf::stages {

AIStage::AIStage(std::shared_ptr<vf::dag::Graph> dag)
    : m_dag(std::move(dag)) {
    m_worker = std::thread(&AIStage::aiWorkerLoop, this);
}

AIStage::~AIStage() {
    m_running.store(false);
    if (m_worker.joinable()) {
        m_worker.join();
    }
}

vf::core::Frame AIStage::process(const vf::core::Frame& frame) {
    /* =====================================
     * Submit frame asynchronously
     * ===================================== */
    {
        std::lock_guard<std::mutex> lock(m_frameMutex);
        m_pendingFrame = std::make_shared<vf::core::Frame>(frame);
        m_hasNewFrame.store(true, std::memory_order_release);
    }

    /* Never wait for AI */
    return frame;
}

std::shared_ptr<AlignedAIResult>
AIStage::latestResult() const {
    std::lock_guard<std::mutex> lock(m_resultMutex);
    return m_latestResult;   // copy shared_ptr (cheap)
}

/* =========================================================
 * Background AI worker
 * ========================================================= */
void AIStage::aiWorkerLoop() {
    while (m_running.load()) {
        if (!m_hasNewFrame.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(milliseconds(1));
            continue;
        }

        std::shared_ptr<vf::core::Frame> frame;
        {
            std::lock_guard<std::mutex> lock(m_frameMutex);
            frame = m_pendingFrame;
            m_hasNewFrame.store(false, std::memory_order_release);
        }

        if (!frame) {
            continue;
        }

        /* Run AI DAG (slow, background) */
        vf::dag::DagResult dagResult = m_dag->execute();

        /* Align result with frame_id */
        auto aligned = std::make_shared<AlignedAIResult>();
        aligned->frame_id  = frame->frameId();
        aligned->timestamp = steady_clock::now();
        aligned->dag_result = dagResult;
        aligned->valid = dagResult.valid;

        /* Publish result */
        {
            std::lock_guard<std::mutex> lock(m_resultMutex);
            m_latestResult = aligned;
        }
    }
}

} // namespace vf::stages

