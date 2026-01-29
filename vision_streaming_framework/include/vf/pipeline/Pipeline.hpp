#pragma once

#include <vector>
#include <memory>

#include "vf/pipeline/StageThread.hpp"

namespace vf::pipeline {

/**
 * @brief Linear pipeline manager
 */
class Pipeline {
public:
    void addStage(std::shared_ptr<StageThread> stage);
    void start();
    void stop();

private:
    std::vector<std::shared_ptr<StageThread>> m_stages;
};

} // namespace vf::pipeline

