#include <iostream>
#include <memory>
#include <set>
#include <chrono>

#include "vf/dag/Graph.hpp"
#include "vf/dag/Scheduler.hpp"
#include "vf/dag/MergeNode.hpp"
#include "vf/dag/Node.hpp"

using namespace vf::dag;

/* =====================================
 * Fake AI Node
 * ===================================== */
class FakeAINode : public Node {
public:
    FakeAINode(std::string name, int sleepMs)
        : m_name(std::move(name)), m_sleepMs(sleepMs) {}

    const std::string& name() const override {
        return m_name;
    }

    DagResult run() override {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepMs));
        std::cout << "[Node done] " << m_name
                  << " (" << m_sleepMs << " ms)\n";
        return DagResult{
            m_name,
            true,
            std::chrono::steady_clock::now()
        };
    }

private:
    std::string m_name;
    int m_sleepMs;
};

int main() {
    std::cout << "=== DAG example: slow optional node ===\n";

    /* =====================================
     * Merge policy
     * ===================================== */
    auto mergeNode = std::make_shared<MergeNode>(
        /* mandatory */ std::set<std::string>{
            "object_det",
            "lane_det"
        },
        /* optional */ std::set<std::string>{
            "traffic_sign"
        },
        /* timeout */ std::chrono::milliseconds(10)
    );

    /* =====================================
     * Scheduler & Graph
     * ===================================== */
    auto scheduler = std::make_shared<Scheduler>(mergeNode);

    scheduler->addNode(
        std::make_shared<FakeAINode>("object_det", 1)
    );
    scheduler->addNode(
        std::make_shared<FakeAINode>("lane_det", 11)
    );
    scheduler->addNode(
        std::make_shared<FakeAINode>("traffic_sign", 1) // slow
    );

    Graph graph(scheduler);

    /* =====================================
     * Run DAG
     * ===================================== */
    auto t0 = std::chrono::steady_clock::now();
    DagResult result = graph.execute();
    auto t1 = std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::cout << "----------------------------------\n";
    std::cout << "Merged result valid : "
              << (result.valid ? "YES" : "NO") << "\n";
    std::cout << "DAG execution time  : "
              << elapsed << " ms\n";
    std::cout << "----------------------------------\n";

    std::cout << "Example finished\n";
    return 0;
}

