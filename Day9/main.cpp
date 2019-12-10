#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <assert.h>
#include <set>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
#include <array>

void RunIntCode(std::vector<long long> data)
{
   int idx = 0;
   int relativeBase = 0;

   while (true)
   {
      const int OPCode = data[idx] % 100;

      if (OPCode == 99)
      {
         return;
      }

      int mode = data[idx] / 100;

      const int A = mode / 100; mode -= A*100;
      const int B = mode / 10; mode -= B*10;
      const int C = mode;

      static auto GetValue = [&](int inMode, int inValId) -> long long&
      {
         switch(inMode)
         {
            case 0:
               return data[data[idx+inValId]];
            case 1:
               return data[idx+inValId];
            case 2:
               return data[relativeBase+data[idx+inValId]];
         }
         return data[0];
      };

      if (OPCode == 1 || OPCode == 2)
      {
         const long long val1 = GetValue(C, 1);
         const long long val2 = GetValue(B, 2);
         long long& result = GetValue(A, 3);

         if (OPCode == 1)
         {
            result = val1 + val2;
         }
         else if (OPCode == 2)
         {
            result = val1 * val2;
         }
         idx += 4;
      }
      else if (OPCode == 3)
      {
         std::cout << "Please give input: ";
         std::cin >> GetValue(C, 1);
         std::cin.ignore();
         std::cout << '\n';
         idx += 2;
      }
      else if (OPCode == 4)
      {
         const long long val = GetValue(C, 1);
         std::cout << "Output: " << val << '\n';
         idx += 2;
      }
      else if (OPCode == 5 || OPCode == 6)
      {
         const long long val1 = GetValue(C, 1);
         const long long val2 = GetValue(B, 2);

         if ((OPCode == 5 && val1 != 0) || (OPCode == 6 && val1 == 0))
         {
            idx = val2;
         }
         else
         {
            idx += 3;
         }
      }
      else if (OPCode == 7 || OPCode == 8)
      {
         const long long val1 = GetValue(C, 1);
         const long long val2 = GetValue(B, 2);
         long long& result = GetValue(A, 3);

         if (OPCode == 7)
         {
            result = (val1 < val2) ? 1 : 0;
         }
         else if (OPCode == 8)
         {
            result = (val1 == val2) ? 1 : 0;
         }
         idx += 4;
      }
      else if (OPCode == 9)
      {
         const long long val = GetValue(C, 1);
         relativeBase += val;
         idx += 2;
      }
   }
}

int main() 
{
   std::ifstream infile("input.txt");   
   std::vector<long long> data;
   std::string value;
   while (std::getline(infile, value, ','))
   {
      data.push_back(std::stoll(value));
   }

   RunIntCode(data);

   return 0;
}