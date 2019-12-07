#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <assert.h>
#include <algorithm>

void CollectDigits(std::vector<int>& outDigits, int inNumber) 
{
    if (inNumber > 9) 
    {
        CollectDigits(outDigits, inNumber / 10);
    }
    outDigits.push_back(inNumber % 10);
}

class Number
{
    int Digits[6];
 
 public:
    Number(const int InitNumber)
    {
        std::vector<int> TempDigits;
        CollectDigits(TempDigits, InitNumber);
        assert(TempDigits.size() == 6);

        std::copy(TempDigits.begin(), TempDigits.end(), Digits);
    }

    void Increase()
    {
        for (int i = 5; i >= 0; i--)
        {
            if (Digits[i] == 9)
            {
                Digits[i] = 0;
            }
            else
            {
                Digits[i]++;
                break;
            }
        }
    }

    void Print()
    {
        for (int i = 0; i < 6; i++)
        {
            std::cout << Digits[i];
        }
        std::cout << '\n';
    }

    bool Check()
    {
        bool bFoundPair = false;
        bool bFoundPairInPreviousCheck = false;
        bool bFoundPairChecked = false;

        for (int i=1; i<6; i++)
        {
            if (Digits[i] < Digits[i-1])
            {
                // next number is lesser. Failed.
                return false;
            }

            if (Digits[i] == Digits[i-1])
            {
                if (bFoundPairInPreviousCheck)
                {
                    // Pair was found in previous check it means it is a triplet or more.
                    // This is not a pair!
                    bFoundPair = false;
                }
                else
                {
                    // found pair
                    bFoundPair = true;
                    bFoundPairInPreviousCheck = true;
                    if (i == 5)
                    {
                        // this is the last digit and it is a pair.
                        bFoundPairChecked = true;
                    }
                }
            }
            else
            {
                if (bFoundPair)
                {
                    // There was a pair previously, which means we have at least one pair!
                    bFoundPairChecked = true;
                }
                bFoundPairInPreviousCheck = false;
            }
        }

        return bFoundPairChecked;
    }
};

int main() 
{

    const int Begin = 273025;
    const int End = 767253;

    int PasswordsFound = 0;

    Number number(Begin);
    for (int i = Begin; i <= End; i++)
    {
        if (number.Check())
        {
            PasswordsFound++;
        }
        number.Increase();
    }

    std::cout << "Result: " << PasswordsFound << " \n";

    return 0;
}
