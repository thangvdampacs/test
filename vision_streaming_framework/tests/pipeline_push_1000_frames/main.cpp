#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

#include "vf/core/Frame.hpp"
#include "vf/core/Buffer.hpp"
#include "vf/pipeline/IStage.hpp"
#include "vf/pipeline/StageQueue.hpp"
#include "vf/pipeline/StageThread.hpp"
#include "vf/pipeline/Pipeline.hpp"

using namespace vf::core;
using namespace vf::pipeline;

/* =====================================
 * Dummy forward stage
 * ===================================== */
class DummyStage : public IStage {
public:
    Frame process(const Frame& frame) override {
        // giả lập xử lý rất nhanh
        return frame;
    }
};

int main() {
    std::cout << "=== Pipeline push 1000 frames test ===\n";

    constexpr size_t QUEUE_DEPTH = 2;
    constexpr int TOTAL_FRAMES = 1000;

    auto q_in  = std::make_shared<StageQueue<Frame>>(QUEUE_DEPTH);
    auto q_out = std::make_shared<StageQueue<Frame>>(QUEUE_DEPTH);

    auto stage = std::make_shared<DummyStage>();
    auto stageThread = std::make_shared<StageThread>(stage, q_in, q_out);

    Pipeline pipeline;
    pipeline.addStage(stageThread);

    pipeline.start();

    std::atomic<int> pushed{0};
    std::atomic<int> dropped{0};

    auto start = std::chrono::steady_clock::now();
    // auto buf = Buffer::allocate(640 * 480);
    static uint8_t fakeCameraBuffer[640 * 480];

    auto camBuf = Buffer::wrap(
        fakeCameraBuffer,
        sizeof(fakeCameraBuffer),
        nullptr
    );
    /* =====================================
     * Producer thread
     * ===================================== */
    std::thread producer([&]() {
        for (int i = 0; i < TOTAL_FRAMES; ++i) {
            Frame f(
                i,
                std::chrono::steady_clock::now(),
                640,
                480,
                PixelFormat::GRAY8,
                camBuf
            );
            auto ret = q_in->push(f);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (ret) {
                pushed++;
            } else {
                dropped++;
            }
        }
    });

    producer.join();

    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // wait for pipeline still running
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    pipeline.stop();

    std::cout << "----------------------------------\n";
    std::cout << "Total frames attempted : " << TOTAL_FRAMES << "\n";
    std::cout << "Frames pushed          : " << pushed.load() << "\n";
    std::cout << "Frames dropped         : " << dropped.load() << "\n";
    std::cout << "Push time (ms)         : " << ms << "\n";
    std::cout << "----------------------------------\n";

    std::cout << "Test finished WITHOUT BLOCK\n";
    return 0;
}

