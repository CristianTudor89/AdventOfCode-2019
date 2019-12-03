#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <assert.h>
using namespace std;

void PartOne();
void PartTwo();

struct Point
{
  Point() = default;
  Point(int x, int y, int s = 0) : x(x), y(y), steps(s) {}

  int x;
  int y;
  int steps;
};

void ReadInput(ifstream & in, vector<Point> & points, int & steps)
{
  string s;
  std::getline(in, s);

  int posX = 100, posY = 100, i = 0;

  while (i < s.size())
  {
    int j = i + 1;
    while (j < s.size() && s[j] != ',')
      j++;

    int offset = stoi(s.substr(i + 1, j - i - 1));
    steps += offset;

    if (s[i] == 'R')
    {
      points.emplace_back(posX + offset, posY, steps);
      posX += offset;
    }
    else if (s[i] == 'L')
    {
      points.emplace_back(posX - offset, posY, steps);
      posX -= offset;
    }
    else if (s[i] == 'U')
    {
      points.emplace_back(posX, posY - offset, steps);
      posY -= offset;
    }
    else if (s[i] == 'D')
    {
      points.emplace_back(posX, posY + offset, steps);
      posY += offset;
    }

    i = j + 1;
  }
}

int ManhattanDist(const Point & p1, const Point & p2)
{
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

bool Intersects(const Point & p1, const Point & p2, const Point & p3, const Point & p4, Point & intP)
{
  bool intersectsOnX = false;

  for (int i = std::min(p1.x, p2.x); i <= std::max(p1.x, p2.x); i++)
  {
    for (int j = std::min(p3.x, p4.x); j <= std::max(p3.x, p4.x); j++)
    {
      if (i == j)
      {
        intersectsOnX = true;
        intP.x = i;
        i = std::max(p1.x, p2.x) + 1;
        break;
      }
    }
  }

  bool intersectsOnY = false;

  for (int i = std::min(p1.y, p2.y); i <= std::max(p1.y, p2.y); i++)
  {
    for (int j = std::min(p3.y, p4.y); j <= std::max(p3.y, p4.y); j++)
    {
      if (i == j)
      {
        intersectsOnY = true;
        intP.y = i;
        i = std::max(p1.y, p2.y) + 1;
        break;
      }
    }
  }

  return intersectsOnX && intersectsOnY;
}

int main()
{
  PartTwo();

	return 0;
}

void PartOne()
{
	ifstream in("in.txt");
	ofstream out("out.txt");

  vector<Point> points1, points2;
  points1.emplace_back(100, 100);
  points2.emplace_back(100, 100);

  int steps1 = 0, steps2 = 0;
  ReadInput(in, points1, steps1);
  ReadInput(in, points2, steps2);

  Point origin(100, 100, 0);
  Point intersection;

  int minDist = 100000;

  for (int i = 0; i < points1.size() - 1; i++)
  {
    for (int j = 0; j < points2.size() - 1; j++)
    {
      if (Intersects(points1[i], points1[i + 1], points2[j], points2[j + 1], intersection))
      {
        if (i != 0 && ManhattanDist(intersection, origin) < minDist)
          minDist = ManhattanDist(intersection, origin);
      }
    }
  }

  out << minDist;
}

void PartTwo()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<Point> points1, points2;
  points1.emplace_back(100, 100, 0);
  points2.emplace_back(100, 100, 0);

  int steps1 = 0, steps2 = 0;
  ReadInput(in, points1, steps1);
  ReadInput(in, points2, steps2);

  Point origin(100, 100, 0);

  int minSteps = 10000000;

  for (int i = 0; i < points1.size() - 1; i++)
  {
    for (int j = 0; j < points2.size() - 1; j++)
    {
      Point intersection(0, 0);
      if (Intersects(points1[i], points1[i + 1], points2[j], points2[j + 1], intersection))
      {
        intersection.steps = points1[i].steps + ManhattanDist(points1[i], intersection);
        intersection.steps += points2[j].steps + ManhattanDist(points2[j], intersection);

        if (i != 0 && intersection.steps < minSteps)
          minSteps = intersection.steps;
      }
    }
  }

  out << minSteps;
}
