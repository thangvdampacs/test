#include "vf/pipeline/StageThread.hpp"
#include <chrono>

namespace vf::pipeline {

StageThread::StageThread(
    std::shared_ptr<IStage> stage,
    std::shared_ptr<StageQueue<vf::core::Frame>> input,
    std::shared_ptr<StageQueue<vf::core::Frame>> output
)
    : m_stage(stage), m_input(input), m_output(output) {}

StageThread::~StageThread() {
    stop();
}

void StageThread::start() {
    m_running = true;
    m_thread = std::thread(&StageThread::run, this);
}

void StageThread::stop() {
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void StageThread::run() {
    while (m_running) {
        vf::core::Frame frame(0, {}, 0, 0, vf::core::PixelFormat::UNKNOWN, nullptr);

        if (!m_input->pop(frame)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        auto out = m_stage->process(frame);

        if (m_output) {
            m_output->push(out); // drop if full
        }
    }
}

} // namespace vf::pipeline

