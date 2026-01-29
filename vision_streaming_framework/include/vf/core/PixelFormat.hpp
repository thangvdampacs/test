#pragma once

#include <cstdint>

namespace vf::core {

/**
 * @brief Supported pixel formats
 */
enum class PixelFormat : uint32_t {
    UNKNOWN = 0,
    // Packed RGB
    RGB888,
    BGR888,
    // YUV
    YUV420P,   // I420
    NV12,
    NV21,
    GRAY8 // Grayscale
};

const char* toString(PixelFormat fmt);

} // namespace vf::core

