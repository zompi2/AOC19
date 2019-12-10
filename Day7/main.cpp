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

int RunIntCode(std::vector<int> data, int phase, int signal)
{
   bool bFinished = false;
   int idx = 0;
   bool phasePassed = false;

   while (bFinished == false)
   {
      const int OPCode = data[idx] % 100;

      if (OPCode == 99)
      {
         bFinished = true;
         break;
      }

      int immediateMode = data[idx] / 100;

      const int A = immediateMode / 100; immediateMode -= A*100;
      const int B = immediateMode / 10; immediateMode -= B*10;
      const int C = immediateMode;

      if (OPCode == 1 || OPCode == 2)
      {
         const int val1 = C == 0 ? data[data[idx+1]] : data[idx+1];
         const int val2 = B == 0 ? data[data[idx+2]] : data[idx+2];
         int& result =  data[data[idx+3]];

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
         if (phasePassed == false)
         {
            phasePassed = true;
            data[data[idx+1]] = phase;
         }
         else
         {
            data[data[idx+1]] = signal;
         }
    
         idx += 2;
      }
      else if (OPCode == 4)
      {
         const int val = C == 0 ? data[data[idx+1]] : data[idx+1];
         return val;
      }
      else if (OPCode == 5 || OPCode == 6)
      {
         const int val1 = C == 0 ? data[data[idx+1]] : data[idx+1];
         const int val2 = B == 0 ? data[data[idx+2]] : data[idx+2];

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
         const int val1 = C == 0 ? data[data[idx+1]] : data[idx+1];
         const int val2 = B == 0 ? data[data[idx+2]] : data[idx+2];
         int& result =  data[data[idx+3]];

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
   } 
}

const int phaseNums = 5;

int GetResultFromPhases(std::vector<int> data, const std::array<int, phaseNums>& inPhases)
{
   int previousSignal = 0;
   for (int i=0; i<5; i++)
   {
      previousSignal = RunIntCode(data, inPhases[i], previousSignal);
   }
   return previousSignal;
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

   std::array<int, phaseNums> phases {0,1,2,3,4};
   std::array<int, phaseNums> originPhases = phases;

   int swap = 0;
   int originSwap = 0;
   int maxResult = 0;

   while(true)
   {
      int Result = GetResultFromPhases(data, phases);
      if (Result > maxResult)
      {
         maxResult = Result;
      }

      if (swap + 1 == phaseNums)
      {
         std::swap(phases[swap], phases[0]);
      }
      else
      {
         std::swap(phases[swap], phases[swap+1]);
      }
      swap++;

      if (swap == phaseNums)
      {
         swap = 0;
      }

      if (phases == originPhases)
      {
         originSwap++;
         if (originSwap == phaseNums - 3)
         {
            break;
         }  
         swap = originSwap;
      }
   }   

   std::cout << "Result: " << maxResult << '\n';

   return 0;
}