#ifndef COMPARE_H
#define COMPARE_H

#include <meta>
#include <type_traits>


#include <concepts>
#include <string>
namespace deep_equal {

template<typename T>
concept has_equal_operator = std::equality_comparable<T> && !std::is_class_v<T>;

template<typename T>
concept iterable = requires(T t) {
    t.begin();
    t.end();
    typename T::value_type;
};

template<typename T>
concept mappable = requires(T t) {
    t.begin();
    t.end();
    typename T::mapped_type;
};

static_assert(has_equal_operator<int>);
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

    template<typename T>
    requires (std::is_class_v<T> && !has_equal_operator<T> && !std::is_pointer_v<T> && !iterable<T> && !mappable<T> && !pointer_like<T> && !requires(T t) { t.begin(); t.end(); })
    static bool compare_same(const T& a, const T& b) {
        template for (constexpr auto mem : std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()))) {
            if (!compare_same(a.[:mem:], b.[:mem:])) {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    requires (has_equal_operator<T> && !pointer_like<T>)
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

    template<typename T>
    requires (iterable<T> && !mappable<T> && !has_equal_operator<T>)
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
    requires (mappable<T> && !has_equal_operator<T>)
    static bool compare_same(const T& a, const T& b) {
        if (a.size() != b.size()) return false;
        auto it_a = a.begin();
        auto it_b = b.begin();
        for (; it_a != a.end() && it_b != b.end(); ++it_a, ++it_b) {
            if (!compare_same(it_a->first, it_b->first)) return false;
            if (!compare_same(it_a->second, it_b->second)) return false;
        }
        return true;
    }
};
} // namespace deep_equal


#endif // COMPARE_H