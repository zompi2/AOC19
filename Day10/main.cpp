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

const float EPSILON = .0001f;

float AlmostEqual(float a, float b)
{
   return std::fabs(a-b) <= EPSILON;
}

class Asteroid
{
public:

   float x = 0.f;
   float y = 0.f;
   int detected = 0;

   Asteroid(float inX, float inY) :
      x(inX), y(inY)
   {}

   Asteroid()
   {}

   bool operator==(const Asteroid& other)
   {
      return (AlmostEqual(x, other.x) && AlmostEqual(y, other.y));
   }
};

class Line
{
public:
   float a = 0.f;
   float b = 0.f;
   float x = 0.f;
   float y = 0.f; 

   bool isVertical = false;
   bool isHorizontal = false;

   Line() {}

   Line(const Asteroid& a1, const Asteroid& a2)
   {
      if (AlmostEqual(a1.x, a2.x))
      {
         isVertical = true;
         x = a1.x;
      }
      else if (AlmostEqual(a1.y, a2.y))
      {
         isHorizontal = true;
         y = a1.y;
      }
      else
      {
         a = (a2.y - a1.y) / (a2.x - a1.x);
         b = a1.y - a*a1.x;
      }
   }

   bool IsOnLine(const Asteroid& a1)
   {
      if (isVertical)
      {
         return AlmostEqual(a1.x, x);
      }
      else if (isHorizontal)
      {
         return AlmostEqual(a1.y, y);
      }
      else
      {
         return AlmostEqual(a1.y, a*a1.x+b);
      }
   }

   float GetXFromY(float y)
   {
      if (isVertical)
      {
         return x;
      }
      else
      {
         return (y-b)/a;
      }
   }
};

bool IsBetween(float x, float a, float b)
{
   return ((a < x && x < b) || (b < x && x < a)); 
}

bool IsBetweenAsteroid(const Asteroid& x, const Asteroid& a, const Asteroid& b)
{
   return IsBetween(x.x, a.x, b.x) && IsBetween(x.y, a.y, b.y);
}

std::string CoordToName(int x, int y)
{
   return std::to_string(x) + 'x' + std::to_string(y);
}

int main() 
{
   std::ifstream infile("input.txt");   
   std::string line;

   std::map<std::string, Asteroid> asteroids;

   int y = 0;
   int x = 0;
   while(infile >> line)
   {
      x = 0;
      for (char c : line)
      {
         if (c=='#')
         {
            asteroids[CoordToName(x,y)] = Asteroid(x,y);
         }
         x++;
      }
      y++;
   }

   int planeW = x;
   int planeH = y;

   for (auto& a1Iter : asteroids)
   {
      Asteroid& a1 = a1Iter.second;
      for (auto& a2Iter : asteroids)
      {
         Asteroid& a2 = a2Iter.second;
         if (a1 == a2) continue;

         Line line(a1, a2);
         bool a2IsVisible = true;
         for (auto& a3Iter : asteroids)
         {
            Asteroid& a3 = a3Iter.second;
            if (a1 == a3) continue;
            if (a2 == a3) continue;

            if (line.IsOnLine(a3))
            {
               if (line.isVertical)
               {
                  if (IsBetween(a3.y, a1.y, a2.y)) {a2IsVisible = false; break;} 
               }
               else if (line.isHorizontal)
               {
                  if (IsBetween(a3.x, a1.x, a2.x)) {a2IsVisible = false; break;}
               }
               else
               {
                  if (IsBetweenAsteroid(a3, a1, a2)) {a2IsVisible = false; break;}
               }
            }
         }
         if (a2IsVisible)
         {
            a1.detected++;
         }
      }
   }

   int biggestDetect = -1;
   Asteroid* abd = nullptr;

   for (auto& a1Iter : asteroids)
   {
      Asteroid& a1 = a1Iter.second;
     // std::cout << '(' << a1.x << ',' << a1.y << ")= " << a1.detected << '\n';
      if (a1.detected > biggestDetect)
      {
         biggestDetect = a1.detected;
         abd = &a1;
      }
   }

   std::cout << "Asteroid with biggest detection: " <<   
      '(' << abd->x << ',' << abd->y << ")= " << abd->detected << '\n';
   

   int shot = 1;
   
   Asteroid aTarget(abd->x, 0.f);
   bool moveHorizontally = true;
   while(shot <= 3)
   {
      Line shotLine(*abd, aTarget);
      if (aTarget.y == 0) // moving through up edge
      {
         for (int y = abd->y - 1; y >=0; y--)
         {
            float x = shotLine.GetXFromY(y);
            if (std::abs(x - (int)x) < 0.001)
            {
               const std::string id = CoordToName((int)x,(int)y);
               auto asteroidFound = asteroids.find(id);
               if (asteroidFound != asteroids.end())
               {
                  std::cout << "Veporizing: " << id << '\n';
                  asteroids.erase(asteroidFound);
                  break;
               }
            }
         }
         if (aTarget.x == planeW-1)
         {
            aTarget.y++;
         }
         else
         {
            aTarget.x++;
         }
      }

      shot++;
   }

   return 0;
}