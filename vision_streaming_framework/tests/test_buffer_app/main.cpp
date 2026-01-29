#include <iostream>
#include <cstring>
#include <memory>

#include "vf/core/Buffer.hpp"

using vf::core::Buffer;

/* ============================
 * Custom deleter để test wrap
 * ============================ */
static void customDeleter(uint8_t* p) {
    std::cout << "[customDeleter] called, ptr=" << static_cast<void*>(p) << std::endl;
    delete[] p;
}

int main() {
    std::cout << "==== Buffer Test App ====\n";

    /* =====================================
     * TEST 1: Allocate buffer
     * ===================================== */
    {
        std::cout << "\n[TEST 1] Buffer::allocate\n";

        auto buf = Buffer::allocate(1024);
        if (!buf) {
            std::cerr << "Allocate failed\n";
            return 1;
        }

        std::cout << "Buffer size: " << buf->size() << "\n";
        std::cout << "Buffer addr: " << static_cast<void*>(buf->data()) << "\n";

        // ghi dữ liệu
        buf->data()[0] = 0xAB;
        buf->data()[1] = 0xCD;

        std::cout << "Data[0]=0x" << std::hex << int(buf->data()[0]) << "\n";
        std::cout << "Data[1]=0x" << std::hex << int(buf->data()[1]) << "\n";
    }
    std::cout << "[TEST 1] scope end → buffer should be freed\n";

    /* =====================================
     * TEST 2: Shared ownership
     * ===================================== */
    {
        std::cout << "\n[TEST 2] Shared ownership\n";

        auto buf1 = Buffer::allocate(16);
        buf1->data()[0] = 42;

        {
            auto buf2 = buf1; // shared_ptr copy
            std::cout << "buf2 sees value: " << int(buf2->data()[0]) << "\n";

            buf2->data()[0] = 99;
        } // buf2 destroyed

        // buf1 vẫn còn sống
        std::cout << "buf1 sees value after buf2 destroyed: "
                  << int(buf1->data()[0]) << "\n";
    }
    std::cout << "[TEST 2] scope end → buffer should be freed\n";

    /* =====================================
     * TEST 3: Wrap external memory (DMABUF-style)
     * ===================================== */
    {
        std::cout << "\n[TEST 3] Buffer::wrap (external memory)\n";

        uint8_t* ext = new uint8_t[32];
        std::memset(ext, 0x11, 32);

        auto buf = Buffer::wrap(ext, 32, customDeleter);

        std::cout << "Wrapped buffer addr: " << static_cast<void*>(buf->data()) << "\n";
        std::cout << "Wrapped buffer size: " << buf->size() << "\n";
        std::cout << "Data[0]=0x" << std::hex << int(buf->data()[0]) << "\n";
    }
    std::cout << "[TEST 3] scope end → customDeleter MUST be called\n";

    std::cout << "\n==== All tests finished ====\n";
    return 0;
}

