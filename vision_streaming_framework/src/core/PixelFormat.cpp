#include "vf/core/PixelFormat.hpp"

namespace vf::core {

const char* toString(PixelFormat fmt) {
    switch (fmt) {
        case PixelFormat::RGB888:   return "RGB888";
        case PixelFormat::BGR888:   return "BGR888";
        case PixelFormat::YUV420P:  return "YUV420P";
        case PixelFormat::NV12:     return "NV12";
        case PixelFormat::NV21:     return "NV21";
        case PixelFormat::GRAY8:    return "GRAY8";
        default:                    return "UNKNOWN";
    }
}

} // namespace vf::core
