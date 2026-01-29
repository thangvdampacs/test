#pragma once

#include <cstdint>
#include <chrono>
#include "vf/core/PixelFormat.hpp"
#include "vf/core/Buffer.hpp"

namespace vf::core {

/**
 * @brief Frame = metadata + shared buffer
 */
class Frame {
public:
    using TimePoint = std::chrono::steady_clock::time_point;

    Frame(
        uint64_t     frameId,
        TimePoint    timestamp,
        uint32_t     width,
        uint32_t     height,
        PixelFormat  format,
        Buffer::Ptr  buffer
    );

    uint64_t    frameId()        const noexcept;
    TimePoint   timestamp() const noexcept;
    uint32_t    width()     const noexcept;
    uint32_t    height()    const noexcept;
    PixelFormat format()    const noexcept;

    Buffer::Ptr buffer() const noexcept;

private:
    uint64_t     m_frameId;
    TimePoint    m_timestamp;
    uint32_t     m_width;
    uint32_t     m_height;
    PixelFormat  m_format;
    Buffer::Ptr  m_buffer;
};

} // namespace vf::core

