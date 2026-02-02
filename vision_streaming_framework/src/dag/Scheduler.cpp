#include "vf/dag/Scheduler.hpp"
#include <chrono>

using namespace std::chrono;

namespace vf::dag {

Scheduler::Scheduler(std::shared_ptr<MergeNode> merge, size_t depth)
    : m_merge(std::move(merge)), m_frameQueue(depth) {}

Scheduler::~Scheduler() { 
    stop(); 
}

void Scheduler::addNode(std::shared_ptr<Node> node) {
    m_nodes.push_back(std::move(node));
}

void Scheduler::start() {
    if(m_running)
        return;
    m_running = true;
    m_worker = std::thread(&Scheduler::workerLoop, this);
}

void Scheduler::stop() {
    if(!m_running)
        return;
    m_running = false;
    m_frameQueue.stop();
    if (m_worker.joinable())
        m_worker.join();
}

void Scheduler::submit(const vf::core::Frame& frame) {
    m_frameQueue.push(frame);
}

bool Scheduler::try_get_latest(DagResult& out) {
    std::lock_guard<std::mutex> lock(m_resultMutex);
    if (!m_hasResult)
        return false;
    out = m_latestResult;
    return true;
}

void Scheduler::workerLoop() {
    while (m_running) {
        vf::core::Frame frame;
        if (!m_frameQueue.pop(frame))
            break;

        DagResult result = runDagForFrame(frame);

        std::lock_guard<std::mutex> lock(m_resultMutex);
        m_latestResult = result;
        m_hasResult = true;
    }
}

DagResult Scheduler::runDagForFrame(const vf::core::Frame& frame) {
    DagResult merged;
    merged.src_frame_id  = frame.frameId();
    merged.src_timestamp = frame.timestamp();

    /* -----------------------------------------
     * Launch nodes in parallel
     * ----------------------------------------- */
    std::vector<std::thread> workers;
    std::mutex partialMutex;
    std::map<std::string, DagResult> partial;

    auto start = steady_clock::now();

    for (auto& node : m_nodes) {
        workers.emplace_back([&, node] {
            DagResult r = node->run();
            std::lock_guard<std::mutex> lock(partialMutex);
            partial[r.name] = r;
        });
    }

    /* -----------------------------------------
     * Merge loop (READY / TIMEOUT / WAIT)
     * ----------------------------------------- */
    while (m_running) {
        auto decision = m_merge->evaluate(
            partial,
            merged,
            start,
            steady_clock::now()
        );

        if (decision == MergeNode::Decision::READY) {
            merged.valid = true;
            merged.name  = "READY";
            break;
        }

        if (decision == MergeNode::Decision::TIMEOUT) {
            merged.valid = true;
            merged.name  = "TIMEOUT";
            break;
        }

        // WAIT -> yield CPU
        std::this_thread::yield();
    }

    /* -----------------------------------------
     * Join all node workers
     * ----------------------------------------- */
    for (auto& t : workers) {
        if (t.joinable())
            t.join();
    }

    return merged;
}

} // namespace vf::dag

