#ifndef QUINE_MCCLUSKEY
#define QUINE_MCCLUSKEY

#include <string>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <sstream>
#include <iomanip>

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
    std::vector<std::vector<qm::TabularEntry>> sortTermsByOnes(std::unordered_map<std::string, std::string>& terms);

    std::vector<std::vector<qm::TabularEntry>> generateNextColumn(std::vector<std::vector<TabularEntry>>& column);

    /// @brief Algorithm for finding prime implicants of logic expression
    /// @param terms map minterms and their binary representation
    /// @return Returns a list of prime implicants in their binary form
    std::set<std::string> findPrimeImplicants(std::unordered_map<std::string, std::string>& terms);

    /// @brief Function to check whether given minterm is covered by the prime implicant
    /// @return Returns true if the minterm is covered by the prime implicant, false otherwise
    bool isCovered(const std::string& primeImplicant, const std::string& minterm);

    struct PrimeImplicantTable
        {
            std::vector<std::vector<int>> table;
            int rows = 0;
            int cols = 0;

            PrimeImplicantTable(std::vector<std::vector<int>> t0, int rows0, int cols0) : table(t0), rows(rows0), cols(cols0) {};

            PrimeImplicantTable(const PrimeImplicantTable& t1) {table = t1.table; rows = t1.rows; cols = t1.cols; }

            void crossOutColumn(int col)
            {
                //table.erase(table.begin() + col);
                table[col][table[col].size() - 1] = -1;
                cols--;
            }

            void crossOutRow(int row)
            {
                /*
                for (int i = 0; i < cols; i++)
                {
                    table[i].erase(table[i].begin() + row);
                }*/
                table[table.size() - 1][row] = -1;
                rows--;
            }

            bool isCrossedOutRow(int row) const
            {
                if (table[table.size() -1][row] == -1)
                    return true;

                return false;
            }

            bool isCrossedOutCol(int col) const
            {
                if (table[col][table[col].size() - 1] == -1)
                    return true;

                return false;
            }

            int sumCol(int col) const
            {
                int res = 0;
                for (int i = 0; i < table[col].size() - 1; i++)
                {
                    if (!isCrossedOutRow(i))
                        res += table[col][i];
                }
                return res;
            }

            int findColLeastOnes() const
            {
                int least;
                for (int i = 0; i < table.size(); i++)
                {
                    if (table[i][table[0].size() - 1] != -1)
                    {
                        least = i;
                        i = table.size();
                    }
                }

                for (int i = least; i < table.size() - 1; i++)
                {
                    if ((sumCol(least) > sumCol(i)) && !isCrossedOutCol(i))
                        least = i;
                }
                return least;
            }

            int findOne(int col) const
            {
                for (int i = 0; i < table[col].size() - 1; i++)
                {
                    if (table[col][i] == 1 && !isCrossedOutRow(i))
                        return i;
                }

                return -1;
            }

            int getMinterm(int row) const
            {
                return table[table.size() - 1][row];
            }

            std::vector<int> getRemainingMinterms() const
            {
                std::vector<int> res;
                for (int i = 0; i < table[0].size() - 1; i++)
                {
                    int tmp = table[table.size() - 1][i];
                    if (tmp != -1)
                        res.push_back(tmp);
                }
                
                return res;
            }

            void printTable()
            {
                for (int j = 0; j < table[0].size(); j++)
                {
                    for (int i = 0; i < table.size(); i++)
                    {
                        std::cout << std::setw(2);
                        std::cout << table[i][j] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        };

    /// @brief Creates a Prime Implicant Table, each entry refers to a minterm
    PrimeImplicantTable createPrimeImplicantTable(const std::set<std::string>& minterms, 
                                                          const std::vector<std::string>& primeImplicants, 
                                                          std::unordered_map<std::string, std::string>& binary);




    /// @brief A naive implementation of Knuth's Algorithm X
    PrimeImplicantTable findExactCover(PrimeImplicantTable& table, std::vector<int>& res, int seedCol = -1);
    
    void printMintermsRaw(const std::set<std::string>& primeImplicants, const std::vector<int>& indices);
    void printMinterms(const std::set<std::string>& primeImplicants, const std::vector<int>& indices);

}

#endif