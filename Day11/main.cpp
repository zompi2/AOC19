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
         //std::cout << "Please give input: \n";
         inputFunc(GetValue(C,1));
         idx += 2;
      }
      else if (OPCode == 4)
      {
         const long long val = GetValue(C, 1);
         //std::cout << "Output: " << val << '\n';
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

std::string PosToString(int x, int y)
{
   return std::to_string(x) + 'x' + std::to_string(y);
}

class Pos
{

public:
   int x = 0;
   int y = 0;

   Pos() {}
   Pos(int inX, int inY) : x(inX), y(inY) {}

   std::string ToString()
   {
      return PosToString(x,y);
   }

  bool operator<(const Pos& other) const
  {
      if (y != other.y)
      {
         return (y < other.y);
      } 
      return (x < other.x);
   }
};

class Panel
{
public:

   Pos pos;
   int color = 0;

   Panel() {}
   Panel(int inX, int inY) : pos(inX, inY) {}
   Panel(const Pos& inPos) : pos(inPos) {}
};

const int RoboDirUP     = 0;
const int RoboDirRIGHT  = 1;
const int RoboDirDOWN   = 2;
const int RoboDirLEFT   = 3;

void RegisterPosition()
{

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

   data.insert(data.end(), 10000, 0);
         
   int outputId = 0;
   Pos currentPos = Pos(0,0);
   std::map<Pos, Panel> panels;
   panels[currentPos] = Panel(currentPos);
   Panel* currentPanel = &(panels[currentPos]);
   currentPanel->color = 1;
   int roboDir = RoboDirUP;

   int MaxX = 0;
   int MaxY = 0;

   std::function<void(long long&)> inputFunc = [&](long long& in)
   {
      in = currentPanel->color;
   };

   std::function<void(long long)> outputFunc = [&](long long out)
   {
      if (outputId == 0)
      {
         currentPanel->color = (int)out;
         outputId = 1;
      }
      else if (outputId == 1)
      {
         if (out == 1)
         {
            roboDir++;
            if (roboDir > 3)
            {
               roboDir = 0;
            }
         }
         else if (out == 0)
         {
            roboDir--;
            if (roboDir < 0)
            {
               roboDir = 3;
            }
         }

         switch(roboDir)
         {
            case RoboDirUP:
               currentPos.y--;
               break;
            case RoboDirRIGHT:
               currentPos.x++;
               break;
              case RoboDirDOWN:
               currentPos.y++;
               break;
            case RoboDirLEFT:
               currentPos.x--;
               break;
         }

         if (currentPos.x > MaxX)
         {
            MaxX = currentPos.x;
         }

         if (currentPos.y > MaxY)
         {
            MaxY = currentPos.y;
         }

         if (panels.find(currentPos) == panels.end())
         {
            panels[currentPos] = Panel(currentPos);
         }
         currentPanel = &(panels[currentPos]);
         
         outputId = 0;
      }
   };

   RunIntCode(data, inputFunc, outputFunc);

   std::cout << "Visited: " << panels.size() << " panels.\n";

   int PlateW = MaxX+1;
   int PlateH = MaxY+1;

   auto FirstPanelItr = panels.begin();
   for (int y = 0; y < PlateH; y++)
   {
      for (int x = 0; x < PlateW; x++)
      {
         auto panelFound = panels.find(Pos(x,y));
         if (panelFound != panels.end())
         {
            if ((*panelFound).second.color == 0)
            {
               std::cout << ' ';
            }
            else
            {
               std::cout << '#';
            }
         }
         else
         {
            std::cout << ' ';
         }
      }
      std::cout << '\n';
   }

   return 0;
}