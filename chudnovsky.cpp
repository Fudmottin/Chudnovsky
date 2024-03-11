/* Comput π using the Chudnovsky Algorithm:
 *
 * 1/π = 12 Σ ((-1)^k * (6k)! * (545140134k + 13591409)) / ((3k)!(k!)^3 * (640320)^(3k+3/2))
 *
 * Where k = 0 => ∞
 *
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <future>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/mpfr.hpp>

using boost::multiprecision::cpp_int;

static const int LOG2_10 = 4;

inline cpp_int factorial(const cpp_int& num) {
    cpp_int fact = 1;
    for(cpp_int i = 2; i <= num; ++i)
        fact *= i;
    return fact;
}

inline cpp_int numerator(const cpp_int& k) {
    auto six_k_fact = factorial(6*k);
    
    return (k & 1 ? -1 : 1) * six_k_fact * (545140134*k + 13591409);
}

inline cpp_int denominator_a(const cpp_int& k) {
    auto factorial_k = factorial(k);
    return factorial(3*k) * factorial_k * factorial_k * factorial_k;
}

inline cpp_int denominator_b (int64_t k) {
    return boost::multiprecision::pow(cpp_int(640320), k * 3);
}

inline int calcPrecision(int num_terms) {
    const int PLACES_PER_TERM = 14;

    return num_terms * PLACES_PER_TERM;
}

// Function to calculate multiple of sigma series with required precision
boost::multiprecision::mpfr_float calcConstant(int precision){
    using boost::multiprecision::mpfr_float;
    mpfr_float::default_precision(precision * LOG2_10);
    mpfr_float numerator = 1;
    mpfr_float denominator_a = 426880;
    mpfr_float denominator_b_squared = 10005;
    mpfr_float result = numerator / (denominator_a * sqrt(denominator_b_squared));
    return result;
}


int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " number_of_terms\n";
        return 1;
    }

    try {
        int64_t num_terms = std::stoi(argv[1]);
        
        if (num_terms <= 0) {
            throw std::invalid_argument("Number of terms must be greater than zero.");
        }
 
        int precision = calcPrecision(num_terms);

        using boost::multiprecision::mpfr_float;
        
        mpfr_float::default_precision(precision * LOG2_10);
        mpfr_float constant = calcConstant(precision);
        std::vector<std::future<mpfr_float>> futures;

        unsigned num_threads = std::thread::hardware_concurrency();
        for(unsigned i = 0; i < num_threads; ++i) {
            futures.push_back(std::async(std::launch::async, [&, i] {
                mpfr_float local_sum = 0;
                for(int64_t k = i; k < num_terms; k += num_threads) {
                    mpfr_float temp = mpfr_float(numerator(k)) /
                                      mpfr_float(denominator_a(k) * denominator_b(k));
                    local_sum += temp;
                }
        
                return local_sum;
            }));
        }

        mpfr_float pi_inverse = 0;
        for(auto& f : futures)
            pi_inverse += f.get();

        mpfr_float pi = mpfr_float(1)/(pi_inverse * constant);
        
        std::cout << std::setprecision(precision) << pi << "\n";
    } catch(std::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

