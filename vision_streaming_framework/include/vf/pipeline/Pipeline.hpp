#pragma once

#include <vector>
#include <memory>

#include "vf/pipeline/StageThread.hpp"
#include "vf/pipeline/StageQueue.hpp"
#include "vf/dag/Graph.hpp"

namespace vf::pipeline {

class Pipeline {
public:
    explicit Pipeline(std::shared_ptr<vf::dag::Graph> graph);

    void addStage(std::shared_ptr<StageThread> stage);
    void addQueue(std::shared_ptr<StageQueue<vf::core::Frame>> queue);

    void start();
    void stop();

private:
    std::shared_ptr<vf::dag::Graph> m_graph;

    std::vector<std::shared_ptr<StageThread>> m_stages;
    std::vector<std::shared_ptr<StageQueue<vf::core::Frame>>> m_queues;
};

} // namespace vf::pipeline

