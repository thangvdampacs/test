#include "vf/pipeline/Pipeline.hpp"

namespace vf::pipeline {

Pipeline::Pipeline(std::shared_ptr<vf::dag::Graph> graph)
    : m_graph(std::move(graph)) {}

void Pipeline::addStage(std::shared_ptr<StageThread> stage) {
    m_stages.push_back(std::move(stage));
}

void Pipeline::addQueue(std::shared_ptr<StageQueue<vf::core::Frame>> queue) {
    m_queues.push_back(std::move(queue));
}

void Pipeline::start() {
    // start AI first
    if (m_graph != nullptr)
        m_graph->start();

    // start pipeline threads
    for (auto& s : m_stages)
        s->start();
}

void Pipeline::stop() {
    // 1 stop pipeline threads (downstream → upstream)
    for (auto it = m_stages.rbegin(); it != m_stages.rend(); ++it)
        (*it)->stop();

    // 2 stop queues (unblock pop)
    for (auto& q : m_queues)
        q->stop();

    // 3 stop AI scheduler
    if (m_graph != nullptr)
        m_graph->stop();
}

} // namespace vf::pipeline
