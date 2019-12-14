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


int main() 
{
   std::ifstream infile("input.txt");   

   std::vector<long long> data;
   std::string value;
   while (std::getline(infile, value, ','))
   {
      data.push_back(std::stoll(value));
   }
   //data.insert(data.end(), 10000, 0);

   return 0;
}