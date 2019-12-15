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

class Room
{

public:

   Room() {}
   
   // -1 : unknown
   //  0 : wall
   //  1 : otherRoom
   //  2 : backtrackingRoom
   int dir[4] = {-1, -1, -1, -1};

   // Rooms
   // north (0), south (1), west (2), east (3)
   Room* otherRoom[4] = {nullptr, nullptr, nullptr, nullptr};

   bool hasOxygen = false;
   bool propagatedOxygen = false;
};

int GetMirrorPath(int in)
{
   switch(in)
   {
      case 0:
         return 1;
      case 1:
         return 0;
      case 2:
         return 3;
      case 3:
         return 2;
   }
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

   std::map<Point, Room*> rooms;
   Point currentPoint(0,0);
   Room* currentRoom = new Room();
   Room* firstRoom = currentRoom;
   Room* oxygenSystemRoom = nullptr;
   rooms[currentPoint] = currentRoom;
   bool oxygenSysemFound = false;

   int exploringPath = 0;
   std::function<void(long long&)> inputFunc = [&](long long& in)
   {
      if (currentRoom == firstRoom && oxygenSysemFound)
      {
         in = 0;
         return;
      }

      for (int i=0; i<4; i++)
      {
         if (currentRoom->dir[i] == -1)
         {
            exploringPath = i;
            in = exploringPath + 1;
            return;
         }
      }

      for (int i=0; i<4; i++)
      {
         if (currentRoom->dir[i] == 2)
         {
            exploringPath = i;
            in = exploringPath +1;
            return;
         }
      }

      std::cout << "WTF!\n";
      assert(false);
   };

   std::function<void(long long)> outputFunc = [&](long long out)
   {
      if (out == 0)
      {
         currentRoom->dir[exploringPath] = 0;
      }
      else
      {
         Point newPoint = currentPoint;
         switch (exploringPath)
         {
            case 0:
               newPoint.y--;
               break;
            case 1:
               newPoint.y++;
               break;
            case 2:
               newPoint.x--;
               break;
            case 3:
               newPoint.x++;
               break;
         }

         if (out == 1 || out == 2)
         {
            if (out == 2)
            {
               if (oxygenSysemFound == false)
               {
                  oxygenSysemFound = true;
                  std::cout << "Found oxygen system at: " << newPoint.x << ", " << newPoint.y << '\n';
            
                  // Backtracking
                  int steps = 1; // start with 1 because we didn't count step to oxygen system

                  bool backtrackFound = true;
                  Room* currentBacktrackedRoom = currentRoom;
                  while (backtrackFound)
                  {
                     backtrackFound = false;
                     for (int i=0; i<4; i++)
                     {
                        if (currentBacktrackedRoom->dir[i] == 2)
                        {
                           backtrackFound = true;
                           steps++;
                           currentBacktrackedRoom = currentBacktrackedRoom->otherRoom[i];
                        }
                     }
                  }

                  std::cout << "Steps to oxygen system: " << steps << '\n';
               }
            }

            if (rooms.find(newPoint) == rooms.end())
            {
               rooms[newPoint] = new Room();

               int mirrorPath = GetMirrorPath(exploringPath);
               rooms[newPoint]->dir[mirrorPath] = 2;
               rooms[newPoint]->otherRoom[mirrorPath] = currentRoom;
            }

            currentRoom->dir[exploringPath] = 1;
            currentRoom->otherRoom[exploringPath] = rooms[newPoint];  
            
            currentRoom = rooms[newPoint];
            currentPoint = newPoint;

            if (oxygenSystemRoom == nullptr && oxygenSysemFound)
            {
               oxygenSystemRoom = currentRoom;
            }
         }
      }
   };

   RunIntCode(data, inputFunc, outputFunc);

   std::vector<Room*> oxygenRooms;
   oxygenSystemRoom->hasOxygen = true;
   oxygenRooms.push_back(oxygenSystemRoom);

   int minutesPassed = 0;
   while(true)
   {
      std::vector<Room*> newOxygenRooms;
      for (Room* oxr : oxygenRooms)
      {
         if (oxr->hasOxygen && oxr->propagatedOxygen == false)
         {
            oxr->propagatedOxygen = true;
            for (int i=0; i<4; i++)
            {
               if (oxr->dir[i] == 1)
               {
                  if (oxr->otherRoom[i]->hasOxygen == false)
                  {
                     oxr->otherRoom[i]->hasOxygen = true;
                     newOxygenRooms.push_back(oxr->otherRoom[i]);
                  }
               }
            }
         }
      }
      if (newOxygenRooms.size() == 0)
      {
         break;
      }
      oxygenRooms.insert(oxygenRooms.begin(), newOxygenRooms.begin(), newOxygenRooms.end());
      minutesPassed++;
   }

   std::cout << "Minutes to fully fill rooms: " << minutesPassed << '\n';

   for (auto r : rooms)
   {
      delete r.second;
   }
   rooms.clear();

   return 0;
}