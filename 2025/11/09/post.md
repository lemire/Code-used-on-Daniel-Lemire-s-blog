# Automated Equality Checks in C++ with Reflection (C++26)

In C++, comparing two objects for equality is straightforward when they are simple types like integers or strings. But what about complex, nested structures? You may have to implement the comparison (`operator==`) manually for each class, which is error-prone and tedious.


Consider a simple `person` class.

```cpp
class person {
public:
    person(std::string n, int a) : name(n), age(a) {}
private:
    std::string name;
    int age;
    std::vector<hobby> hobbies;
    std::optional<uint64_t> salary;
};
```

To compare two `person` objects, we need to check if their names, ages, hobbies, and salaries match. Hobbies are a vector of `hobby` objects, each with a name. Salaries are optional. Manually writing `operator==` for this would involve checking each field, and if `hobby` changes, you would have to update it.

Without reflection, you would hae to write something ugly of the sort.

```cpp
bool operator==(const person& a, const person& b) {
    return a.name == b.name && a.age == b.age && a.hobbies == b.hobbies && a.salary == b.salary;
}
```

But this assumes `hobby` has `operator==`, and `std::vector<hobby>` has it only if `hobby` does. If `hobby` lacks `==`, compilation fails. Moreover, for large classes, it becomes annoying.

Instead, I wrote a small example that fully automates the process. You just add an operator overload.

```cpp
bool operator==(const person& a, const person& b) {
    return deep_equal::deep_equal::compare(a, b);
}
```


The trick is that C++26 allows you to query a type's members at compile time and iterate over them. The function `std::meta::nonstatic_data_members_of`, which gives us a list of members.

My complete implementation is less than a hundred lines of code, and it comes down to the following
lines of code.

```cpp
bool compare_same(const T& a, const T& b) {
    template for (constexpr auto mem : std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()))) {
        if (!compare_same(a.[:mem:], b.[:mem:])) {
                return false;
        }
    }
    return true;
}
```

It relies on the reflection operator `^^T`, which produces a `std::meta::info` value representing the type itself. Inside the function body, `std::meta::nonstatic_data_members_of(^^T, ...)` queries all non-static data members of `T`, returning a vector of reflection values in declaration order. The `unchecked` access context deliberately bypasses visibility rules, allowing comparison of private and protected members. This reflection vector is then wrapped in `std::define_static_array`, which materializes it into a static array, making it iterable in a compile-time loop. The `template for` loop iterates over each member reflection `mem` at compile time. For each, the splice expression `a.[:mem:]` directly accesses the corresponding member. The `[: :]` is more or less the inverse of the reflection operator (`^^T`): think of it as a going into a meta universe with `^^` and coming back into the standard C++ universe with `[: :]`.

The approach ensures zero runtime overhead, as all reflection, splicing, and looping are resolved during compilation. [I posted a full demonstration](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2025/11/09).