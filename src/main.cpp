#include <iostream>

#include "quine-mccluskey.hpp"

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



int main()
{
    std::unordered_map<std::string, std::string> binary;
    int numVariables = 0;
    std::vector<std::string> dontcares = {};
    std::vector<std::string> minterms;

    qm::getInput(numVariables, minterms, dontcares);
    binary = qm::mapMintermsToBinary(minterms, dontcares, numVariables);

    std::cout << qm::combineWithDash(binary["2"], binary["3"]) << std::endl;

    return 0;
}

