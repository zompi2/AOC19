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

void RunIntCode(std::vector<long long> data, std::function<void(long long&)>& inputFunc, std::function<void(long long)>& outputFunc)
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

      int mode = (int)(data[idx]) / 100;

      const int A = mode / 100; mode -= A*100;
      const int B = mode / 10; mode -= B*10;
      const int C = mode;

      static auto GetValue = [&](int inMode, int inValId) -> long long&
      {
         switch(inMode)
         {
            case 0:
               return data[(int)(data[idx+inValId])];
            case 1:
               return data[idx+inValId];
            case 2:
               return data[relativeBase+(int)(data[idx+inValId])];
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
         inputFunc(GetValue(C,1));
         idx += 2;
      }
      else if (OPCode == 4)
      {
         const long long val = GetValue(C, 1);
         outputFunc(val);
         idx += 2;
      }
      else if (OPCode == 5 || OPCode == 6)
      {
         const long long val1 = GetValue(C, 1);
         const long long val2 = GetValue(B, 2);

         if ((OPCode == 5 && val1 != 0) || (OPCode == 6 && val1 == 0))
         {
            idx = (int)val2;
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
         relativeBase += (int)val;
         idx += 2;
      }
   }
}

class Point
{
public:
   int x = 0;
   int y = 0;

   Point() {}
   Point(int inX, int inY) : x(inX), y(inY) {}

   bool operator<(const Point& other) const
   {
      if (y != other.y)
      {
         return (y < other.y);
      } 
      return (x < other.x);
   }
};

class Tile
{
public:
   
   Point pos;
   int id = 0;

   Tile() {}
   Tile(Point inPos, int inId) : pos(inPos), id(inId) {}
};

int main() 
{
   std::ifstream infile("input.txt");   

   std::vector<long long> data;
   std::string value;
   while (std::getline(infile, value, ','))
   {
      data.push_back(std::stoll(value));
   }
   data.insert(data.end(), 10000, 0);

   data[0] = 2;

   int outIdx = 0;

   int outX = 0;
   int outY = 0;
   int outId = 0;

   int numOfBlockTiles = 0;
   int scores = 0;

   std::map<Point, Tile> tiles;

   int ScreenW = -1;
   int ScreenH = -1;

   const bool bUseBot = true;

   std::function<void(long long&)> inputFunc = [&](long long& in)
   {
      if (ScreenW == -1 && ScreenH == -1)
      {
         ScreenW = (*std::prev(tiles.end())).first.x;
         ScreenH = (*std::prev(tiles.end())).first.y;
      }

      std::cout << "\033[1;1H";
      std::cout << "SCORE: " << scores << '\n';

      Point palletPos;
      Point ballPos;

      auto Iter = tiles.begin();
      for (int y = 0; y <= ScreenH; y++)
      {
         for (int x = 0; x <= ScreenW; x++)
         {
            Tile& t = (*Iter).second;
            if (x != t.pos.x && y != t.pos.y)
            {
               assert(false);
            }

            switch(t.id)
            {
               case 0:
                  std::cout << ' ';
                  break;
               case 1:
                  std::cout << '|';
                  break;
               case 2:
                  std::cout << '#';
                  break;
               case 3:
                  std::cout << '_';
                  palletPos = Point(x,y);
                  break;
               case 4:
                  std::cout << 'o';
                  ballPos = Point(x,y);
                  break;
               default:
                  assert(false);
            }
            Iter++;
         }
         std::cout << '\n';
      }

      std::cout << "Ball (" << ballPos.x << "," << ballPos.y << ")\n";
      std::cout << "Palete (" << palletPos.x << "," << palletPos.y << ")\n";

      if (bUseBot)
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(1));

         if (palletPos.x < ballPos.x)
         {
            in = 1;
         }
         else if (palletPos.x > ballPos.x)
         {
            in = -1;
         }
         else
         {
            in = 0;
         }
      }
      else
      {
         std::cin >> in;
         std::cin.ignore();
      }
   };

   std::function<void(long long)> outputFunc = [&](long long out)
   {
      switch(outIdx)
      {
         case 0:
            outX = out;
            outIdx++;
            break;
         case 1:
            outY = out;
            outIdx++;
            break;
         case 2:
            outId = out;
            outIdx = 0;
            
            {
               if (outId == 2)
               {
                  numOfBlockTiles++;
               }

               if (outX == -1 && outY == 0)
               {
                  if (outId > scores)
                  {
                     scores = outId;
                  }
               }
               else
               {
                  tiles[Point(outX, outY)] = Tile(Point(outX, outY), outId);
               }
            }
      }
   };

   RunIntCode(data, inputFunc, outputFunc);

   std::cout << "FINAL SCORE: " << scores << '\n';

   return 0;
}