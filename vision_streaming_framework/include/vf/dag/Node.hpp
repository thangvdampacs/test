#pragma once

#include <string>
#include "vf/dag/DagResult.hpp"

namespace vf::dag {

/**
 * @brief Abstract DAG node
 */
class Node {
public:
    virtual ~Node() = default;

    virtual const std::string& name() const = 0;

    /**
     * @brief Execute node logic (blocking)
     */
    virtual DagResult run() = 0;
};

} // namespace vf::dag

