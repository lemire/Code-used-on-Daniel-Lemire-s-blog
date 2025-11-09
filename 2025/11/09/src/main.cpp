#include "compare.h"
#include <vector>
#include <optional>
#include <string>
#include <cstdint>
#include <print>

class hobby {
public:
    hobby(std::string n) : name(n) {}
private:
    std::string name;
};


class person {
public:
    person(std::string n, int a) : name(n), age(a) {}
    void add_hobby(const hobby& h) {
        hobbies.push_back(h);
    }
    void set_salary(uint64_t v) {
        salary = v;
    }
private:
    std::string name;
    int age;
    std::vector<hobby> hobbies;
    std::optional<uint64_t> salary;
};

bool operator==(const person& a, const person& b) {
    return deep_equal::deep_equal::compare(a, b);
}

int main() {
    person alice("Alice", 30);
    alice.add_hobby(hobby("Reading"));
    alice.set_salary(75000);
    person bob("Bob", 25);
    bob.add_hobby(hobby("Cycling"));
    bob.set_salary(60000);

    person alice2("Alice", 25);
    bob.add_hobby(hobby("Flying"));
    bob.set_salary(75000);
    hobby h1("Reading");
    hobby h2("Readding");
    std::vector<hobby> hobbies1 = { hobby("Reading"), hobby("Cycling") };
    std::vector<hobby> hobbies2 = { hobby("Reading"), hobby("Cycling") };
    std::print("Are hobbies equal? {}\n",
               deep_equal::deep_equal::compare(hobbies1, hobbies2) ? "Yes" : "No");

    std::print("Are Alice and Bob equal? {}\n",
               deep_equal::deep_equal::compare(alice, bob) ? "Yes" : "No");


    std::print("Are Alice and Bob equal? {}\n",
               alice == bob ? "Yes" : "No");
    
    std::print("Are Alice and Alice (2) equal? {}\n",
               alice == alice2 ? "Yes" : "No");
    // Main entry point
    return EXIT_SUCCESS;
}