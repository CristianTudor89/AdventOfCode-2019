#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <assert.h>
using namespace std;

long long relativeBase = 0;
bool firstTime = true;

struct Point
{
  Point(int x, int y) : x(x), y(y) {}

  bool operator==(const Point & point) const
  {
    return tie(x, y) == tie(point.x, point.y);
  }

  bool operator<(const Point & point) const
  {
    return tie(x, y) < tie(point.x, point.y);
  }

  int x;
  int y;
};

static const int dirX[4] = { -1, 0, 1, 0 };
static const int dirY[4] = { 0, 1, 0, -1 };

set<Point> wallPoints;

void RunProgram(ofstream & out, vector<long long> & v, const vector<Point> & points, int & i);

bool PointExists(const vector<Point> & points, const Point & point)
{
  return any_of(points.begin(), points.end(), [&point](const Point & p) { return p == point; });
}

void ReadOpcode(int instruction, int & opcode, int & mode1, int & mode2, int & mode3)
{
  if (instruction == 99)
    opcode = instruction;
  else
    opcode = instruction % 10;

  instruction /= 100;
  mode1 = instruction % 10;

  instruction /= 10;
  mode2 = instruction % 10;

  instruction /= 10;
  mode3 = instruction % 10;
}

long long GetVal(const vector<long long> & v, long long pos, int mode)
{
  if (mode == 0)
    return v[pos];

  if (mode == 1)
    return pos;

  if (mode == 2)
    return v[relativeBase + pos];

  return 0;
}

void SetVal(vector<long long> & v, long long pos, int mode, long long val)
{
  assert(mode != 1);

  if (mode == 0)
    v[pos] = val;
  else if (mode == 2)
    v[pos + relativeBase] = val;
}

Point GetNewPoint(const Point & p, int direction)
{
  Point newPoint(p);

  switch (direction)
  {
  case 1:
    newPoint.x--;
    break;

  case 2:
    newPoint.x++;
    break;

  case 3:
    newPoint.y--;
    break;

  case 4:
    newPoint.y++;
    break;

  default:
    break;
  }

  return newPoint;
}

map<Point, int> Explore(const Point & origin)
{
  queue<Point> unexploredPoints;
  unexploredPoints.push(origin);

  map<Point, int> distMap;
  
  while (!unexploredPoints.empty())
  {
    Point currentPoint = unexploredPoints.front();
    unexploredPoints.pop();

    for (int i = 0; i < 4; i++)
    {
      Point nextPoint(currentPoint.x + dirX[i], currentPoint.y + dirY[i]);
 
      bool isWall = any_of(wallPoints.begin(), wallPoints.end(), [&nextPoint](const auto & point) 
                           { return point == nextPoint; });

      if (!isWall && distMap.find(nextPoint) == distMap.end())
      {
        distMap[nextPoint] = distMap[currentPoint] + 1;
        unexploredPoints.push(nextPoint);
      }
    }
  }

  return distMap;
}

void MoveDroid(ofstream & out, vector<long long> & v, const vector<Point> & points, int & i, int mode)
{
  for (int j = 1; j <= 4; j++)
  {
    Point candidatePoint = GetNewPoint(points.back(), j);
    if (PointExists(points, candidatePoint))
      continue;

    vector<Point> newPoints{ points };
    newPoints.push_back(candidatePoint);

    vector<long long> v1{ v };
    SetVal(v1, v1[i + 1], mode, j);

    int k = i + 2;
    RunProgram(out, v1, newPoints, k);
  }
}

void RunProgram(ofstream & out, vector<long long> & v, const vector<Point> & points, int & i)
{
  bool hitWall = false, movedOneStep = false;

  while (true)
  {
    int opcode, mode1, mode2, mode3;
    ReadOpcode(v[i], opcode, mode1, mode2, mode3);

    if (opcode == 99)
    {
      break;
    }

    if (opcode == 1)
    {
      long long val1 = GetVal(v, v[i + 1], mode1);
      long long val2 = GetVal(v, v[i + 2], mode2);

      SetVal(v, v[i + 3], mode3, val1 + val2);
      i += 4;
    }
    else if (opcode == 2)
    {
      long long val1 = GetVal(v, v[i + 1], mode1);
      long long val2 = GetVal(v, v[i + 2], mode2);

      SetVal(v, v[i + 3], mode3, val1 * val2);
      i += 4;
    }
    else if (opcode == 3)
    {
      if (firstTime)
      {
        firstTime = false;

        MoveDroid(out, v, points, i, mode1);
        break;
      }
      else
      {
        if (hitWall)
          break;

        if (movedOneStep)
        {
          MoveDroid(out, v, points, i, mode1);
        }

        break;
      }
    }
    else if (opcode == 4)
    {
      int status = GetVal(v, v[i + 1], mode1);

      if (status == 0)
      {
        hitWall = true;
        wallPoints.insert(points.back());
        break;
      }
      else if (status == 1)
      {
        movedOneStep = true;
      }
      else if (status == 2)
      {
        std::cout << points.size() - 1 << endl;
      }
      else
        assert(0);

      i += 2;
    }
    else if (opcode == 5)
    {
      long long val1 = GetVal(v, v[i + 1], mode1);
      long long val2 = GetVal(v, v[i + 2], mode2);

      if (val1)
        i = val2;
      else
        i += 3;
    }
    else if (opcode == 6)
    {
      long long val1 = GetVal(v, v[i + 1], mode1);
      long long val2 = GetVal(v, v[i + 2], mode2);

      if (!val1)
        i = val2;
      else
        i += 3;
    }
    else if (opcode == 7)
    {
      long long val1 = GetVal(v, v[i + 1], mode1);
      long long val2 = GetVal(v, v[i + 2], mode2);

      SetVal(v, v[i + 3], mode3, val1 < val2);
      i += 4;
    }
    else if (opcode == 8)
    {
      long long val1 = GetVal(v, v[i + 1], mode1);
      long long val2 = GetVal(v, v[i + 2], mode2);

      SetVal(v, v[i + 3], mode3, val1 == val2);
      i += 4;
    }
    else if (opcode == 9)
    {
      long long val = GetVal(v, v[i + 1], mode1);

      relativeBase += val;
      i += 2;
    }
  }
}

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<long long> v;
  vector<Point> points(1, { 0, 0 });

  while (!in.eof())
  {
    char c;
    long long i;
    in >> i;
    v.push_back(i);

    if (!in.eof())
      in >> c;
  }

  v.resize(10000);

  int i = 0;
  RunProgram(out, v, points, i);

  // Part 2
  auto distMap = Explore({ 14, -16 });

  auto maxDistanceIt = max_element(distMap.begin(), distMap.end(), [](auto & dist1, auto & dist2) {
    return dist1.second < dist2.second;
  });

  std::cout << maxDistanceIt->second << endl;

  return 0;
}
