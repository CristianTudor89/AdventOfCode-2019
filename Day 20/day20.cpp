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

const int sizeX = 117;
const int sizeY = 115;

vector<vector<char>> matrix(sizeX, vector<char>(sizeY, ' '));
int minVal = 100000;

static const int dirX[4] = { -1, 0, 1, 0 };
static const int dirY[4] = { 0, 1, 0, -1 };

struct Point
{
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) {}

  bool operator==(const Point & point) const
  {
    return tie(x, y) == tie(point.x, point.y);
  }

  bool operator!=(const Point & point) const
  {
    return tie(x, y) != tie(point.x, point.y);
  }

  bool operator<(const Point & point) const
  {
    return tie(x, y) < tie(point.x, point.y);
  }

  int x;
  int y;
};

map<string, set<Point>> extractedPortals;
map<Point, Point> portals;

Point startPoint(116, 33), endPoint(116, 41);
int offsetXY = 2;

bool PointExists(const vector<Point> & points, const Point & point)
{
  return any_of(points.begin(), points.end(), [&point](const Point & p) { return p == point; });
}

Point GetPointThroughPortal(const Point & point)
{
  if (portals.find(point) != portals.end())
    return portals[point];

  return Point(-1, -1);
}

bool IsPointOnEdge(const Point & point)
{
  return (point.x == 0) || (point.x == sizeX - 1) || (point.y == 0) || (point.y == sizeY - 1);
}

bool IsValidNextPoint(const Point & point)
{
  if (point.x < 0 || point.y < 0 || point.x >= sizeX || point.y >= sizeY)
    return false;

  return matrix[point.x][point.y] == '.';
}

void PrintMatrix()
{
  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      cout << matrix[i][j];
    }

    cout << endl;
  }
}

bool IsPortalLetter(char letter)
{
  return letter >= 'A' && letter <= 'Z';
}

bool IsPortalAround(const vector<string> & lines, int i, int j, string & portalName)
{
  if (lines[i][j] != '.')
    return false;

  if (i >= 2 && IsPortalLetter(lines[i - 1][j]) && IsPortalLetter(lines[i - 2][j]))
  {
    portalName = string(1, lines[i - 2][j]) + string(1, lines[i - 1][j]);
    return true;
  }

  if (i < lines.size() - 2 && IsPortalLetter(lines[i + 1][j]) && IsPortalLetter(lines[i + 2][j]))
  {
    portalName = string(1, lines[i + 1][j]) + string(1, lines[i + 2][j]);
    return true;
  }

  if (j >= 2 && IsPortalLetter(lines[i][j - 1]) && IsPortalLetter(lines[i][j - 2]))
  {
    portalName = string(1, lines[i][j - 2]) + string(1, lines[i][j - 1]);
    return true;
  }

  if (j < lines[i].size() - 2 && IsPortalLetter(lines[i][j + 1]) && IsPortalLetter(lines[i][j + 2]))
  {
    portalName = string(1, lines[i][j + 1]) + string(1, lines[i][j + 2]);
    return true;
  }

  return false;
}

void Explore(const Point & currentPoint, const vector<Point> & visited, int totalCount)
{
  if (currentPoint == endPoint)
  {
    if (totalCount < minVal)
      minVal = totalCount;

    return;
  }

  vector<Point> nextPoints;
  for (int i = 0; i < 4; i++)
    nextPoints.emplace_back(currentPoint.x + dirX[i], currentPoint.y + dirY[i]);

  Point pointThroughPortal = GetPointThroughPortal(currentPoint);
  if (pointThroughPortal != Point(-1, -1))
    nextPoints.push_back(pointThroughPortal);

  for (const auto & nextPoint : nextPoints)
  {
    if (!IsValidNextPoint(nextPoint))
      continue;

    if (PointExists(visited, nextPoint))
      continue;

    auto newVisited{ visited };
    newVisited.push_back(nextPoint);

    Explore(nextPoint, newVisited, totalCount + 1);
  }
}

void Explore(const Point & currentPortal, const Point & currentPoint, const vector<Point> & visited, int totalCount, int level)
{
  if (currentPoint == endPoint)
  {
    if (level == 0)
    {
      if (totalCount < minVal)
      {
        minVal = totalCount;
        cout << minVal << endl;
      }
    }

    return;
  }

  vector<Point> nextPoints;
  for (int i = 0; i < 4; i++)
    nextPoints.emplace_back(currentPoint.x + dirX[i], currentPoint.y + dirY[i]);

  Point pointThroughPortal = GetPointThroughPortal(currentPoint);
  if (pointThroughPortal != Point(-1, -1))
    nextPoints.push_back(pointThroughPortal);

  for (const auto & nextPoint : nextPoints)
  {
    if (!IsValidNextPoint(nextPoint))
      continue;

    if (PointExists(visited, nextPoint))
      continue;

    if (GetPointThroughPortal(currentPoint) == nextPoint)
    {
      int newLevel = IsPointOnEdge(currentPoint) ? level - 1 : level + 1;
      Explore(nextPoint, nextPoint, { nextPoint }, totalCount, newLevel);
    }
    else
    {
      auto newVisited{ visited };
      newVisited.push_back(nextPoint);

      Explore(currentPortal, nextPoint, newVisited, totalCount + 1, level);
    }
  }
}

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  int posX = 0, posY = 0, offsetX = 0;

  vector<string> lines;
  while (!in.eof())
  {
    string line;
    getline(in, line);
    lines.push_back(line);
  }

  for (int i = 0; i < lines.size(); i++)
  {
    for (int j = 0; j < lines[i].size(); j++)
    {
      string portalName;

      if (IsPortalAround(lines, i, j, portalName))
      {
        extractedPortals[portalName].insert({i - offsetXY, j - offsetXY});
      }
    }
  }

  for (auto & extractedPortal : extractedPortals)
  {
    if (extractedPortal.second.size() == 2)
    {
      auto firstIt = extractedPortal.second.begin();
      auto secondIt = std::next(firstIt, 1);

      portals[*firstIt] = *secondIt;
      portals[*secondIt] = *firstIt;
    }
  }

  in.clear();
  in.seekg(0, ios::beg);

  while (!in.eof())
  {
    string line;
    getline(in, line);

    offsetX++;
    if (offsetX <= offsetXY || offsetX > sizeX + offsetXY)
      continue;

    for (int i = offsetXY; i < line.size() - offsetXY; i++)
    {
      if (line[i] >= 'A' && line[i] <= 'Z')
        matrix[offsetX - offsetXY - 1][i - offsetXY] = ' ';
      else
        matrix[offsetX - offsetXY - 1][i - offsetXY] = line[i];
    }
  }

  PrintMatrix();

  vector<Point> visited{ startPoint };
  int totalCount = 0;

  // Part 1
  Explore(startPoint, visited, totalCount);

  cout << minVal << endl;

  // Part 2
  minVal = 100000;
  totalCount = 0;
  int level = 0;

  Explore(startPoint, startPoint, visited, totalCount, level);

  cout << minVal << endl;

  return 0;
}
