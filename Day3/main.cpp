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

struct Move
{
    EDir dir;
    Point begin, end;
    int length;

    Move(const Point& inBeginPoint, const std::string& inStr)
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

    std::string wire1;
    std::string wire2;

    infile >> wire1;
    infile >> wire2;

    std::vector<std::string> wire1Vec = Split(wire1, ',');
    std::vector<std::string> wire2Vec = Split(wire2, ',');

    std::vector<Move> wire1Moves;
    wire1Moves.reserve(wire1Vec.size());

    std::vector<Move> wire2Moves;
    wire2Moves.reserve(wire2Vec.size());

    Point currentPoint = Point(0,0);
    for (const std::string& element : wire1Vec)
    {
        wire1Moves.push_back(Move(currentPoint, element));
        currentPoint = wire1Moves.back().end;
    }

    currentPoint = Point(0,0);
    for (const std::string& element : wire2Vec)
    {
        wire2Moves.push_back(Move(currentPoint, element));
        currentPoint = wire2Moves.back().end;
    }

    int wire2Len = 0;
    for (const Move& move2 : wire2Moves)
    {
        int wire1Len = 0;
        for (const Move& move1 : wire1Moves)
        {
            if (move1.dir != move2.dir)
            {
                if (move1.dir == EDir::VERTICAL)
                {
                    if (IsBetween(move1.begin.x, move2.begin.x, move2.end.x))
                    {
                        if (IsBetween(move2.begin.y, move1.begin.y, move1.end.y))
                        {
                            int X = move1.begin.x;
                            int Y = move2.begin.y;

                            std::cout << "INTERSECT! " << X << " " << Y << '\n';
                            std::cout << "calculating route\n";

                            int LastLen1 = std::abs(Y - move1.begin.y); 
                            int LastLen2 = std::abs(X - move2.begin.x); 

                            std::cout << "Len1: " << wire1Len + LastLen1 << " Len2: " << wire2Len + LastLen2<< '\n';

                            SetShortestIntersection(CalcManhattanDistanceToZero(X, Y));
                            SetShortestPath(wire1Len + LastLen1 + wire2Len + LastLen2);
                        }
                    }
                }
                else if (move1.dir == EDir::HORIZONTAL)
                {
                    if (IsBetween(move2.begin.x, move1.begin.x, move1.end.x))
                    {
                        if (IsBetween(move1.begin.y, move2.begin.y, move2.end.y))
                        {
                            int X = move2.begin.x;
                            int Y = move1.begin.y;

                            std::cout << "INTERSECT! " << X << " " << Y << '\n';

                            std::cout << "calculating route\n";

                            int LastLen1 = std::abs(X - move1.begin.x); 
                            int LastLen2 = std::abs(Y - move2.begin.y); 

                            std::cout << "Len1: " << wire1Len + LastLen1 << " Len2: " << wire2Len + LastLen2<< '\n';

                            SetShortestIntersection(CalcManhattanDistanceToZero(X, Y));
                            SetShortestPath(wire1Len + LastLen1 + wire2Len + LastLen2);
                        }
                    }
                }
            }
            wire1Len += move1.length;
        }
        wire2Len += move2.length;
    }

    std::cout << "Result: " << result << '\n';
    std::cout << "Result2: " << result2 << '\n';

    return 0;
}
