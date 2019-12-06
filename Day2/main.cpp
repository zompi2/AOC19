#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <assert.h>

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

int CalcResult(int noun, int verb, std::vector<int> data)
{
    data[1] = noun;
    data[2] = verb;

    bool bFinished = false;
    int idx = 0;

    while (bFinished == false)
    {
        if (idx >= data.size())
        {
            assert("Invalid Id");
        }

        const int OPCode = data[idx];
        if (OPCode == 99)
        {
            bFinished = true;
            break;
        }
        else if (OPCode == 1 || OPCode == 2)
        {
            const int val1 = data[data[idx+1]];
            const int val2 = data[data[idx+2]];
            int& result =  data[data[idx+3]];

            if (OPCode == 1)
            {
                result = val1 + val2;
            }
            else
            {
                result = val1 * val2;
            }
        }
        else
        {
            assert("Invalid OPCode");
        }

        idx += 4;
    } 
    
    return data[0];
}

int main() 
{
    std::ifstream infile("input.txt");

    std::vector<int> data;
    std::string value;
    while (std::getline(infile, value, ','))
    {
        data.push_back(std::stoi(value));
    }

    [&]{
        for (int noun = 0; noun < 100; noun++)
        {
            for (int verb = 0; verb < 100; verb++)
            {
                int Result = CalcResult(noun, verb, data);
                if (Result == 19690720)
                {
                    std::cout << "Result: " << noun << ' ' << verb << '\n';
                    return;
                }
            }
        }
    }();

    return 0;
}
