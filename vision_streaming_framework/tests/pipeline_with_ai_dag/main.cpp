#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include "vf/core/Frame.hpp"
#include "vf/core/Buffer.hpp"
#include "vf/core/PixelFormat.hpp"

#include "vf/pipeline/StageQueue.hpp"
#include "vf/pipeline/StageThread.hpp"
#include "vf/pipeline/Pipeline.hpp"

#include "vf/stages/AIStage.hpp"

#include "vf/dag/Graph.hpp"
#include "vf/dag/Scheduler.hpp"
#include "vf/dag/MergeNode.hpp"
#include "vf/dag/Node.hpp"

using namespace std::chrono;

/* =========================================================
 * Fake AI Nodes
 * ========================================================= */
class FakeAINode : public vf::dag::Node {
public:
    FakeAINode(std::string name, int ms)
        : m_name(std::move(name)), m_ms(ms) {}

    const std::string& name() const override {
        return m_name;
    }

    vf::dag::DagResult run() override {
        std::this_thread::sleep_for(milliseconds(m_ms));
        vf::dag::DagResult r;
        r.name = m_name;
        r.valid = true;
        r.timestamp = steady_clock::now();
        return r;
    }

private:
    std::string m_name;
    int m_ms;
};

/* =========================================================
 * Dummy forward stage
 * ========================================================= */
class ForwardStage : public vf::pipeline::IStage {
public:
    vf::core::Frame process(const vf::core::Frame& frame) override {
        return frame;
    }
};

/* =========================================================
 * Main
 * ========================================================= */
int main() {
    std::cout << "=== Pipeline + Async AI DAG (30 FPS) ===\n";

    /* ---------------- DAG ---------------- */
    auto mergeNode = std::make_shared<vf::dag::MergeNode>(
        /* mandatory */ std::set<std::string>{"object", "lane"},
        /* optional  */ std::set<std::string>{"traffic"},
        /* timeout   */ milliseconds(15)
    );

    auto scheduler = std::make_shared<vf::dag::Scheduler>(mergeNode);
    scheduler->addNode(std::make_shared<FakeAINode>("object", 3));
    scheduler->addNode(std::make_shared<FakeAINode>("lane", 11));
    scheduler->addNode(std::make_shared<FakeAINode>("traffic", 20));

    auto graph = std::make_shared<vf::dag::Graph>(scheduler);
    auto aiStage = std::make_shared<vf::stages::AIStage>(graph);

    /* ---------------- Pipeline ---------------- */
    constexpr size_t QDEPTH = 2;
    auto q0 = std::make_shared<vf::pipeline::StageQueue<vf::core::Frame>>(QDEPTH);
    auto q1 = std::make_shared<vf::pipeline::StageQueue<vf::core::Frame>>(QDEPTH);

    auto forward = std::make_shared<ForwardStage>();

    auto stageAI = std::make_shared<vf::pipeline::StageThread>(aiStage, q0, q1);
    auto stageOut = std::make_shared<vf::pipeline::StageThread>(forward, q1, nullptr);

    vf::pipeline::Pipeline pipeline;
    pipeline.addStage(stageAI);
    pipeline.addStage(stageOut);
    pipeline.start();

    /* ---------------- 30 FPS simulation ---------------- */
    constexpr int FPS = 30;
    constexpr int TOTAL_FRAMES = 90;
    auto framePeriod = milliseconds(1000 / FPS);

    auto start = steady_clock::now();

    for (int i = 0; i < TOTAL_FRAMES; ++i) {
        auto now = steady_clock::now();
        auto ts_ms = duration_cast<milliseconds>(now - start).count();

        auto buf = vf::core::Buffer::allocate(640 * 480);
        vf::core::Frame frame(
            i,
            now,
            640,
            480,
            vf::core::PixelFormat::GRAY8,
            buf
        );

        q0->push(frame);

        /* Fetch latest AI result (non-blocking) */
        auto ai = aiStage->latestResult();
        if (ai && ai->valid) {
            std::cout << "[PIPE] frame=" << i
                      << " t=" << ts_ms << " ms"
                      << " | AI(frame=" << ai->frame_id
                      << ", lag=" << (i - ai->frame_id) << ")\n";
        } else {
            std::cout << "[PIPE] frame=" << i
                      << " t=" << ts_ms << " ms"
                      << " | AI=none\n";
        }

        std::this_thread::sleep_until(now + framePeriod);
    }

    pipeline.stop();

    std::cout << "=== Done ===\n";
    return 0;
}

