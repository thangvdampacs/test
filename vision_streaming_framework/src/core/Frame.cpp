#include "vf/core/Frame.hpp"

namespace vf::core {

Frame::Frame(
    uint64_t     frameId,
    TimePoint    timestamp,
    uint32_t     width,
    uint32_t     height,
    PixelFormat  format,
    Buffer::Ptr  buffer
)
    : m_frameId(frameId),
      m_timestamp(timestamp),
      m_width(width),
      m_height(height),
      m_format(format),
      m_buffer(buffer) {}

uint64_t Frame::frameId() const noexcept {
    return m_frameId;
}

Frame::TimePoint Frame::timestamp() const noexcept {
    return m_timestamp;
}

uint32_t Frame::width() const noexcept {
    return m_width;
}

uint32_t Frame::height() const noexcept {
    return m_height;
}

PixelFormat Frame::format() const noexcept {
    return m_format;
}

Buffer::Ptr Frame::buffer() const noexcept {
    return m_buffer;
}

} // namespace vf::core

