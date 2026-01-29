#pragma once

#include <vector>
#include <memory>
#include <map>
#include <mutex>
#include <future>
#include <thread>

#include "vf/dag/Node.hpp"
#include "vf/dag/MergeNode.hpp"

namespace vf::dag {

class Scheduler {
public:
    explicit Scheduler(std::shared_ptr<MergeNode> merge);

    void addNode(std::shared_ptr<Node> node);

    DagResult run();

private:
    static DagResult nodeWorker(std::shared_ptr<Node> node);

private:
    std::vector<std::shared_ptr<Node>> m_nodes;
    std::shared_ptr<MergeNode> m_merge;

    std::map<std::string, DagResult> m_results;
    std::mutex m_mutex;

    std::vector<std::future<DagResult>> m_futures;
};

} // namespace vf::dag
