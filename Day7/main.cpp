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

class Amp
{
public:   
   std::vector<int> data;
   int phase;
   bool phaseSet = false;
   int idx = 0;
};

// returns true if will still continue it's job.
bool RunIntCode(std::shared_ptr<Amp>& amp, int& signal)
{
   int& idx = amp->idx;
   std::vector<int>& data = amp->data;
   int& phase = amp->phase;
   bool& phaseSet = amp->phaseSet;

   while (true)
   {
      const int OPCode = data[idx] % 100;

      if (OPCode == 99)
      {
         return false;
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
         if (phaseSet == false)
         {
            phaseSet = true;
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
         signal = C == 0 ? data[data[idx+1]] : data[idx+1];
         idx += 2;
         return true;
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

   return true;
}

const int phaseNums = 5;

int GetResultFromPhases(std::vector<int> data, int* inPhases)
{
   std::vector<std::shared_ptr<Amp>> Amps;
   for (int i=0; i<phaseNums; i++)
   {
      std::shared_ptr<Amp> tmpAmp(new Amp());
      tmpAmp->data = data;
      tmpAmp->phase = inPhases[i];
      Amps.push_back(tmpAmp);
   }

   int previousSignal = 0;
   int i = 0;

   while (true)
   {
      if (RunIntCode(Amps[i], previousSignal) == false)
      {
         break;
      }
      i++;
      if (i == phaseNums)
      {
         i = 0;
      }
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

   int phases[5] = {5,5,5,5,5};

   bool bIsRunning = true;
   int maxResult = 0;
   while(bIsRunning)
   {
      std::set<int> Tmp;
      for (int i = 0; i < 5; i++)
      {
         Tmp.insert(phases[i]);
      }
      if (Tmp.size() == 5)
      {
         int Result = GetResultFromPhases(data, phases);
         if (Result > maxResult)
         {
            maxResult = Result;
         }
      }

      for (int i = 4; i >= 0; i--)
      {
         if (phases[i] == 9)
         {
            if (i==0)
            {
               bIsRunning = false;
               break;
            }
            phases[i] = 5;
         }
         else
         {
            phases[i] += 1;
            break;
         }
      }
   }

   std::cout << "Result: " << maxResult << '\n';
   
   return 0;
}