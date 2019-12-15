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
#include <cmath>
#include <chrono>
#include <thread>

std::vector<std::string> Split(const std::string& s, char delimiter)
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

class Reactor
{
public:
   std::map<std::string, int> Requires;
   std::pair<std::string, int> Produces;
};
std::vector<Reactor> reactors;
std::map<std::string, int> partsRequired;
int oreRequired = 0;

bool IsActeptingOreOnly(const std::string& name)
{
   for (auto& r : reactors)
   {
      if (r.Produces.first == name)
      {
         if (r.Requires.size() == 1)
         {
            if (r.Requires.find("ORE") != r.Requires.end())
            {
               return true;
            }
         }
      }
   }
  
   return false;
}

void AddOrInrease(std::map<std::string, int>& map, const std::string& what, int amount)
{
   if (amount > 0)
   {
      if (map.find(what) != map.end())
      {
         map[what] += amount;
      }
      else
      {
         map[what] = amount;
      }
   }
}

int main() 
{
   std::ifstream infile("input.txt");   
   std::string s;

   // PARSE INPUT TO REACTORS
   while(std::getline(infile, s))
   {
      reactors.push_back(Reactor());
      Reactor* currentReactor = &reactors.back();

      std::size_t sep = s.find(" => ");
      std::vector<std::string> reqs = Split(s.substr(0, sep), ',');
      for (std::string& req : reqs)
      {
         std::vector<std::string> reqval = Split(req, ' ');
         if (reqval[0] == "")
         {
            reqval.erase(reqval.begin());
         }
         currentReactor->Requires[reqval[1]] = std::atoi(reqval[0].c_str());
      }

      std::vector<std::string> outs = Split(s.substr(sep + 4, s.length() - sep), ' ');
      currentReactor->Produces.first = outs[1];
      currentReactor->Produces.second = std::atoi(outs[0].c_str());
   }

   partsRequired["FUEL"] = 1;

   bool bPurePartsOnly = false;
   while(bPurePartsOnly == false)
   {
      bPurePartsOnly = true;
      std::map<std::string, int> subParts;
      for (auto& part : partsRequired)
      {
         if (IsActeptingOreOnly(part.first) == false)
         {
            if (part.second > 0)
            {
               bPurePartsOnly = false;
               for (auto& r : reactors)
               {
                  if (r.Produces.first == part.first)
                  {
                     int c = (int)std::ceil((float)part.second / r.Produces.second);
                     for (auto& req : r.Requires)
                     {
                        AddOrInrease(subParts, req.first, req.second * c);
                     }

                     part.second -= r.Produces.second * c;      
                     break;
                  }
               }  
            }
         }
      }

      for (auto& sr : subParts)
      {
         AddOrInrease(partsRequired, sr.first, sr.second);
      }
   }

   oreRequired = 0;
   for (auto& part : partsRequired)
   {
      for (auto&r : reactors)
      {
         if (part.first == r.Produces.first)
         {
            int c = (int)std::ceil((float)part.second / r.Produces.second);
            int x = c * r.Requires["ORE"];
            oreRequired += c * r.Requires["ORE"];
            break;
         }
      }
   }
   
   std::cout << "Ore used: " << oreRequired << '\n';

   return 0;
}