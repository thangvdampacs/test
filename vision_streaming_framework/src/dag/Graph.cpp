#include "vf/dag/Graph.hpp"

namespace vf::dag {

Graph::Graph(std::shared_ptr<Scheduler> scheduler)
    : m_scheduler(scheduler) {}

DagResult Graph::execute() {
    return m_scheduler->run();
}

} // namespace vf::dag

