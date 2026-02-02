#include "vf/dag/MergeNode.hpp"
#include <iostream>

using namespace std::chrono;

namespace vf::dag {

MergeNode::MergeNode(uint64_t timeout_ms)
    : m_timeoutMs(timeout_ms) {}

void MergeNode::addMandatory(const std::string& name) {
    m_mandatory.insert(name);
}

void MergeNode::addOptional(const std::string& name) {
    m_optional.insert(name);
}

uint64_t MergeNode::timeout_ms() const noexcept {
    return m_timeoutMs;
}

MergeNode::Decision MergeNode::evaluate(
    const std::map<std::string, DagResult>& results,
    DagResult& out,
    steady_clock::time_point start,
    steady_clock::time_point now) const {

    bool mandatory_ok = true;
    bool optional_ok = true;

    // 1 mandatory nodes
    for (const auto& name : m_mandatory) {
        auto it = results.find(name);
        if (it != results.end() && it->second.valid) {
            out.node_status[name] = NodeStatus::VALID;
        } else {
            mandatory_ok = false;
        }
    }

    // 2 optional nodes
    for (const auto& name : m_optional) {
        auto it = results.find(name);
        if (it != results.end() && it->second.valid) {
            out.node_status[name] = NodeStatus::VALID;
        }
        else {
            out.node_status[name] = NodeStatus::MISSING;
        }
    }

    // 3 READY
    if (mandatory_ok) {
        for (const auto& [_, r] : results)
            out.mergeFrom(r);
        return Decision::READY;
    }

    // 4 TIMEOUT
    if (duration_cast<milliseconds>(now - start).count() >= timeout_ms()) {
        for (const auto& name : m_mandatory) {
            if (!out.node_status.count(name))
                out.node_status[name] = NodeStatus::TIMEOUT;
        }

        for (const auto& [_, r] : results)
            out.mergeFrom(r);

        return Decision::TIMEOUT;
    }

    return Decision::WAIT;
}

} // namespace vf::dag

