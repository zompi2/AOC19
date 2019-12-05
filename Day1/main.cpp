#include <iostream>
#include <fstream>

int CalcFuel(const int iMass)
{
    const int result = iMass / 3 - 2;
    return result <= 0 ? 0 : result + CalcFuel(result);
}

int main() 
{
    std::ifstream infile("input.txt");
    int x, acc = 0;
    while (infile >> x)
    {
       acc += CalcFuel(x);
    }

    std::cout << "Result: " << acc << '\n';

    {
        std::cout << "Press any key to continue...\n";
        getchar();
    }

    return 0;
}
