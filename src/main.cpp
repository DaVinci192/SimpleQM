#include <iostream>
#include "stdint.h"

/* algorithm 
Inputs: Minterm numbers, num variables, optional don't cares

output: simplified expression

helper class:
- column/table class

helper functions: 
- calculate number of mismatching bits
- count 1's in a number

- vector to hold minterms
- dictionary to hold index to vector of minterms
*/

int8_t countOnes(int32_t n)
{
    int8_t res = 0;
    for (int8_t i = 0; i < 32; i++)
    {
        if ((n >> i) & 1)
            res++;
    }
    return res;
}

int8_t mismatchedBits(int32_t n1, int32_t n2)
{
    int8_t res = 0;
    for (int8_t i = 0; i < 32; i++)
    {
        if (((n1 >> i) & 1) != ((n2 >> i) & 1))
            res++;
    }
    return res;
}

int main()
{
    std::cout << int(mismatchedBits(0, 1)) << std::endl;
    return 0;
}

