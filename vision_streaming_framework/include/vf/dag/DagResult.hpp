#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>

namespace vf::dag {

enum class NodeStatus {
    VALID,
    TIMEOUT,
    MISSING
};

struct DagResult {

    std::string name;

    uint64_t src_frame_id{0};
    std::chrono::steady_clock::time_point src_timestamp{};

    bool valid{false};

    // demo payload
    std::vector<int> dummy_data;

    void mergeFrom(const DagResult& other);

    std::unordered_map<std::string, NodeStatus> node_status;
};

} // namespace vf::dag

