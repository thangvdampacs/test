#pragma once
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <map>
#include <mutex>

#include "vf/core/Frame.hpp"
#include "vf/pipeline/StageQueue.hpp"
#include "vf/dag/Node.hpp"
#include "vf/dag/MergeNode.hpp"
#include "vf/dag/DagResult.hpp"

namespace vf::dag {

class Scheduler {
public:
    Scheduler(std::shared_ptr<MergeNode> merge, size_t queue_depth = 1);
    ~Scheduler();

    void start();
    void stop();

    void submit(const vf::core::Frame& frame);
    bool try_get_latest(DagResult& out);

    void addNode(std::shared_ptr<Node> node);

private:
    void workerLoop();
    DagResult runDagForFrame(const vf::core::Frame& frame);

    std::vector<std::shared_ptr<Node>> m_nodes;
    std::shared_ptr<MergeNode> m_merge;

    vf::pipeline::StageQueue<vf::core::Frame> m_frameQueue;

    std::thread m_worker;
    std::atomic<bool> m_running{false};

    std::mutex m_resultMutex;
    DagResult m_latestResult;
    bool m_hasResult{false};
};

} // namespace vf::dag

