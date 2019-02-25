#ifndef STACK_HPP
#define STACK_HPP
#include <inttypes.h>
#include <ostream>

using default_stack_report_function = void(const char*);
namespace  {
    [[ noreturn ]] void default_report(const char* message) {
        throw std::runtime_error(message);
    }
    template<typename Funcobj, typename ...Args>
    constexpr bool is_noexcept() {
        return noexcept (std::declval<Funcobj*>()(std::forward<Args>(std::declval<Args>())...));
    }

}

template<typename T, typename Report_function = default_stack_report_function>
class stack
{
public:
    using value_type = T;
    stack(Report_function* = default_report) noexcept;
    stack(stack const&);
    stack(stack &&) noexcept;
    void push(value_type const& val);
    void push(value_type && val);
    value_type pop() noexcept(is_noexcept<Report_function, const char*>());
    unsigned size() const noexcept(is_noexcept<Report_function, const char*>());
    value_type const& top() const noexcept(is_noexcept<Report_function, const char*>());
    ~stack() noexcept;
    void dump() const noexcept;
    bool valid() const noexcept;
private:
    value_type* data;
    unsigned size_, capacity;
    Report_function* report;
    uint32_t virgin;

    uint32_t secure_hash() const noexcept;
    void resize_data();
};

#include "stack.inl"
#endif // STACK_HPP
