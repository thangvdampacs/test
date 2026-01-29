#include "vf/dag/MergeNode.hpp"

namespace vf::dag {

MergeNode::MergeNode(
    std::set<std::string> mandatory,
    std::set<std::string> optional,
    std::chrono::milliseconds timeout
)
    : m_mandatory(std::move(mandatory)),
      m_optional(std::move(optional)),
      m_timeout(timeout) {}

bool MergeNode::merge(
    const std::map<std::string, DagResult>& inputs,
    DagResult& out
) {
    for (const auto& name : m_mandatory) {
        auto it = inputs.find(name);
        if (it == inputs.end() || !it->second.valid) {
            return false; // mandatory missing
        }
    }

    out.name = "MergedResult";
    out.valid = true;
    out.timestamp = std::chrono::steady_clock::now();
    return true;
}

} // namespace vf::dag

