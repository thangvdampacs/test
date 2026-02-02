#include "vf/dag/Graph.hpp"

namespace vf::dag {

Graph::Graph(std::shared_ptr<Scheduler> scheduler)
    : m_scheduler(std::move(scheduler)) {}

void Graph::start() { m_scheduler->start(); }
void Graph::stop()  { m_scheduler->stop(); }

void Graph::addNode(std::shared_ptr<Node> node) {
    m_scheduler->addNode(std::move(node));
}

void Graph::submit(const vf::core::Frame& frame) {
    m_scheduler->submit(frame);
}

bool Graph::try_get_latest(DagResult& out) const {
    return m_scheduler->try_get_latest(out);
}

} // namespace vf::dag

