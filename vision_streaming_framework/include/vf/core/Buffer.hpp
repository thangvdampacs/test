#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace vf::core {

/**
 * @brief Reference-counted frame buffer
 */
class Buffer {
public:
    using Ptr = std::shared_ptr<Buffer>;

    /**
     * @brief Allocate new buffer
     */
    static Ptr allocate(size_t size);

    /**
     * @brief Wrap external memory (zero-copy)
     *
     * @param data       Pointer to external memory
     * @param size       Buffer size
     * @param deleter    Custom deleter (can be empty)
     */
    static Ptr wrap(
        uint8_t* data,
        size_t size,
        void (*deleter)(uint8_t*) = nullptr
    );

    uint8_t* data() noexcept;
    const uint8_t* data() const noexcept;

    size_t size() const noexcept;
    
    ~Buffer();

private:
    Buffer(uint8_t* data, size_t size, void (*deleter)(uint8_t*));

    uint8_t* m_data;
    size_t   m_size;
    void   (*m_deleter)(uint8_t*);
};

} // namespace vf::core

