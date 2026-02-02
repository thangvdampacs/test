#include "vf/core/Frame.hpp"

namespace vf::core {

Frame::Frame() noexcept = default;

uint64_t Frame::frameId() const noexcept 
{ 
    return m_frameId; 
}

Frame::TimePoint Frame::timestamp() const noexcept 
{ 
    return m_timestamp; 
}

uint32_t Frame::width() const noexcept 
{ 
    return m_width; 
}

uint32_t Frame::height() const noexcept 
{ 
    return m_height; 
}

PixelFormat Frame::format() const noexcept 
{ 
    return m_format; 
}

Buffer::Ptr Frame::buffer() const noexcept 
{ 
    return m_buffer; 
}

void Frame::setFrameId(uint64_t id) noexcept { m_frameId = id; }
void Frame::setTimestamp(TimePoint ts) noexcept { m_timestamp = ts; }
void Frame::setSize(uint32_t w, uint32_t h) noexcept { m_width = w; m_height = h; }
void Frame::setFormat(PixelFormat fmt) noexcept { m_format = fmt; }
void Frame::setBuffer(Buffer::Ptr buf) noexcept { m_buffer = buf; }

} // namespace vf::core

