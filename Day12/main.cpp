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

class Vec3D
{
public:

   int x = 0;
   int y = 0;
   int z  =0;

   Vec3D() {}
   Vec3D(int _x, int _y, int _z) :
      x(_x), y(_y), z(_z)
   {}

   Vec3D& operator+=(const Vec3D& r)
   {
      x+=r.x;
      y+=r.y;
      z+=r.z;
      return *this;
   }
};

class Moon
{
public:
   
   int id = 0;

   Vec3D pos;
   Vec3D vel;

   int potential = 0;
   int kinetic = 0;

   Moon() {}
   Moon(int inId, const Vec3D& inPos) :
      id(inId), pos(inPos)
   {}

   void ApplyVelocity()
   {
      pos += vel;
   }

   int CalcPotential()
   {
      potential = std::abs(pos.x) + std::abs(pos.y) + std::abs(pos.z);
      return potential;
   }

   int CalcKinetic()
   {
      kinetic = std::abs(vel.x) + std::abs(vel.y) + std::abs(vel.z);
      return kinetic;
   }

   void Print()
   {
      printf("pos=<x=%i, y=%i, z=%i>, cel=<x=%i, y=%i, z=%i>\n", 
         pos.x, pos.y, pos.z, vel.x, vel.y, vel.z);
   }

};

int main() 
{
   std::vector<Moon> moons;
   moons.push_back(Moon(0, Vec3D(-8,-10,0)));
   moons.push_back(Moon(1, Vec3D(5,5,10)));
   moons.push_back(Moon(2, Vec3D(2,-7,3)));
   moons.push_back(Moon(3, Vec3D(9,-8,-3)));

   const long long steps = 4686774924;
   for (long long s=1; s<=steps; s++)
   {
      //auto start = std::chrono::high_resolution_clock::now();
      for (int i=0; i<4; i++)
      {
         for (int j=i+1; j<4; j++)
         {
            if (moons[i].pos.x > moons[j].pos.x)
            {
               moons[i].vel.x -= 1;
               moons[j].vel.x += 1;
            }
            else if (moons[i].pos.x < moons[j].pos.x)
            {
               moons[i].vel.x += 1;
               moons[j].vel.x -= 1;
            }
            
            if (moons[i].pos.y > moons[j].pos.y)
            {
               moons[i].vel.y -= 1;
               moons[j].vel.y += 1;
            }
            else if (moons[i].pos.y < moons[j].pos.y)
            {
               moons[i].vel.y += 1;
               moons[j].vel.y -= 1;
            }

            if (moons[i].pos.z > moons[j].pos.z)
            {
               moons[i].vel.z -= 1;
               moons[j].vel.z += 1;
            }
            else if (moons[i].pos.z < moons[j].pos.z)
            {
               moons[i].vel.z += 1;
               moons[j].vel.z -= 1;
            }
         }
      }

      for (int i=0; i<4; i++)
      {
         moons[i].ApplyVelocity();
      }

      /*
      auto stop = std::chrono::high_resolution_clock::now(); 
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
      std::cout << duration.count() << '\n'; 
      */
   }

   for (int i=0; i<4; i++)
   {
      moons[i].Print();
   }

   int sumOfEnergy = 0;
   for (int i=0; i<4; i++)
   {
      sumOfEnergy += (moons[i].CalcKinetic() * moons[i].CalcPotential());
   }

   std::cout << "sum of all energy: " << sumOfEnergy << '\n';

   return 0;
}