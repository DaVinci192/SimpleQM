#include "quine-mccluskey.hpp"
#include <algorithm>
#include <iomanip>

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

std::vector<std::vector<qm::TabularEntry>> qm::sortTermsByOnes(std::unordered_map<std::string, std::string>& terms)
{
    std::vector<std::vector<qm::TabularEntry>> res;
    std::unordered_map<int, int> sortHelper;
    
    
    for (auto i = terms.begin(); i != terms.end(); i++) // input each row into table
    {
        qm::TabularEntry entry = qm::TabularEntry(i->second);
        int numOnes = qm::countOnes(entry.term);

        if (sortHelper.find(numOnes) == sortHelper.end())
        {
            std::vector<qm::TabularEntry> firstTerm;
            firstTerm.push_back(entry);
            res.push_back(firstTerm);
            sortHelper[numOnes] = res.size() - 1;
        }
        else
        {
            res[sortHelper[numOnes]].push_back(entry);
        }
    }

    std::sort(res.begin(), res.end(), /* sort the rows by number of ones (in ascending order) */
              [](const std::vector<qm::TabularEntry>& lhs, const std::vector<qm::TabularEntry>& rhs) ->
              bool { return qm::countOnes(lhs[0].term) < qm::countOnes(rhs[0].term); });

    return res;
}

std::vector<std::vector<qm::TabularEntry>> qm::generateNextColumn(std::vector<std::vector<TabularEntry>>& column)
{
    /* Algorithm
    - Compare each TabularEntry in each row with each TabularEntry in the subsequent row
    - if mismatched bits == 1
        -> both TabularEntry.checked = true
        -> combineWithDash and append result to new row in res 
    - else
        -> move on 
    return res
    */
   std::vector<std::vector<qm::TabularEntry>> res(column.size() - 1); // initialize res with (column.size()-1) rows

   for (int i = 0; i < column.size() - 1; i++) // iterate over each row, up until the second-to-last one
   {
       for (int j = 0; j < column[i].size(); j++) // iterate over each TableEntry in each row
       {
           TabularEntry *curr = &column[i][j];
           for (int k = 0; k < column[i+1].size(); k++) // iterate over each TableEntry in the next row
           {
               TabularEntry *comp = &column[i+1][k];
               if (mismatchedBits(curr->term, comp->term) == 1)
               {
                    curr->isChecked = true;
                    comp->isChecked = true;
                    
                    TabularEntry tmp = TabularEntry(combineWithDash(curr->term, comp->term));
                    res[i].push_back(tmp);
               }
           }
       }
   }
   return res;
}

std::set<std::string> qm::findPrimeImplicants(std::unordered_map<std::string, std::string>& terms)
{
    std::set<std::string> res;
    std::vector<std::vector<std::vector<qm::TabularEntry>>> table; ///< list of columns
    std::vector<std::vector<qm::TabularEntry>> c0 = qm::sortTermsByOnes(terms); ///< sorted list of terms by number of ones

    table.push_back(c0);

    do
    {
        table.push_back(qm::generateNextColumn(table[table.size()-1]));
    } while (table[table.size()-1].size() > 0);

    for (int i = 0; i < table.size(); i++)
    {
        for (int j = 0; j < table[i].size(); j++)
        {
            for (int k = 0; k < table[i][j].size(); k++)
            {
                if (!table[i][j][k].isChecked)
                    res.insert(table[i][j][k].term); // set takes care of dupliate terms
            }
        }
    }

    return res;
}

bool qm::isCovered(const std::string& primeImplicant, const std::string& minterm)
{
    for (int i = 0; i < primeImplicant.size(); i++)
    {
        if ((primeImplicant[i] != '-') && (primeImplicant[i] != minterm[i]))
            return false;
    }
    return true;
}

qm::PrimeImplicantTable qm::createPrimeImplicantTable(const std::set<std::string>& primeImplicants, 
                                                          const std::vector<std::string>& minterms, 
                                                          std::unordered_map<std::string, std::string>& binaryMap)
{
    std::vector<std::vector<int>> res;

    for (int i = 0; i < minterms.size(); i++)
    {
        std::vector<int> tmp;
        for (std::set<std::string>::iterator j = primeImplicants.begin(); j != primeImplicants.end(); j++)
        {
            (qm::isCovered(*j, binaryMap[minterms[i]])) ? tmp.push_back(1) : tmp.push_back(0);
        }
        tmp.push_back(std::stoi(minterms[i]));
        res.push_back(tmp);
    }

    std::vector<int> primeImplicantsInt;
    for (int i = 0; i < primeImplicants.size(); i++)
    {
        primeImplicantsInt.push_back(i);
    }
    primeImplicantsInt.push_back(-1);
    res.push_back(primeImplicantsInt);

    qm::PrimeImplicantTable t{res, int(primeImplicants.size()), int(minterms.size())};

    return t;
}


qm::PrimeImplicantTable qm::findExactCover(qm::PrimeImplicantTable& piTable, std::vector<int>& res, int seedCol)
{
    if (piTable.cols == 0)
        return piTable;
    

    int col; 

    if (seedCol != -1)
        col = seedCol;
    else
        col = piTable.findColLeastOnes(); 
    
    
    if (piTable.sumCol(col) == 0 && !piTable.isCrossedOutCol(col))
        return piTable;

    int row = piTable.findOne(col);
        
    res.push_back(piTable.getMinterm(row));

    for (int i = 0; i < piTable.table.size() - 1; i++) // iterate over cols
    {
        if (piTable.table[i][row] == 1 && !piTable.isCrossedOutCol(i))
        {
            piTable.crossOutColumn(i);
        }
    }
    piTable.crossOutRow(row);

    return qm::findExactCover(piTable, res);
}

void qm::printMintermsRaw(const std::set<std::string>& primeImplicants, const std::vector<int>& indices)
{
    std::vector<std::string> res;

    for (const auto& implicant : primeImplicants)
    {
        res.push_back(implicant);
    }

    for (int i = 0; i < indices.size(); i++)
    {
        std::cout << res[indices[i]] << std::endl;
    }
}

void qm::printMinterms(const std::set<std::string>& primeImplicants, const std::vector<int>& indices)
{

    std::vector<std::string> res;

    for (const auto& implicant : primeImplicants)
    {
        res.push_back(implicant);
    }

    for (int i = 0; i < indices.size(); i++) // iterate over each prime implicant
    {
        for (int j = 0; j < res[indices[i]].size(); j++) // iterate over each character in string
        {
            if (res[indices[i]][j] != '-')
            {
                std::cout << char(int('A') + j);
                if (res[indices[i]][j] == '0')
                    std::cout << '\'';
                    
            }
        }
        std::cout << std::endl;
    }   

}

