#include "vf/dag/DagResult.hpp"

namespace vf::dag {

void DagResult::mergeFrom(const DagResult& other) {
    if (!other.valid)
        return;

    dummy_data.insert(
        dummy_data.end(),
        other.dummy_data.begin(),
        other.dummy_data.end()
    );
}

} // namespace vf::dag

