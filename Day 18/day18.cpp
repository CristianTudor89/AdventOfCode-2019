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
#include <chrono>
#include <assert.h>
using namespace std;

static const int UNREACHABLE = 100000000;
static const int sizeX = 81;
static const int sizeY = 81;
int minVal = UNREACHABLE;

vector<vector<char>> matrix(sizeX, vector<char>(sizeY, 0));

struct Path
{
  Path(const vector<char> & prevNodes) : previousNodes(prevNodes) {}

  vector<char> previousNodes;
};

struct GraphNode
{
  GraphNode(char key, const Path & initialPath) : key(key)
  {
    paths.push_back(initialPath);
  }

  char key;
  vector<Path> paths;
};

static const int dirX[4] = { -1, 0, 1, 0 };
static const int dirY[4] = { 0, 1, 0, -1 };

map<pair<char, char>, vector<pair<int, Path>>> distances;

struct Point
{
  Point() : x(0), y(0) {}
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

int ManhattanDistance(const Point & p1, const Point & p2)
{
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

bool PointExists(const vector<Point> & points, const Point & point)
{
  return any_of(points.begin(), points.end(), [&point](const Point & p) { return p == point; });
}

bool IsKey(char field)
{
  return field >= 'a' && field <= 'z';
}

bool IsDoor(char field)
{
  return field >= 'A' && field <= 'Z';
}

bool IsWall(char field)
{
  return field == '#';
}

bool IsValidNextPoint(const Point & point, const vector<char> & previousKeys, char searchedKey)
{
  if (point.x < 0 || point.y < 0 || point.x >= sizeX || point.y >= sizeY)
    return false;

  if (IsWall(matrix[point.x][point.y]))
    return false;

  if (IsDoor(matrix[point.x][point.y]))
  {
    char door = matrix[point.x][point.y];

    bool found = any_of(previousKeys.begin(), previousKeys.end(), [door](char key) { return tolower(door) == key; });
    if (!found)
      return false;
  }

  if (IsKey(matrix[point.x][point.y]) && matrix[point.x][point.y] != searchedKey)
  {
    char existingKey = matrix[point.x][point.y];

    bool found = any_of(previousKeys.begin(), previousKeys.end(), [existingKey](char key) { return existingKey == key; });
    if (!found)
      return false;
  }

  return true;
}

vector<char> GetKeys()
{
  vector<char> keys;

  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      if (IsKey(matrix[i][j]))
        keys.push_back(matrix[i][j]);
    }
  }

  return keys;
}

void PrintMatrix(const vector<vector<char>> & currentMatrix)
{
  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      cout << currentMatrix[i][j];
    }

    cout << endl;
  }

  cout << endl;
}

int GetMinDistance(const Point & currentPoint, const Point & destPoint, const vector<Point> & visitedPoints, const vector<char> & previousKeys, int count, int minDistance)
{
  if (currentPoint == destPoint)
    return count;

  for (int i = 0; i < 4; i++)
  {
    Point nextPoint(currentPoint.x + dirX[i], currentPoint.y + dirY[i]);

    if (!IsValidNextPoint(nextPoint, previousKeys, matrix[destPoint.x][destPoint.y]))
      continue;

    if (PointExists(visitedPoints, nextPoint))
      continue;

    vector<Point> newVisitedPoints{ visitedPoints };
    newVisitedPoints.push_back(nextPoint);

    int minDist = GetMinDistance(nextPoint, destPoint, newVisitedPoints, previousKeys, count + 1, minDistance);
    if (minDist < minDistance)
      minDistance = minDist;
  }

  return minDistance;
}

int GetMinDistanceToKey(char sourceKey, char destKey, const vector<char> & previousKeys)
{
  Point sourcePoint, destPoint;

  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      if (matrix[i][j] == sourceKey)
      {
        sourcePoint.x = i;
        sourcePoint.y = j;
      }
      else if (matrix[i][j] == destKey)
      {
        destPoint.x = i;
        destPoint.y = j;
      }
    }
  }

  vector<Point> visitedPoints{ sourcePoint };
  int minDist = UNREACHABLE, count = 0;

  return GetMinDistance(sourcePoint, destPoint, visitedPoints, previousKeys, count, minDist);
}

GraphNode * FindGraphNode(char key, vector<GraphNode> & graphNodesToSearch)
{
  for (auto & graphNode : graphNodesToSearch)
  {
    if (graphNode.key == key)
    {
      return &graphNode;
    }
  }

  return nullptr;
}

