#include <string>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "stdint.h"

/* algorithm 
Inputs: Minterm numbers, num variables, optional don't cares

output: simplified expression

helper class:
- column/table class

- column class
    entries: minterms/list of minterms, checked or not checked
        store minterms as strings

    entries separated (i.e. by number of 1's in first table)

helper functions: 
- calculate number of mismatching bits
- count 1's in a number

how to store data for first column of tabular method?
-> vector of pairs
    -> the index of each pair indicates the number of 1's in the corresponding list
    -> pair: pair<bool, vector<int>>
        fist bool indicates whether term is checked
        second vector<int> is list of minterms

how to store data for subsequent columns of tabular method?
-> list of vector of pairs
    -> entry[0]: bool (indicating checked/unchecked)
    -> entry[1]: vector<int> (indicates which minterms make up this term)
    -> each list separates sections of columns
*/



void mapMinterms(std::unordered_map<std::string, int32_t>& toInt, const std::vector<std::string>& minterms, const std::vector<std::string>& dontcares)
{
    for (int8_t i = 0; i < minterms.size(); i++)
    {
        toInt[minterms[i]] = std::stoi(minterms[i]);
    }

    for (int8_t i = 0; i < dontcares.size(); i++)
    {
        toInt[dontcares[i]] = std::stoi(dontcares[i]);
    }
}

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

void getInput(int& numVariables, std::vector<std::string>& minterms, std::vector<std::string>& dontcares)
{
    std::string line, term;
    char isDontcares;

    std::cout << "Enter number of variables: ";
    std::cin >> numVariables;
    std::cin.ignore(); // clear cin buffer to prevent getline skipping

    std::cout << "Enter minterms separated by spaces: ";
    std::getline(std::cin, line);
    std::stringstream mintermStream(line);
    while (getline(mintermStream, term, ' '))
    {
        minterms.push_back(term);
    }

    std::cout << "Do you wish to input dontcare terms (y/n)? ";
    std::cin >> isDontcares;
    std::cin.ignore();

    if (isDontcares == 'y')
    {
        std::string line2, term2;
        std::cout << "Enter dontcare terms separated by spaces: ";
        std::getline(std::cin, line2);
        std::stringstream dontcareStream(line2);
        while (std::getline(dontcareStream, term2, ' '))
        {
            dontcares.push_back(term2);
        }
    }
}

int main()
{
    std::unordered_map<std::string, int32_t> toInt;
    int numVariables = 0;
    std::vector<std::string> dontcares = {};
    std::vector<std::string> terms;

    getInput(numVariables, terms, dontcares);
    mapMinterms(toInt, terms, dontcares);



    return 0;
}

