#pragma once
#include <memory>

#include "vf/pipeline/IStage.hpp"
#include "vf/core/Frame.hpp"
#include "vf/dag/Graph.hpp"

namespace vf::stages {

class AIStage : public vf::pipeline::IStage {
public:
    explicit AIStage(std::shared_ptr<vf::dag::Graph> graph);

    vf::core::Frame process(const vf::core::Frame& frame) override;

private:
    std::shared_ptr<vf::dag::Graph> m_graph;
};

} // namespace vf::stages
