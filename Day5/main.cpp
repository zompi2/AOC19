#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <assert.h>

int main() 
{
   std::ifstream infile("input.txt");
   
   std::vector<int> data;
   std::string value;
   while (std::getline(infile, value, ','))
   {
      data.push_back(std::stoi(value));
   }

   bool bFinished = false;
   int idx = 0;

   while (bFinished == false)
   {
      const int OPCode = data[idx] % 100;
	   if (OPCode == 99)
      {
         bFinished = true;
         break;
      }
	  
      int immediateMode = data[idx] / 100;

      const int A = immediateMode / 100; immediateMode -= A*100;
      const int B = immediateMode / 10; immediateMode -= B*10;
      const int C = immediateMode;
     
      if (OPCode == 1 || OPCode == 2)
      {
		   const int val1 = C == 0 ? data[data[idx+1]] : data[idx+1];
		   const int val2 = B == 0 ? data[data[idx+2]] : data[idx+2];
		   int& result =  data[data[idx+3]];
	  
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
         std::cout << "Please give input: ";        
         std::cin >> data[data[idx+1]];
         std::cin.ignore();
         std::cout << '\n';
         idx += 2;
      }
      else if (OPCode == 4)
      {
         const int val = C == 0 ? data[data[idx+1]] : data[idx+1];
         std::cout << "Output: " << val << '\n';
         idx += 2;
      }
      else if (OPCode == 5 || OPCode == 6)
      {
		   const int val1 = C == 0 ? data[data[idx+1]] : data[idx+1];
         const int val2 = B == 0 ? data[data[idx+2]] : data[idx+2];

         if ((OPCode == 5 && val1 != 0) || (OPCode == 6 && val1 == 0))
         {
            idx = val2;
         }
         else
         {
            idx += 3;
         }
      }
      else if (OPCode == 7 || OPCode == 8)
      {
		   const int val1 = C == 0 ? data[data[idx+1]] : data[idx+1];
         const int val2 = B == 0 ? data[data[idx+2]] : data[idx+2];
         int& result =  data[data[idx+3]];
		 
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
   } 

   return 0;
}
