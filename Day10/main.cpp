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
#include <math.h>

const float EPSILON = .0001f;

float AlmostEqual(float a, float b)
{
   return std::fabs(a-b) <= EPSILON;
}

class Point
{
public:
   float x = 0.f;
   float y = 0.f;

   Point() 
   {}

   Point(float inX, float inY) :
      x(inX), y(inY)
   {}

   bool operator==(const Point& other) const
   {
      return (AlmostEqual(x, other.x) && AlmostEqual(y, other.y));
   }
};

class Vec2D : public Point
{
public:
   Vec2D(float inX, float inY) : 
      Point(inX, inY)
   {}

   float Length() const
   {
      return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
   }

   Vec2D Normalized() const
   {
      const float len = Length();
      return Vec2D(x / len, y / len);
   }
};

class Asteroid : public Point
{
public:
   int detected = 0;

   Asteroid() : 
      Point()
   {}

   Asteroid(float inX, float inY) : 
      Point(inX, inY)
   {}
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

   Line(const Point& p1, const Point& p2)
   {
      if (AlmostEqual(p1.x, p2.x))
      {
         isVertical = true;
         x = p1.x;
      }
      else if (AlmostEqual(p1.y, p2.y))
      {
         isHorizontal = true;
         y = p1.y;
      }
      else
      {
         a = (p2.y - p1.y) / (p2.x - p1.x);
         b = p1.y - a*p1.x;
      }
   }

   bool IsOnLine(const Point& p1)
   {
      if (isVertical)
      {
         return AlmostEqual(p1.x, x);
      }
      else if (isHorizontal)
      {
         return AlmostEqual(p1.y, y);
      }
      else
      {
         return AlmostEqual(p1.y, a*p1.x+b);
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
   return ((a <= x && x <= b) || (b <= x && x <= a)); 
}

bool IsBetweenPoints(const Point& x, const Point& a, const Point& b)
{
   return IsBetween(x.x, a.x, b.x) && IsBetween(x.y, a.y, b.y);
}

std::string CoordToName(int x, int y)
{
   return std::to_string(x) + 'x' + std::to_string(y);
}

inline Vec2D FromTwoPoints(const Point& p1, const Point& p2)
{
   return Vec2D(p2.x-p1.x, p2.y - p1.y);
}

inline float Dot(const Vec2D& v1, const Vec2D& v2)
{
   return v1.x*v2.x + v1.y*v2.y;
}

inline float ToDegrees(float rad)
{
   return rad * (180.f / M_PI);
}

class Ray
{
public:

   Point p1, p2;
   Line line;
   float angle = 0;
   std::map<int, Asteroid*> asteroidsOnLine;
   std::map<int, Asteroid*>::iterator asteroidsIt;

   Ray() {}

   Ray(const Point& inP1, const Point& inP2) :
      p1(inP1), p2(inP2), line(inP1, inP2)
   {
      Vec2D vec1 = FromTwoPoints(p1, Point(p1.x ,0));
      Vec2D vec2 = FromTwoPoints(p1, p2);
      angle = ToDegrees(std::acos(Dot(vec1.Normalized(), vec2.Normalized())));
      if (angle > 0 && angle < 180)
      {
          if (vec2.x < vec1.x)
          {
             angle = 360 - angle;
          }
      }
      angle = roundf(angle * 100) / 100;
   }

   void CollectAllAsteroids(std::vector<Asteroid>& a1)
   {
      for (Asteroid& a : a1)
      {
         if (a == p1) continue;
         //if (a == p2) continue;

         if (line.IsOnLine(a))
         {
            if (line.isVertical)
            {
               if (IsBetween(a.y, p1.y, p2.y) == false) continue; 
            }
            else if (line.isHorizontal)
            {
               if (IsBetween(a.x, p1.x, p2.x) == false) continue;
            }
            else
            {
               if (IsBetweenPoints(a, p1, p2) == false) continue;
            }

            int distance = std::abs(a.x - p1.x) + std::abs(a.y - p1.y);
            asteroidsOnLine[distance] = &a;
         }
      }

      asteroidsIt = asteroidsOnLine.begin();
   }

   Asteroid* GetNextAsteroid()
   {
      if (asteroidsIt == asteroidsOnLine.end())
      {
         return nullptr;
      }
      else
      {
         Asteroid* result = (*asteroidsIt).second;
         asteroidsIt++;
         return result;
      }
   }
};

int main() 
{
   std::ifstream infile("input.txt");   
   std::string line;

   std::vector<Asteroid> asteroids;

   int y = 0;
   int x = 0;
   while(infile >> line)
   {
      x = 0;
      for (char c : line)
      {
         if (c=='#')
         {
            asteroids.push_back(Asteroid(x,y));
         }
         x++;
      }
      y++;
   }

   int planeW = x;
   int planeH = y;

   for (auto& a1 : asteroids)
   {
      for (auto& a2 : asteroids)
      {
         if (a1 == a2) continue;

         Line line(a1, a2);
         bool a2IsVisible = true;
         for (auto& a3 : asteroids)
         {
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
                  if (IsBetweenPoints(a3, a1, a2)) {a2IsVisible = false; break;}
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

   for (auto& a1 : asteroids)
   {
      if (a1.detected > biggestDetect)
      {
         biggestDetect = a1.detected;
         abd = &a1;
      }
   }

   std::cout << "Asteroid with biggest detection: " <<   
      '(' << abd->x << ',' << abd->y << ")= " << abd->detected << '\n';
   

   std::map<float, Ray> rays;
   for (auto& a1 : asteroids)
   {
      if (a1 == *abd) continue;

      Ray ray(*abd, a1);
      
      if (rays.find(ray.angle) == rays.end())
      {
         rays[ray.angle] = ray;
         rays[ray.angle].CollectAllAsteroids(asteroids);
      }
   }

   bool ShouldEnd = false;
   int i = 1;
   while(ShouldEnd == false)
   {
      ShouldEnd = true;
      for (auto& ray : rays)
      {
         const Asteroid* asteroid = ray.second.GetNextAsteroid();
         if (asteroid != nullptr)
         {
            std::cout << i << ". Vaporized asteroid: (" << asteroid->x << ',' << asteroid->y << ")\n";          
            if (i%9 == 0)
            {
               std::cout << "---\n";
            }
            i++;
            ShouldEnd = false;
         }
      }
   }
   
   return 0;
}