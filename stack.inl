#include "stack.hpp"
#include "securehash.hpp"
#include <utility>
#include <stdexcept>
#include <iostream>
#include <cassert>

#ifndef NDEBUG
#define CHECK_CONTENT() \
    if (!valid()) { dump(); report("Detected invalid state"); }
#define UPDATE() virgin = secure_hash();
#else
#define CHECK_CONTENT() __ASSERT_VOID_CAST(0)
#define UPDATE() __ASSERT_VOID_CAST(0)
#endif



template<typename T, typename Report_function>
stack<T, Report_function>::stack(Report_function* rep_fun) noexcept
    : data(nullptr)
    , size_(0)
    , capacity(0)
    , report(rep_fun)
    , virgin(secure_hash())
{}

template<typename T, typename Report_function>
stack<T, Report_function>::stack(stack const& val)
    : stack()
{
    data = new value_type[val.size_];
    capacity = val.capacity;

    for (; size_ < val.size_; ++size_) {
        data[size_] = val.data[size_];
    }
    UPDATE()
}

template<typename T, typename Report_function>
stack<T, Report_function>::stack(stack && val) noexcept
    : stack()
{
    std::swap(val.data, data);
    std::swap(val.size_, size_);
    std::swap(val.capacity, capacity);
    UPDATE()
}

template<typename T, typename Report_function>
void stack<T, Report_function>::push(value_type const& val) {
    CHECK_CONTENT()
    if (size_ == capacity) {
        resize_data();
    }
    data[size_++] = val;
    UPDATE()
}

template<typename T, typename Report_function>
void stack<T, Report_function>::push(value_type && val) {
    CHECK_CONTENT()
    if (size_ == capacity) {
        resize_data();
    }
    data[size_++] = std::move(val);
    UPDATE()
}

template<typename T, typename Report_function>
typename stack<T, Report_function>::value_type
stack<T, Report_function>::pop() noexcept(is_noexcept<Report_function, const char*>()) {
    CHECK_CONTENT()
    if (size_ <= 0) {
        throw std::runtime_error("Attemptink to shrink the empty stack (pop method)");
    }
    --size_;
    UPDATE()
    return std::move(data[size_]);
}

template<typename T, typename Report_function>
unsigned stack<T, Report_function>::size() const noexcept (is_noexcept<Report_function, const char*>()) {
    CHECK_CONTENT()
    return size_;
}

template<typename T, typename Report_function>
typename
stack<T, Report_function>::value_type const&
stack<T, Report_function>::top() const noexcept (is_noexcept<Report_function, const char*>()) {
    CHECK_CONTENT()
    if (size_ <= 0) {
        throw std::runtime_error("Attemptink to use the empty stack (top method)");
    }
    return data[size_ - 1];
}

template<typename T, typename Report_function>
stack<T, Report_function>::~stack() noexcept {
    delete [] data;
    data = nullptr;
    size_ = 0;
    capacity = 0;
}



template<typename T, typename Report_function>
void stack<T, Report_function>::resize_data() {
    CHECK_CONTENT()
    unsigned new_cap = (capacity + 1) * 2;
    value_type* buf = new value_type[new_cap];
    capacity = new_cap;
    for (unsigned i = 0; i < size_; ++i) {
        buf[i] = std::move(data[i]);
    }
    delete [] data;
    data = buf;
    UPDATE()
}


template<typename T, typename Report_function>
void stack<T, Report_function>::dump() const noexcept {
    std::cerr << "Dump of the stack\n";
    std::cerr << "Size: " << size_ << '\n';
    std::cerr << "Capacity: " << capacity << '\n';
    std::cerr << "-------------------\n";
    std::cerr << "Begin\n";
    std::cerr << "-------------------\n";
    for (int i = 0; i < size_; ++i) {
        std::cerr << '[' << i << ']' << ": " << data[i] << '\n';
    }
    std::cerr << "-------------------\n";
    std::cerr << "End\n";
    std::cerr << "-------------------\n";
    std::cerr << "The virgin's state: ";
    std::cerr << (valid() ? "Valid" : "Invalid") << '\n';
    std::cerr << "stack.virgin: " << std::hex << virgin << '\n';
    std::cerr << "correct state: " << secure_hash() << std::endl;
    std::cerr << std::endl;
}

template<typename T, typename Report_function>
bool stack<T, Report_function>::valid() const noexcept {
    return virgin == secure_hash();
}

template<typename T, typename Report_function>
uint32_t stack<T, Report_function>::secure_hash() const noexcept {
    struct {
        value_type* data;
        unsigned size, capacity;
    } val;

    val.data = data;
    val.size = size_;
    val.capacity = capacity;
    return sha_256(val);
}
