#ifndef SECUREHASH_HPP
#define SECUREHASH_HPP
#include <cinttypes>

template<typename T>
uint32_t sha_256(T const& fdata) noexcept;

uint32_t sha_256_cs(void const* fdata, uint32_t bytes_count) noexcept;

#include "securehash.inl"
#endif // SECUREHASH_HPP
