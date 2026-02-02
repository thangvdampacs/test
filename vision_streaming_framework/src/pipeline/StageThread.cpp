#include "vf/pipeline/StageThread.hpp"

namespace vf::pipeline {

StageThread::StageThread(
    std::shared_ptr<IStage> stage,
    std::shared_ptr<StageQueue<vf::core::Frame>> input,
    std::shared_ptr<StageQueue<vf::core::Frame>> output
)
    : m_stage(std::move(stage)),
      m_input(std::move(input)),
      m_output(std::move(output)) {}

StageThread::~StageThread() {
    stop();
}

void StageThread::start() {
    if (m_running)
        return;

    m_running = true;
    m_thread = std::thread(&StageThread::run, this);
}

void StageThread::stop() {
    if (!m_running)
        return;

    m_running = false;

    // IMPORTANT: unblock pop()
    if (m_input)
        m_input->stop();

    if (m_thread.joinable())
        m_thread.join();
}

void StageThread::run() {
    while (m_running) {
        vf::core::Frame frame;

        // BLOCKING pop
        if (!m_input->pop(frame))
            break;

        vf::core::Frame out = m_stage->process(frame);

        if (m_output) {
            m_output->push(out); // drop if full
        }
    }
}

} // namespace vf::pipeline

