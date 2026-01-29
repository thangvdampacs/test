#include "vf/core/Buffer.hpp"
#include <cstdlib>
#include <cstring>

namespace vf::core {

static void defaultDeleter(uint8_t* p) {
    std::free(p);
}

Buffer::Ptr Buffer::allocate(size_t size) {
    uint8_t* mem = static_cast<uint8_t*>(std::malloc(size));
    if (!mem) {
        return nullptr;
    }
    std::memset(mem, 0, size);
    return Ptr(new Buffer(mem, size, defaultDeleter));
}

Buffer::Ptr Buffer::wrap(uint8_t* data, size_t size, void (*deleter)(uint8_t*)) {
    return Ptr(new Buffer(data, size, deleter));
}

Buffer::Buffer(uint8_t* data, size_t size, void (*deleter)(uint8_t*))
    : m_data(data), m_size(size), m_deleter(deleter) {}

uint8_t* Buffer::data() noexcept {
    return m_data;
}

const uint8_t* Buffer::data() const noexcept {
    return m_data;
}

size_t Buffer::size() const noexcept {
    return m_size;
}

Buffer::~Buffer() {
    if (m_deleter && m_data) {
        m_deleter(m_data);
    }
}

} // namespace vf::core
