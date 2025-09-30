#include <limits>
#include <numbers>
#include <print>

int main() {
    double max_double = std::numeric_limits<double>::max();
    double pi = std::numbers::pi;

    std::print("Max double (decimal): {}\n", max_double);
    std::print("Max double (hex): {:a}\n", max_double);
    std::print("Pi (decimal): {}\n", pi);
    std::print("Pi (hex): {:a}\n", std::numbers::pi);

    double infinity = std::numeric_limits<double>::infinity();
    std::print("Zero: {}\n", 1 / infinity);

    std::print("{}\n", 1.7976931348623158e308 == std::numeric_limits<double>::max());
    double x =179769313486231580793728971405303415079934132710037826936173778980444968292764750946649017977587207096330286416692887910946555547851940402630657488671505820681908902000708383676273854845817711531764475730270069855571366959622842914819860834936475292719074168444365510704342711559699508093042880177904174497792.0;

    return 0;
}

