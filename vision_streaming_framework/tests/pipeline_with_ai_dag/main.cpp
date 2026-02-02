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

static const char* toStr(vf::dag::NodeStatus s) {
    switch (s) {
        case vf::dag::NodeStatus::VALID:   return "OK";
        case vf::dag::NodeStatus::TIMEOUT: return "TIMEOUT";
        case vf::dag::NodeStatus::MISSING: return "MISS";
    }
    return "?";
}

/* =====================================================
 * Dummy Capture Stage
 * ===================================================== */
class CaptureStage : public vf::pipeline::IStage {
public:
    vf::core::Frame process(const vf::core::Frame&) override {
        vf::core::Frame f;
        f.setFrameId(++m_id);
        f.setTimestamp(steady_clock::now());
        return f;
    }

private:
    uint64_t m_id{0};
};


/* =====================================================
 * Dummy Display Stage
 * ===================================================== */
class DisplayStage : public vf::pipeline::IStage {
public:
    vf::core::Frame process(const vf::core::Frame& frame) override {
        std::cout << "[Display] Frame " << frame.frameId();

        vf::dag::DagResult r;
        if (frame.getMeta("vf.ai.result", r)) {
            std::cout << " | AI(" << r.name << ") from frame " << r.src_frame_id << " | ";
            for (const auto& [node, status] : r.node_status) {
                std::cout << node << ":" << toStr(status) << " ";
            }
        } else {
            std::cout << " | AI:none";
        }

        std::cout << "\n";
        return frame;
    }
};

/* =====================================================
 * Dummy AI Nodes
 * ===================================================== */
class ObjectNode : public vf::dag::Node {
public:
    vf::dag::DagResult run() override {
        std::this_thread::sleep_for(milliseconds(10));
        vf::dag::DagResult r;
        r.name = "object";
        r.valid = true;
        return r;
    }
};

class LaneNode : public vf::dag::Node {
public:
    vf::dag::DagResult run() override {
        std::this_thread::sleep_for(milliseconds(12));
        vf::dag::DagResult r;
        r.name = "lane";
        r.valid = true;
        return r;
    }
};

class SignNode : public vf::dag::Node {
public:
    vf::dag::DagResult run() override {
        std::this_thread::sleep_for(milliseconds(13)); // slow
        vf::dag::DagResult r;
        r.name = "sign";
        r.valid = true;
        return r;
    }
};

/* =========================================================
 * Main
 * ========================================================= */
int main() {
    std::cout << "=== Pipeline + AI DAG (30 FPS) ===\n";

    /* ---------------- DAG ---------------- */
    std::cout << "start build DAG" << std::endl;
    auto mergeNode = std::make_shared<vf::dag::MergeNode>(15); //set 15ms timeout for DAG

    mergeNode->addMandatory("object");
    mergeNode->addMandatory("lane");
    mergeNode->addMandatory("sign");

    auto scheduler  = std::make_shared<vf::dag::Scheduler>(mergeNode,1);
    auto graph      = std::make_shared<vf::dag::Graph>(scheduler);
    graph->addNode(std::make_shared<ObjectNode>());
    graph->addNode(std::make_shared<LaneNode>());
    graph->addNode(std::make_shared<SignNode>());
    /* ---------------- Pipeline ---------------- */
    constexpr size_t QDEPTH = 2;
    auto q0 = std::make_shared<vf::pipeline::StageQueue<vf::core::Frame>>(QDEPTH);
    auto q1 = std::make_shared<vf::pipeline::StageQueue<vf::core::Frame>>(QDEPTH);
    auto q2 = std::make_shared<vf::pipeline::StageQueue<vf::core::Frame>>(QDEPTH);

    auto capture = std::make_shared<CaptureStage>();
    auto ai      = std::make_shared<vf::stages::AIStage>(graph);
    auto display = std::make_shared<DisplayStage>();

    auto state_capture = std::make_shared<vf::pipeline::StageThread>(capture, q0, q1);
    auto stage_ai = std::make_shared<vf::pipeline::StageThread>(ai, q1, q2);
    auto state_display = std::make_shared<vf::pipeline::StageThread>(display, q2, nullptr);

    vf::pipeline::Pipeline pipeline(graph);
    pipeline.addQueue(q0);
    pipeline.addQueue(q1);
    pipeline.addQueue(q2);
    pipeline.addStage(state_capture);
    pipeline.addStage(stage_ai);
    pipeline.addStage(state_display);
    pipeline.start();

    /* ---------------- 30 FPS simulation ---------------- */
    constexpr int FPS = 30;
    constexpr int TOTAL_FRAMES = 30;
    auto framePeriod = milliseconds(1000 / FPS);

    for (int i = 0; i < TOTAL_FRAMES; ++i) {
        vf::core::Frame dummy_frame;
        q0->push(dummy_frame);
        std::this_thread::sleep_for(milliseconds(framePeriod));
    }

    pipeline.stop();

    std::cout << "=== Done ===\n";
    return 0;
}

