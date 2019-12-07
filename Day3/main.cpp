#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <assert.h>

enum class EDir : uint8_t
{
    VERTICAL,
    HORIZONTAL,
};

struct Point
{
    int x, y;
    Point() :
        x(0),
        y(0)
    {}
    Point(int inX, int inY) :
        x(inX),
        y(inY)
    {}
};

struct Wire
{
    EDir dir;
    Point begin, end;
    int length;

    Wire(const Point& inBeginPoint, const std::string& inStr)
    {
        begin = inBeginPoint;
        end = inBeginPoint;
        length = std::stoi(inStr.substr(1, inStr.length()-1));
        switch (inStr[0])
        {
            case 'U': end.y += length; dir = EDir::VERTICAL; break;
            case 'D': end.y -= length; dir = EDir::VERTICAL; break;
            case 'R': end.x += length; dir = EDir::HORIZONTAL; break;
            case 'L': end.x -= length; dir = EDir::HORIZONTAL; break; 
        }
    }
};

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

bool IsBetween(int X, int A, int B)
{
    return ((A < X && X < B) || (B < X && X < A));
}

static int result = -1;
void SetShortestIntersection(int newLen)
{
    if (result == -1)
    {
        result = newLen;
    }
    else
    {
        if (newLen < result)
        {
            result = newLen;
        }
    }
}

static int result2 = -1;
void SetShortestPath(int newLen)
{
    if (result2 == -1)
    {
        result2 = newLen;
    }
    else
    {
        if (newLen < result2)
        {
            result2 = newLen;
        }
    }
}

int CalcManhattanDistanceToZero(int X, int Y)
{
    return std::abs(X) + std::abs(Y);
}

int main() 
{
    std::ifstream infile("input.txt");

    std::string wire1Data;
    std::string wire2Data;

    infile >> wire1Data;
    infile >> wire2Data;

    std::vector<std::string> wire1Steps = Split(wire1Data, ',');
    std::vector<std::string> wire2Steps = Split(wire2Data, ',');

    std::vector<Wire> wire1;
    wire1.reserve(wire1Steps.size());

    std::vector<Wire> wire2;
    wire2.reserve(wire2Steps.size());

    Point currentPoint = Point(0,0);
    for (const std::string& step : wire1Steps)
    {
        wire1.push_back(Wire(currentPoint, step));
        currentPoint = wire1.back().end;
    }

    currentPoint = Point(0,0);
    for (const std::string& step : wire2Steps)
    {
        wire2.push_back(Wire(currentPoint, step));
        currentPoint = wire2.back().end;
    }

    int wire2Len = 0;
    for (const Wire& wire2Element : wire2)
    {
        int wire1Len = 0;
        for (const Wire& wire1Element : wire1)
        {
            if (wire1Element.dir != wire2Element.dir)
            {
                if (wire1Element.dir == EDir::VERTICAL)
                {
                    if (IsBetween(wire1Element.begin.x, wire2Element.begin.x, wire2Element.end.x))
                    {
                        if (IsBetween(wire2Element.begin.y, wire1Element.begin.y, wire1Element.end.y))
                        {
                            const int X = wire1Element.begin.x;
                            const int Y = wire2Element.begin.y;

                            const  int LastLen1 = std::abs(Y - wire1Element.begin.y); 
                            const int LastLen2 = std::abs(X - wire2Element.begin.x); 

                            SetShortestIntersection(CalcManhattanDistanceToZero(X, Y));
                            SetShortestPath(wire1Len + LastLen1 + wire2Len + LastLen2);
                        }
                    }
                }
                else if (wire1Element.dir == EDir::HORIZONTAL)
                {
                    if (IsBetween(wire2Element.begin.x, wire1Element.begin.x, wire1Element.end.x))
                    {
                        if (IsBetween(wire1Element.begin.y, wire2Element.begin.y, wire2Element.end.y))
                        {
                            const int X = wire2Element.begin.x;
                            const int Y = wire1Element.begin.y;

                            const int LastLen1 = std::abs(X - wire1Element.begin.x); 
                            const int LastLen2 = std::abs(Y - wire2Element.begin.y); 
                            
                            SetShortestIntersection(CalcManhattanDistanceToZero(X, Y));
                            SetShortestPath(wire1Len + LastLen1 + wire2Len + LastLen2);
                        }
                    }
                }
            }
            wire1Len += wire1Element.length;
        }
        wire2Len += wire2Element.length;
    }

    std::cout << "Result: " << result << '\n';
    std::cout << "Result2: " << result2 << '\n';

    return 0;
}
