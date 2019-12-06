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

    Move(const Point& inBeginPoint, const std::string& inStr)
    {
        begin = inBeginPoint;
        end = inBeginPoint;
        const int value = std::stoi(inStr.substr(1, inStr.length()-1));
        switch (inStr[0])
        {
            case 'U': end.y += value; dir = EDir::VERTICAL; break;
            case 'D': end.y -= value; dir = EDir::VERTICAL; break;
            case 'R': end.x += value; dir = EDir::HORIZONTAL; break;
            case 'L': end.x -= value; dir = EDir::HORIZONTAL; break; 
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

    for (const Move& move2 : wire2Moves)
    {
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
                            std::cout << "INTERSECT! " << move1.begin.x << " " << move2.begin.y << '\n';
                            SetShortestIntersection(CalcManhattanDistanceToZero(move1.begin.x, move2.begin.y));
                        }
                    }
                }
                else if (move1.dir == EDir::HORIZONTAL)
                {
                    if (IsBetween(move2.begin.x, move1.begin.x, move1.end.x))
                    {
                        if (IsBetween(move1.begin.y, move2.begin.y, move2.end.y))
                        {
                            std::cout << "INTERSECT! " << move2.begin.x << " " << move1.begin.y << '\n';
                            SetShortestIntersection(CalcManhattanDistanceToZero(move2.begin.x, move1.begin.y));
                        }
                    }
                }
            }
        }
    }

    std::cout << "Result: " << result << '\n';

    return 0;
}
