#include <experimental/meta>
#include <utility>
#include <vector>
#include <array>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <sstream>
#include <concepts>
//#include <meta>

template<typename T>
concept sql_serializable = std::is_arithmetic_v<T> || std::same_as<T, std::string>;

// Workaround for expansion statements (as per P2996R13)
namespace __impl {
    template<auto... vals>
    struct replicator_type {
        template<typename F>
        constexpr void operator>>(F body) const {
            (body.template operator()<vals>(), ...);
        }
    };
    
    template<auto... vals>
    inline constexpr replicator_type<vals...> replicator = {};
}

template<typename R>
consteval auto expand(R range) {
    std::vector<std::meta::info> args;
    for (auto r : range) {
        args.push_back(reflect_constant(r));
    }
    return substitute(^^__impl::replicator, args);
}
// end of workaround


template<typename T>
consteval std::string generate_sql_columns() {
    std::string columns;
    bool first = true;

    constexpr auto ctx = std::meta::access_context::current();

    // Iterate over public data members using the replicator workaround
    [:expand(std::meta::nonstatic_data_members_of(^^T, ctx)):] >> [&]<auto member>{
        using member_type = typename[:type_of(member):];
        if constexpr (sql_serializable<member_type>) {
            if (!first) {
                columns += ", ";
            }
            first = false;

            // Get member name
            auto name = std::meta::identifier_of(member);
            columns += name;
        }
    };

    return columns;
}





template<typename T>
constexpr std::string generate_sql_valuess(const T& obj) {
    std::string values;
    bool first = true;

    constexpr auto ctx = std::meta::access_context::current();

    // Iterate over public data members using the replicator workaround
    [:expand(std::meta::nonstatic_data_members_of(^^T, ctx)):] >> [&]<auto member>{
        using member_type = typename[:type_of(member):];
        if constexpr (sql_serializable<member_type>) {
            if (!first) {
                values += ", ";
            }
            first = false;

            // Get member value
            auto value = obj.[:member:];

            // Format value based on type
            if constexpr (std::same_as<member_type, std::string>) {
                // Escape single quotes in strings
                std::string escaped = value;
                size_t pos = 0;
                while ((pos = escaped.find('\'', pos)) != std::string::npos) {
                    escaped.replace(pos, 1, "''");
                    pos += 2;
                }
                values += "'" + escaped + "'";
            }
            else if constexpr (std::is_arithmetic_v<member_type>) {
                // Ensure consistent floating-point representation
                values += std::to_string(value);
            }
            else {
                // Direct output for other types
                values += value;
            }
        }
    };

    return values;
}

template<typename T>
constexpr std::string generate_sql_insert(const T& obj, const std::string& table_name) {
    constexpr std::string columns = generate_sql_columns<T>();
    std::string values = generate_sql_valuess(obj);
    return "INSERT INTO " + table_name + " (" + columns + ") VALUES (" + values + ");";
}


// Example usage:
struct User {
    User() = default;
    int id;
    std::string name;
    double balance;
private:
    int secret; // Ignored in SQL generation
};

std::string f() {
    constexpr User u{};
    return generate_sql_insert(u, "tbl");
}



constexpr std::string g() {
    return generate_sql_columns<User>();
}

int main() {
std::cout << f() << std::endl;
}

