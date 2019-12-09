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

class Object
{
   std::string name;
   std::shared_ptr<Object> DirectOrbit;

public:
   Object() 
   {}

   Object(const std::string& InName) :
      name(InName)
   {}

   void SetNewDirectOrbit(const std::shared_ptr<Object>& NewOrbit)
   {
      DirectOrbit = NewOrbit;
   }

   int GetAllOrbitsCount()
   {
      if (DirectOrbit)
      {
         return 1 + DirectOrbit->GetAllOrbitsCount();
      }
      else
      {
         return 0;
      }
   }

   void GetListOfIndirectOrbits(std::vector<std::string>& outResult)
   {
      if (DirectOrbit)
      {
         outResult.push_back(DirectOrbit->name);
         DirectOrbit->GetListOfIndirectOrbits(outResult);
      }
   }
};

int main() 
{
   std::ifstream infile("input.txt");

   std::map<std::string, std::shared_ptr<Object>> Objects;

   std::string InputLine;
   while (infile >> InputLine)
   {
      const size_t delimpos = InputLine.find(')');
      std::string A = InputLine.substr(0, delimpos);
      std::string B = InputLine.substr(delimpos + 1 , InputLine.length() - delimpos);

      if (Objects.find(A) == Objects.end())
      {
         Objects[A] = std::make_shared<Object>(A);
      }
      if (Objects.find(B) == Objects.end())
      {
         Objects[B] = std::make_shared<Object>(B);
      }

      Objects[B]->SetNewDirectOrbit(Objects[A]);
   }
   
   int Orbits = 0;
   for (auto Object : Objects)
   {
      Orbits += Object.second->GetAllOrbitsCount();
   }

   std::vector<std::string> ListYOU;
   Objects["YOU"]->GetListOfIndirectOrbits(ListYOU);

   std::vector<std::string> ListSAN;
   Objects["SAN"]->GetListOfIndirectOrbits(ListSAN);

   int YOUDistance = 0;
   int SANDistance = 0;
   for (const std::string YouOrbit : ListYOU)
   {
      auto iter = std::find(ListSAN.begin(), ListSAN.end(), YouOrbit);
      if (iter != ListSAN.end())
      {
         SANDistance = std::distance(ListSAN.begin(), iter);
         break;
      }
      YOUDistance++;
   }

   const int ResultDistance = YOUDistance + SANDistance;

   std::cout << "Sum of Orbits: " << Orbits << '\n';
   std::cout << "Distance from YOU to SAN: " << ResultDistance << '\n';

   return 0;
}
