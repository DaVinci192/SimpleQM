#ifndef QUINE_MCCLUSKEY
#define QUINE_MCCLUSKEY

#include <string>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <sstream>

namespace qm
{
    /// @brief Prompts and reads user input from the console into variables
    /// @param numVariables the number of variables in the logic function
    /// @param minterms the list of minterms (base 10) inputted by the user
    /// @param dontcares the list of dontcares (base 10) inputted by the user
    void getInput(int& numVariables, std::vector<std::string>& minterms, std::vector<std::string>& dontcares);

    /// @brief Defines an unordered map which pairs minterms and dontcare terms (base 10, strings) to binary equivalent (represented in a string)
    /// @param minterms the list of minterms (base 10) inputted by the user
    /// @param dontcares the list of dontcares (base 10) inputted by the user
    std::unordered_map<std::string, std::string> mapMintermsToBinary(const std::vector<std::string>& minterms, const std::vector<std::string>& dontcares, const int numVariables);

    /// @brief Counts the number of ones in the binary form of n
    /// @param n the term value in binary form
    /// @return Returns the number of ones in the binary form of n
    int countOnes(std::string n);

    /// @brief Counts the number of mismatched bits/dashes between 2 numbers
    /// @param n1 first term in binary form
    /// @param n2 second term in binary form
    /// @return Returns the number of mismatched bits/dashes between n1 and n2
    int mismatchedBits(std::string n1, std::string n2);

    /// @brief Returns a new string combining n1 and n2, replacing mismatching bits with a dash '-'
    /// @param n1 first term in binary form
    /// @param n2 second term in binary form
    std::string combineWithDash(std::string n1, std::string n2);

    struct TabularEntry
    {
        std::string term;
        bool isChecked;

        TabularEntry(std::string s) : term(s), isChecked(false) {}
    };

    /// @brief Function to sort terms by number of ones (in binary form) into a table column
    /// @param terms list of terms
    /// @returns Returns a vector of vectors of entries
    std::vector<std::vector<qm::TabularEntry>> sortTermsByOnes(const std::unordered_map<std::string, std::string>& terms);

    std::vector<std::vector<qm::TabularEntry>> generateNextColumn(std::vector<std::vector<TabularEntry>>& column);

    /// @brief Algorithm for finding prime implicants of logic expression
    /// @param terms map minterms and their binary representation
    /// @return Returns a list of prime implicants in their binary form
    std::set<std::string> findPrimeImplicants(const std::unordered_map<std::string, std::string>& terms);

}

#endif