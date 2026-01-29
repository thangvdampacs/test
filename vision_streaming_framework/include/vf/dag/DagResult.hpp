#pragma once

#include <string>
#include <chrono>

namespace vf::dag {

/**
 * @brief Result produced by one DAG node
 */
struct DagResult {
    std::string name;
    bool        valid{false};
    std::chrono::steady_clock::time_point timestamp;
};

} // namespace vf::dag

