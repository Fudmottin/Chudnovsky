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
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/mpfr.hpp>


boost::multiprecision::cpp_int factorial(uint32_t num) {
    using boost::multiprecision::cpp_int;
    cpp_int fact = 1;
    for(cpp_int i = 1; i <= num; ++i)
        fact *= i;
    return fact;
}

boost::multiprecision::cpp_int numerator(uint32_t k) {
    using boost::multiprecision::cpp_int;
    
    auto six_k_fact = factorial(6*k);
    
    return (k & 1 ? -1 : 1) * six_k_fact * (545140134*k + 13591409);
}

boost::multiprecision::cpp_int denominator_a(uint32_t k) {
    auto factorial_k = factorial(k);
    return factorial(3*k) * factorial_k * factorial_k * factorial_k;
}

boost::multiprecision::cpp_int pow_3k(uint32_t k) {
    boost::multiprecision::cpp_int base = 640320;
    boost::multiprecision::cpp_int ret = 1;
    uint32_t exponent = 3 * k;

    for (uint32_t i = 0; i < exponent; ++i)
        ret *= base;

    return ret;
}

int calcPrecision(int num_terms) {
    const int MIN_PRECISION = 6; // At least six significant figures.
    const int PLACES_PER_TERM = 14; // Each term adds about 14 correct decimal places.

    int estimated_precision = num_terms * PLACES_PER_TERM;

    // Ensure at least minimum precision.
    if (estimated_precision < MIN_PRECISION) {
        return MIN_PRECISION;
    }

    return estimated_precision;
}

// Function to calculate (640320)^(3/2) with desired precision
boost::multiprecision::mpfr_float calcConstant(int precision){
    using boost::multiprecision::mpfr_float;
    mpfr_float::default_precision(precision);
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
        uint32_t num_terms = std::stoi(argv[1]);
        
        if (num_terms <= 0) {
            throw std::invalid_argument("Number of terms must be greater than zero.");
        }
 
        int precision = calcPrecision(num_terms);

        using boost::multiprecision::mpfr_float;
        
        mpfr_float constant = calcConstant(precision);
        mpfr_float pi_inverse = 0; // Initialize inverse of Pi
          
        for(uint32_t k=0; k < num_terms; ++k) {
            pi_inverse += mpfr_float(numerator(k)) / (mpfr_float(denominator_a(k)) * mpfr_float(pow_3k(k)));
        }
          
        mpfr_float pi = 1/(pi_inverse * constant);
        
        std::cout << std::setprecision(precision) << pi << "\n";
    } catch(std::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

