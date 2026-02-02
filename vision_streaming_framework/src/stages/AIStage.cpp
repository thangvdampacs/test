#include "vf/stages/AIStage.hpp"

namespace vf::stages {

AIStage::AIStage(std::shared_ptr<vf::dag::Graph> graph)
    : m_graph(std::move(graph)) {}

vf::core::Frame AIStage::process(const vf::core::Frame& frame) {
    m_graph->submit(frame);

    vf::core::Frame out = frame;

    vf::dag::DagResult result;
    if (m_graph->try_get_latest(result)) {
        out.setMeta("vf.ai.result", result);
    }

    return out;
}

} // namespace vf::stages

