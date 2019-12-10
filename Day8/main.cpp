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

int main() 
{
   std::ifstream infile("input.txt");
   std::vector<char> data;
   std::copy(  std::istream_iterator<char>(infile), 
               std::istream_iterator<char>(), 
               std::back_insert_iterator(data));

   const int dataSize = data.size();
   const int W = 25;
   const int H = 6;
   const int imgSize = W*H;
   const int L = dataSize / imgSize;

   int smallestZeroes = -1;
   int LWithSmallestZeroes = -1;
   for (int i=0; i<L; i++)
   {
      const int offset = i * imgSize;
      int zeroes = std::count(data.begin() + offset, data.begin() + offset + imgSize, '0');
      if (smallestZeroes == -1 || zeroes < smallestZeroes)
      {
         smallestZeroes = zeroes;
         LWithSmallestZeroes = i;
      }
   }

   const int offset = LWithSmallestZeroes * imgSize;
   int ones = std::count(data.begin() + offset, data.begin() + offset + imgSize, '1');
   int twos = std::count(data.begin() + offset, data.begin() + offset + imgSize, '2');

   std::cout << "Result: " << ones*twos << '\n';

   std::vector<char> final;
   final.reserve(imgSize);

   for (int i=0; i<imgSize; i++)
   {
      for (int l=0; l<L; l++)
      {
         const char pixel = data[i + l*imgSize];
         if (pixel != '2')
         {
            final.push_back(pixel == '1' ? '*' : ' ');
            break;
         }
      }
   }

   for (int h = 0; h < H; h++)
   {
      for (int w = 0; w < W; w++)
      {
         std::cout<<final[h*W+w];
      }
      std::cout<<'\n';
   }

   return 0;
}