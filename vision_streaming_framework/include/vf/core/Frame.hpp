#pragma once

#include <cstdint>
#include <chrono>
#include <unordered_map>
#include <any>
#include <string>

#include "vf/core/PixelFormat.hpp"
#include "vf/core/Buffer.hpp"

namespace vf::core {

class Frame {
public:
    using TimePoint = std::chrono::steady_clock::time_point;

    Frame() noexcept;

    uint64_t    frameId()   const noexcept;
    TimePoint   timestamp() const noexcept;
    uint32_t    width()     const noexcept;
    uint32_t    height()    const noexcept;
    PixelFormat format()    const noexcept;
    Buffer::Ptr buffer()    const noexcept;

    void setFrameId(uint64_t id) noexcept;
    void setTimestamp(TimePoint ts) noexcept;
    void setSize(uint32_t w, uint32_t h) noexcept;
    void setFormat(PixelFormat fmt) noexcept;
    void setBuffer(Buffer::Ptr buf) noexcept;

    /* ===== Metadata API ===== */
    template<typename T>
    void setMeta(const std::string& key, T value) {
        m_meta[key] = std::make_any<T>(std::move(value));
    }

    template<typename T>
    bool getMeta(const std::string& key, T& out) const {
        auto it = m_meta.find(key);
        if (it == m_meta.end())
            return false;
        out = std::any_cast<T>(it->second);
        return true;
    }

private:
    uint64_t    m_frameId{0};
    TimePoint   m_timestamp{};
    uint32_t    m_width{0};
    uint32_t    m_height{0};
    PixelFormat m_format{PixelFormat::UNKNOWN};
    Buffer::Ptr m_buffer{nullptr};

    std::unordered_map<std::string, std::any> m_meta;
};

} // namespace vf::core

