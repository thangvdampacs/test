#pragma once
#include <memory>

#include "vf/core/Frame.hpp"
#include "vf/dag/Scheduler.hpp"
#include "vf/dag/DagResult.hpp"

namespace vf::dag {

class Graph {
public:
    explicit Graph(std::shared_ptr<Scheduler> scheduler);

    void start();
    void stop();

    void addNode(std::shared_ptr<Node> node);

    void submit(const vf::core::Frame& frame);
    bool try_get_latest(DagResult& out) const;

private:
    std::shared_ptr<Scheduler> m_scheduler;
};

} // namespace vf::dag

