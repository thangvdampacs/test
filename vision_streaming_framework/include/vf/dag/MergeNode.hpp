#pragma once
#include <set>
#include <map>
#include <string>
#include <chrono>

#include "vf/dag/DagResult.hpp"

namespace vf::dag {

class MergeNode {
public:
    enum class Decision 
    { 
        WAIT, 
        READY, 
        TIMEOUT 
    };

    explicit MergeNode(uint64_t timeout_ms);

    void addMandatory(const std::string& name);
    void addOptional(const std::string& name);

    uint64_t timeout_ms() const noexcept;

    Decision evaluate(const std::map<std::string, DagResult>& results,
                      DagResult& out,
                      std::chrono::steady_clock::time_point start,
                      std::chrono::steady_clock::time_point now) const;

private:

    uint64_t m_timeoutMs;
    std::set<std::string> m_mandatory;
    std::set<std::string> m_optional;
};

} // namespace vf::dag