int GetDistance(char keyToSearchFrom, char searchedKey, const Path & pathToTest)
{
  if (distances.find(make_pair(keyToSearchFrom, searchedKey)) == distances.end())
    return UNREACHABLE;

  int minDistance = UNREACHABLE;

  auto & routes = distances[make_pair(keyToSearchFrom, searchedKey)];
  for (auto & route : routes)
  {
    bool isValidRoute = true;

    for (char key : route.second.previousNodes)
    {
      isValidRoute = any_of(pathToTest.previousNodes.begin(), pathToTest.previousNodes.end(), [key](char keyVal) { return keyVal == key; });
      if (!isValidRoute)
        break;
    }

    if (isValidRoute)
    {
      if (route.first < minDistance)
        minDistance = route.first;
      break;
    }
  }

  return minDistance;
}

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  int i = 0, j = 0, originX = 0, originY = 0;

  while (!in.eof())
  {
    char c;
    in >> c;

    matrix[i][j] = c;

    // Part 2
    if (matrix[i][j] == '@')
    {
      originX = i;
      originY = j;
    }

    j++;
    if (j == sizeY)
    {
      j = 0;
      i++;
      if (i == sizeX)
        break;
    }
  }

  // Part 2
  matrix[originX][originY] = '#';
  matrix[originX - 1][originY] = '#';
  matrix[originX + 1][originY] = '#';
  matrix[originX][originY - 1] = '#';
  matrix[originX][originY + 1] = '#';
  matrix[originX - 1][originY - 1] = '<';
  matrix[originX - 1][originY + 1] = '>';
  matrix[originX + 1][originY - 1] = '^';
  matrix[originX + 1][originY + 1] = 'v';

  PrintMatrix(matrix);

  vector<char> allKeys = GetKeys();

  vector<GraphNode> visitedNodes;

  // Part 1
  // vector<GraphNode> originNodes{ {'@', { {} }} };

  // Part 2
  vector<GraphNode> originNodes{ {'<', { {} }}, {'>', { {} }}, {'^', { {} }}, {'v', { {} }} };

  for (const auto & originNode : originNodes)
  {
    for (char key : allKeys)
    {
      int minDist = GetMinDistanceToKey(originNode.key, key, {});
      if (minDist < UNREACHABLE)
      {
        GraphNode newGraphNode(key, { { originNode.key } });

        distances[make_pair(originNode.key, key)].emplace_back(minDist, Path{ { originNode.key } });

        visitedNodes.push_back(newGraphNode);
      }
    }
  }

  while (!visitedNodes.empty())
  {
    auto nodesToSearchFrom{ visitedNodes };
    visitedNodes.clear();

    for (const GraphNode & nodeToSearchFrom : nodesToSearchFrom)
    {
      for (char key : allKeys)
      {
        if (key == nodeToSearchFrom.key)
          continue;

        for (const Path & path : nodeToSearchFrom.paths)
        {
          bool found = any_of(path.previousNodes.begin(), path.previousNodes.end(), [key](char val) { return key == val; });
          if (found)
            continue;

          Path newPath(path);
          newPath.previousNodes.push_back(nodeToSearchFrom.key);

          int minDist = GetDistance(nodeToSearchFrom.key, key, newPath);
          if (minDist == UNREACHABLE)
          {
            minDist = GetMinDistanceToKey(nodeToSearchFrom.key, key, newPath.previousNodes);
          }

          if (minDist == UNREACHABLE)
            continue;
          
          distances[make_pair(nodeToSearchFrom.key, key)].emplace_back(minDist, newPath);

          Path pathToTest(newPath);
          pathToTest.previousNodes.push_back(key);

          if (allKeys.size() == pathToTest.previousNodes.size() - 1)
          {
            int cost = 0;

            for (int i = 0; i < pathToTest.previousNodes.size() - 1; i++)
            {
              Path testPath({});
              for (int j = 0; j <= i; j++)
                testPath.previousNodes.push_back(pathToTest.previousNodes[j]);

              auto & routes = distances[make_pair(pathToTest.previousNodes[i], pathToTest.previousNodes[i + 1])];
              cost += GetDistance(pathToTest.previousNodes[i], pathToTest.previousNodes[i + 1], testPath);
            }

            if (cost < minVal)
              minVal = cost;
          }
          else
          {
            GraphNode * existingGraphNode = FindGraphNode(key, visitedNodes);
            if (existingGraphNode)
            {
              existingGraphNode->paths.push_back(newPath);
            }
            else
            {
              GraphNode newGraphNode(key, { newPath });
              visitedNodes.push_back(newGraphNode);
            }
          }
        }
      }
    }
  }

  cout << minVal << endl;

  return 0;
}
