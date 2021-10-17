#include "quine-mccluskey.hpp"

void qm::getInput(int& numVariables, std::vector<std::string>& minterms, std::vector<std::string>& dontcares)
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

std::unordered_map<std::string, std::string> qm::mapMintermsToBinary(const std::vector<std::string>& minterms, const std::vector<std::string>& dontcares, const int numVariables)
{
    std::unordered_map<std::string, std::string> toBinary;

    for (int i = 0; i < minterms.size(); i++)
    {
        int tmpInt = std::stoi(minterms[i]);
        std::string binaryString = "";
        
        for (int j = 0; j < numVariables; j++)
        {
            int bit = (tmpInt >> j) & 1 ? 1 : 0;
            std::string stringBit = std::to_string(bit);
            binaryString.insert(0, stringBit);
        }
        toBinary[minterms[i]] = binaryString;
    }

    for (int i = 0; i < dontcares.size(); i++)
    {
        int32_t tmpInt = std::stoi(dontcares[i]);
        std::string binaryString = "";

        for (int j = 0; j < numVariables; j++)
        {
            int bit = (tmpInt >> j) & 1 ? 1 : 0;
            std::string stringBit = std::to_string(bit);
            binaryString.insert(0, stringBit);
        }
        toBinary[dontcares[i]] = binaryString;
    }
    
    return toBinary;
}

int qm::countOnes(std::string n)
{
    int res = 0;
    for (int i = 0; i < n.size(); i++)
    {
        if (n[i] == '1')
            res++;
    }
    return res;
}

int qm::mismatchedBits(std::string n1, std::string n2)
{
    int res = 0;
    for (int i = 0; i < n1.size(); i++)
    {
        if (n1[i] != n2[i])
            res++;
    }
    return res;
}

std::string qm::combineWithDash(std::string n1, std::string n2)
{
    std::string res = "";
    for (int i = 0; i < n1.size(); i++)
    {
        if (n1[i] != n2[i])
            res.push_back('-');
        else
            res.push_back(n1[i]);
    }
    return res;
}


