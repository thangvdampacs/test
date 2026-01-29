#pragma once

#include <memory>
#include "vf/core/Frame.hpp"

namespace vf::pipeline {

/**
 * @brief Interface for 1 pipeline stage
 */
class IStage {
public:
    virtual ~IStage() = default;

    /**
     * @brief Process one frame
     */
    virtual vf::core::Frame process(const vf::core::Frame& frame) = 0;
};

} // namespace vf::pipeline

