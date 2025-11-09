#ifndef COMPARE_H
#define COMPARE_H

#include <meta>
#include <type_traits>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <variant>
#include <memory>
#include <string>
#include <tuple>

namespace deep_equal {

template<typename T>
concept iterable = requires(T t) {
    t.begin();
    t.end();
};

template<typename T>
concept mappable = requires(T t) {
    t.begin();
    t.end();
    typename T::mapped_type;
};

template<typename T>
concept has_equal_operator = requires(const T& a, const T& b) { a == b; };

template<typename T>
concept pointer_like = requires(T p) {
    *p;
    !p;
};

struct deep_equal {
    template<typename T, typename U>
    static bool compare(const T& a, const U& b) {
        using TA = std::decay_t<decltype(a)>;
        using TB = std::decay_t<decltype(b)>;
        if constexpr (std::is_same_v<TA, TB>) {
            return compare_same(a, b);
        } else {
            return false;
        }
    }

private:

    /*template<typename T>
    requires (std::is_class_v<T> && !has_equal_operator<T> && !std::is_pointer_v<T> && !iterable<T> && !mappable<T> && !pointer_like<T>)
    static bool compare_same(const T& a, const T& b) {
        template for (constexpr auto mem : std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()))) {
            if (!compare_same(a.[:mem:], b.[:mem:])) {
                return false;
            }
        }
        return a == b;
    }*/

    template<typename T>
    requires (has_equal_operator<T> && !std::is_pointer_v<T>)
    static bool compare_same(const T& a, const T& b) {
        return a == b;
    }

    template<typename Ptr>
    requires pointer_like<Ptr>
    static bool compare_same(Ptr a, Ptr b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return compare_same(*a, *b);
    }

    /*template<typename T>
    requires (iterable<T> && !mappable<T>)
    static bool compare_same(const T& a, const T& b) {
        if (a.size() != b.size()) return false;
        auto it_a = a.begin();
        auto it_b = b.begin();
        for (; it_a != a.end() && it_b != b.end(); ++it_a, ++it_b) {
            if (!compare_same(*it_a, *it_b)) return false;
        }
        return true;
    }

    template<typename T>
    requires mappable<T>
    static bool compare_same(const T& a, const T& b) {
        if (a.size() != b.size()) return false;
        auto it_a = a.begin();
        auto it_b = b.begin();
        for (; it_a != a.end() && it_b != b.end(); ++it_a, ++it_b) {
            if (!compare_same(it_a->first, it_b->first)) return false;
            if (!compare_same(it_a->second, it_b->second)) return false;
        }
        return true;
    }*/
};

// Macro to make a structure comparable (to be placed in the class)
//#define DEEP_EQUAL_COMPARABLE(...) \
//    friend bool deep_equal::deep_equal::compare_same(const decltype(*this)& a, const decltype(*this)& b) { \
//        return std::tie(a.##__VA_ARGS__) == std::tie(b.##__VA_ARGS__); \
//    }

// Example usage with structured bindings + macro
/*struct Person {
    std::string name;
    int age;
    std::vector<std::string> hobbies;
    std::optional<double> salary;

    // Enable deep comparison
    DEEP_EQUAL_COMPARABLE(name, age, hobbies, salary)
};*/

} // namespace deep_equal

// Public utility function
/*template<typename T, typename U>
bool deep_equal(const T& a, const U& b) {
    return deep_equal::deep_equal::compare(a, b);
}*/

#endif // COMPARE_H