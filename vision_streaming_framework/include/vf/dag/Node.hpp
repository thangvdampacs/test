#pragma once
#include "vf/dag/DagResult.hpp"

namespace vf::dag {

class Node {
public:
    virtual ~Node() = default;
    virtual DagResult run() = 0;   // blocking inside worker OK
};

} // namespace vf::dag

