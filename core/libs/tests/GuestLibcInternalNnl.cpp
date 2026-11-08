#include "prx/libSceLibcInternal/libSceLibcInternal.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <type_traits>

static_assert(std::is_same_v<decltype(&_Nnl_nid_postfix),
                             std::int64_t (APS5_VABI *)(FileStream*, std::uint8_t*, std::uint8_t*)>);

int main() {
    std::array<std::uint8_t, 32> bytes;
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        bytes[i] = static_cast<std::uint8_t>(i + 17);
    }
    const auto original = bytes;
    struct Case {
        std::size_t first;
        std::size_t last;
        std::int64_t expected;
    };
    const std::array<Case, 6> cases{{{2, 11, 9}, {0, 32, 32}, {3, 4, 1},
                                    {7, 7, 0}, {11, 2, 0}, {32, 0, 0}}};
    for (const auto& item : cases) {
        try {
            const auto result = _Nnl_nid_postfix(nullptr, bytes.data() + item.first,
                                                bytes.data() + item.last);
            if (result != item.expected) {
                std::fprintf(stderr, "_Nnl(%zu, %zu): expected %lld, got %lld\n", item.first,
                             item.last, static_cast<long long>(item.expected), static_cast<long long>(result));
                return EXIT_FAILURE;
            }
        } catch (const std::exception& error) {
            std::fprintf(stderr, "_Nnl(%zu, %zu) threw: %s\n", item.first, item.last, error.what());
            return EXIT_FAILURE;
        }
    }
    if (bytes != original) {
        std::fputs("_Nnl modified the buffer\n", stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
