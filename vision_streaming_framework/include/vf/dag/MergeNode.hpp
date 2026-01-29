#pragma once

#include <map>
#include <string>
#include <set>
#include <chrono>

#include "vf/dag/DagResult.hpp"

namespace vf::dag {

/**
 * @brief Merge policy node
 */
class MergeNode {
public:
    MergeNode(
        std::set<std::string> mandatory,
        std::set<std::string> optional,
        std::chrono::milliseconds timeout
    );

    /**
     * @brief Try merge available results
     */
    bool merge(
        const std::map<std::string, DagResult>& inputs,
        DagResult& out
    );
    
    std::chrono::milliseconds m_timeout;
private:
    std::set<std::string> m_mandatory;
    std::set<std::string> m_optional;
};

} // namespace vf::dag

