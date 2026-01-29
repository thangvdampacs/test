#include "vf/dag/Scheduler.hpp"
#include <chrono>

using namespace std::chrono;

namespace vf::dag {

Scheduler::Scheduler(std::shared_ptr<MergeNode> merge)
    : m_merge(std::move(merge)) {}

void Scheduler::addNode(std::shared_ptr<Node> node) {
    m_nodes.push_back(std::move(node));
}

/* ==========================================
 * Worker function (NO lambda)
 * ========================================== */
DagResult Scheduler::nodeWorker(std::shared_ptr<Node> node) {
    return node->run();
}

DagResult Scheduler::run() {
    auto start = steady_clock::now();
    m_futures.clear();

    /* ======================================
     * Launch nodes via std::async
     * ====================================== */
    for (auto& node : m_nodes) {
        m_futures.emplace_back(
            std::async(
                std::launch::async,
                &Scheduler::nodeWorker,
                node
            )
        );
    }

    DagResult merged;

    /* ======================================
     * Merge loop with timeout
     * ====================================== */
    while (true) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            // collect finished results (NON-BLOCKING)
            for (auto& f : m_futures) {
                if (f.valid() && f.wait_for(milliseconds(0)) == std::future_status::ready) {

                    DagResult r = f.get();   // safe: already ready
                    m_results[r.name] = r;
                }
            }

            if (m_merge->merge(m_results, merged)) {
                break;
            }
        }

        if (steady_clock::now() - start > milliseconds(m_merge->m_timeout)) {
            break;
        }

        std::this_thread::sleep_for(milliseconds(1));
    }

    return merged;
}

} // namespace vf::dag
