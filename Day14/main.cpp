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
std::map<std::string, int> spareParts;
int oreRequired = 0;

void AddSparePart(const std::string& what, int amount)
{
   if (amount > 0)
   {
      if (spareParts.find(what) != spareParts.end())
      {
         spareParts[what] += amount;
      }
      else
      {
         spareParts[what] = amount;
      }
   }
}

void Produce(const std::string& what, int amount)
{
   if (what == "ORE")
   {
      oreRequired += amount;
      return;
   }

   for (auto& r : reactors)
   {
      if (r.Produces.first == what)
      {
         for (auto& req : r.Requires)
         {
            int needToProduceRatio = (int)std::ceil((float)amount / r.Produces.second);
            int requires = req.second * needToProduceRatio;
            AddSparePart(what, r.Produces.second - amount);
            Produce(req.first, requires);
         }
      }
   }  
}

int main() 
{
   std::ifstream infile("input.txt");   
   std::string s;

   int expect = 0;


   Reactor* reactorProducingFuel = nullptr;
   std::vector<Reactor*> reactorsAcceptingOre;

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

   for (auto& r : reactors)
   {
      if (r.Produces.first == "FUEL")
      {
         reactorProducingFuel = &r;
      }
      else
      {
         for (auto& req : r.Requires)
         {
            if (req.first == "ORE")
            {
               reactorsAcceptingOre.push_back(&r);
            }
         }
      }
   }

   Produce("FUEL", 1);

   for (auto& spare : spareParts)
   {
      for (auto& r : reactors)
      {
         if (r.Produces.first == spare.first)
         {
            int producesCount = spare.second / r.Produces.second;
            oreRequired -= producesCount*r.Produces.second;
            break;
         }
      }
   }

   std::cout << "Ore used: " << oreRequired << '\n';

   return 0;
}