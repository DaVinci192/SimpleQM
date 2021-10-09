#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "stdint.h"

using namespace std;

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
*/



map<std::string, int32_t> toInt;
void mapMinterms(map<std::string, int32_t> m, const vector<std::string> minterms, const vector<std::string> dontcares = {})
{
    for (int8_t i = 0; i < minterms.size(); i++)
    {
        m[minterms[i]] = stoi(minterms[i]);
    }

    for (int8_t i = 0; i < dontcares.size(); i++)
    {
        m[dontcares[i]] = stoi(dontcares[i]);
    }
}

void sortMintermList(map<int8_t, vector<std::string>> mL, const vector<string> terms, const map<std::string, int32_t> termMap)
{
    for (int8_t i = 0; i < terms.size(); i++)
    {
        int8_t numOnes = countOnes(termMap[terms[i]]);
        mL[numOnes].push_back(terms[i]);
    }
}


vector<vector<int>> c1;

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

vector<std::string> getInput(std::string prompt)
{
    std::string line;
    std::string term;
    vector<std::string> terms;

    std::cout << prompt;
    std::getLine(std::cin, line);
    std::istringstream stream(line);
    while (stream >> term)
    {
        terms.push_back(term);
    }
    return terms;
}

int main()
{
    
    vector<std::string> terms = getInput("Enter minterms here: ");

    for (int i = 0; i < terms.size(); i++)
    {
        std::cout << terms[i] << std::endl;
    }

    return 0;
}

