# Chudnovsky Algorithm for Calculating Pi

This program calculates the value of Pi using the [Chudnovsky algorithm](https://en.wikipedia.org/wiki/Chudnovsky_algorithm),
an extremely fast method to calculate the digits of π. It was published by the Chudnovsky brothers in 1987.

## Prerequisites

The program requires two libraries:

1. **Boost:** A set of peer-reviewed portable C++ source libraries that extend the functionality of C++. You can download it from [here](https://www.boost.org/users/download/). 
2. **MPFR:** A library for multiple-precision floating-point computations with correct rounding. You can download it from [here](http://www.mpfr.org/mpfr-current/#download).

Make sure to install these libraries before compiling and running the program.

## How To Compile The Program?
You need a compiler that supports C++20 standard because this code uses features introduced in this version.
Here is an example compile line: `c++ -std=c++20 -O3 -o chudnovsky chudnovsky.cpp -L/usr/local/opt/mpfr/lib/ -lmpfr`

## Usage
To run the program, you should pass one argument which is the number of terms to be computed in the series.
Usage example: `./chudnovsky 10` 

It will compute Pi up to 10 terms.

The output will display Pi computed up to `n` terms (where `n` was your input) and then print out a calculated approximation of Pi.

## Algorithm Explanation
The Chudnovky algorithm calculates 1/π as:

12 Σ ((-1)^k * (6k)! * (545140134k + 13591409)) / ((3k)!(k!)^3 * (640320)^(3k+3/2))

Where the summation Σ is from k = 0 to ∞.

The program uses Boost's multiprecision and special function libraries to calculate factorials and powers. It also uses MPFR for multiple-precision floating-point computations.

## Note

Remember that the more terms you use, the more accurate your calculation will be, but it will also take longer to compute. For instance, using 10 terms provides a very accurate approximation of Pi.

This program is a great way to understand how numerical methods work and how C++ can be used for scientific computing tasks. Feel free to experiment with it and optimize it as needed!

## Addendum

```
Pi-1000000.txt
pi-diff.sh
```

These files are for testing the output for correctness. The Pi-1000000.txt file is
the first million digits of π in a single line of text. The shell script is intended
to compare the output of chudnovsky to the canonical π text to see where disagreement
begins.

```
$ time ./chudnovsky 25 > pi.txt
./chudnovsky 25 > pi.txt  0.00s user 0.00s system 71% cpu 0.007 total
$ sh pi-diff.sh
cmp: EOF on pi.txt
Difference found at position: 115
Displaying ten characters from each file starting five chars before the difference...
           5   1   3   2   8   2   3   0   6   6

           5   1   3   2   9   5   7   3   2   9

$
```
