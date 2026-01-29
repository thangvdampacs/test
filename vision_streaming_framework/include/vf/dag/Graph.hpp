#pragma once

#include <memory>
#include "vf/dag/Scheduler.hpp"

namespace vf::dag {

/**
 * @brief Simple DAG graph wrapper
 */
class Graph {
public:
    explicit Graph(std::shared_ptr<Scheduler> scheduler);

    DagResult execute();

private:
    std::shared_ptr<Scheduler> m_scheduler;
};

} // namespace vf::dag

