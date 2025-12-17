# Chudnovsky Algorithm for Calculating Pi

This program calculates the value of Pi using the [Chudnovsky
algorithm](https://en.wikipedia.org/wiki/Chudnovsky_algorithm), an extremely
fast method to calculate the digits of π. It was published by the Chudnovsky
brothers in 1988.

## Prerequisites

The program requires two libraries:

1. **Boost:** A set of peer-reviewed portable C++ source libraries that extend
   the functionality of C++. You can download it from
   [here](https://www.boost.org/users/download/). 
2. **MPFR:** A library for multiple-precision floating-point computations with
   correct rounding. You can download it from
   [here](http://www.mpfr.org/mpfr-current/#download).

Make sure to install these libraries before compiling and running the program.
Both packages are also available on [brew.sh](https://brew.sh).

These libraries are used for the high precision computation required to compute
many digits of π.

## How To Compile The Program?
You need a compiler that supports C++20 standard because this code uses
features introduced in this version.  Here is an example
compile line: `c++ -std=c++20 -O3 -o chudnovsky chudnovsky.cpp
-L/usr/local/opt/mpfr/lib/ -lmpfr` This was used on my i9 MacBook Pro.

On an M2 Mac mini, I used the following line: `c++ -std=c++20 -O3 -I
/opt/homebrew/include -L /opt/homebrew/lib -o chudnovsky chudnovsky.cpp -lmpfr`

This is because HomeBrew installs packages on Intel based Macs differently than
M* based Macs. Or perhaps it is because of Big Sur. Whatever. Who cares? It's
different and it's annoying!

On Debian Linux use `c++ --std=c++20 -O3 -o chudnovsky chudnovsky.cpp -lmpfr
-lgmp`.

## Usage

To run the program, you should pass one argument which is the number of
terms to be computed in the series.  Usage example: `./chudnovsky 10` 

It will compute Pi up to 10 terms. Don't confuse terms for digits. You will get
many more digits for each term. Term refers to the series used. Each term
produces aproximately 14 decimal digits.

The output will display Pi computed up to `n` terms (where `n` was your input)
and then print out a calculated approximation of Pi.

## Algorithm Explanation

The Chudnovsky algorithm calculates 1/π as:

![12 Σ ((-1)^k * (6k)! * (545140134k + 13591409)) / ((3k)!(k!)^3 *
(640320)^(3k+3/2))](https://github.com/Fudmottin/Chudnovsky/blob/main/chudnovsky.svg)

## Note

Remember that the more terms you use, the more accurate your calculation will
be, but it will also take longer to compute. For instance, using 10 terms
provides a very accurate approximation of Pi.

This program is a great way to understand how numerical methods work and how C++
can be used for scientific computing tasks. Feel free to experiment with it and
optimize it as needed!

## Addendum

``` Pi-1000000.txt pi-diff.sh ```

These files are for testing the output for correctness. The Pi-1000000.txt file
is the first million digits of π in a single line of text. The shell script is
intended to compare the output of chudnovsky to the canonical π text to see
where disagreement begins.

``` $ time ./chudnovsky 100 > pi.txt ./chudnovsky 100 > pi.txt  0.03s user 0.01s
system 360% cpu 0.010 total $ sh pi-diff.sh cmp: EOF on pi.txt Difference found
at position: 1401 Displaying ten characters from each file starting five chars
before the difference...

           6   9   9   2   2   7   9   6   7   8

           6   9   9   2   2   8  \n

$ time ./chudnovsky 200 > pi.txt ./chudnovsky 200 > pi.txt  0.16s user 0.02s
system 944% cpu 0.018 total $ sh pi-diff.sh cmp: EOF on pi.txt Difference found
at position: 2802 Displaying ten characters from each file starting five chars
before the difference...

           7   9   9   3   4   4   0   3   7   4

           7   9   9   3   4  \n

$ time ./chudnovsky 400 > pi.txt ./chudnovsky 400 > pi.txt  1.01s user 0.06s
system 1396% cpu 0.077 total $ sh pi-diff.sh cmp: EOF on pi.txt Difference found
at position: 5601 Displaying ten characters from each file starting five chars
before the difference...

           7   7   1   0   9   8   7   0   4   0

           7   7   1   0   9   9  \n

$ time ./chudnovsky 800 > pi.txt ./chudnovsky 800 > pi.txt  6.99s user 0.28s
system 1527% cpu 0.476 total $ sh pi-diff.sh cmp: EOF on pi.txt Difference found
at position: 11202 Displaying ten characters from each file starting five chars
before the difference...

           0   1   9   9   9   2   0   5   9   8

           0   1   9   9   9  \n

```

As you can see, there is a point where the digits are start to be just plain
wrong.  that's just the nature of this implementation. If you follow the link to
the wikipedia page, you will see that there is more room for optimization. This
implementation is parallelized using std::future<mpfr_float>.

