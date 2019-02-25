#include "securehash.hpp"

template<typename T>
uint32_t sha_256(T const& fdata) noexcept {
    sha_256_cs(&fdata, sizeof (T));
}
