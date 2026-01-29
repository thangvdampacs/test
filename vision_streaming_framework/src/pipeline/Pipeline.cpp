#include "vf/pipeline/Pipeline.hpp"

namespace vf::pipeline {

void Pipeline::addStage(std::shared_ptr<StageThread> stage) {
    m_stages.push_back(stage);
}

void Pipeline::start() {
    for (auto& s : m_stages) {
        s->start();
    }
}

void Pipeline::stop() {
    for (auto& s : m_stages) {
        s->stop();
    }
}

} // namespace vf::pipeline

