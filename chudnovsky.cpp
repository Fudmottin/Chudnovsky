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
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/mpfr.hpp>

using boost::multiprecision::cpp_int;

static const int log2n = 4;

cpp_int factorial(cpp_int num) {
    cpp_int fact = 1;
    for(cpp_int i = 1; i <= num; ++i)
        fact *= i;
    return fact;
}

cpp_int numerator(cpp_int k) {
    auto six_k_fact = factorial(6*k);
    
    return (k & 1 ? -1 : 1) * six_k_fact * (545140134*k + 13591409);
}

cpp_int denominator_a(cpp_int k) {
    auto factorial_k = factorial(k);
    return factorial(3*k) * factorial_k * factorial_k * factorial_k;
}

cpp_int pow_3k(int64_t k) {
    cpp_int base = 640320;
    cpp_int ret = 1;
    int64_t exponent = 3 * k;

    for (int64_t i = 0; i < exponent; ++i)
        ret *= base;

    return ret;
}

int calcPrecision(int num_terms) {
    const int PLACES_PER_TERM = 15;

    return num_terms * PLACES_PER_TERM;
}

// Function to calculate multiple of sigma series with required precision
boost::multiprecision::mpfr_float calcConstant(int precision){
    using boost::multiprecision::mpfr_float;
    mpfr_float::default_precision(precision * log2n);
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
        
        mpfr_float::default_precision(precision * log2n);
        mpfr_float constant = calcConstant(precision);
        mpfr_float pi_inverse = 0;
          
        for(int64_t k=0; k < num_terms; ++k) {
            pi_inverse += mpfr_float(numerator(k)) / mpfr_float(denominator_a(k) * pow_3k(k));
        }
          
        mpfr_float pi = mpfr_float(1)/(pi_inverse * constant);
        
        std::cout << std::setprecision(precision) << pi << "\n";
    } catch(std::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

