#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <assert.h>


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

int main() 
{
    std::ifstream infile("input.txt");

    return 0;
}
