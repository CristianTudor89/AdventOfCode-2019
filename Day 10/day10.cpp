#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <assert.h>
using namespace std;

#define PI 3.14159265f

int mapSize = 41;

int CMMDC(int a, int b)
{
  while (a != b)
  {
    if (a > b)
      a -= b;
    else
      b -= a;
  }

  return a;
}

bool IsInLineOfSight(const vector<vector<char>> & map, int i, int j, int k, int l)
{
  if (i == k)
  {
    for (int v = min(j + 1, l + 1); v < max(j, l); v++)
      if (map[i][v] == '#')
        return false;

    return true;
  }
  
  if (j == l)
  {
    for (int v = min(i + 1, k + 1); v < max(i, k); v++)
      if (map[v][j] == '#')
        return false;

    return true;
  }

  int rowDiff = abs(k - i);
  int colDiff = abs(l - j);

  int cmmdc = CMMDC(rowDiff, colDiff);

  int stepX = (k - i) / cmmdc;
  int stepY = (l - j) / cmmdc;

  int x = i + stepX;
  int y = j + stepY;

  while (x != k && y != l)
  {
    if (map[x][y] == '#')
      return false;

    x += stepX;
    y += stepY;
  }

  return true;
}

int DetectFrom(const vector<vector<char>> & map, int i, int j)
{
  int count = 0;

  for (int k = 0; k < mapSize; k++)
  {
    for (int l = 0; l < mapSize; l++)
    {
      if (map[k][l] == '#' && !(i == k && j == l) && IsInLineOfSight(map, i, j, k, l))
        count++;
    }
  }

  return count;
}

struct AsteroidCoords
{
  int x;
  int y;
};

vector<AsteroidCoords> GatherCoords(const vector<vector<char>> & map, int i, int j)
{
  vector<AsteroidCoords> asteroidCoords;

  for (int k = 0; k < mapSize; k++)
  {
    for (int l = 0; l < mapSize; l++)
    {
      if (map[k][l] == '#' && !(i == k && j == l) && IsInLineOfSight(map, i, j, k, l))
        asteroidCoords.push_back({ k, l });
    }
  }

  return asteroidCoords;
}

int ManhattanDistance(const AsteroidCoords & p1, const AsteroidCoords & p2)
{
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

void BuildAngleMap(const vector<vector<char>> & asteroidMap, map<float, vector<AsteroidCoords>> & angleMap, const AsteroidCoords & origin)
{
  vector<AsteroidCoords> asteroidCoords = GatherCoords(asteroidMap, origin.x, origin.y);

  for (const AsteroidCoords & coords : asteroidCoords)
  {
    float angle = 0.f;

    if (coords.y == origin.y)
    {
      angle = (coords.x <= origin.x) ? 0.f : 180.f;
    }
    else
    {
      angle = atan(abs(coords.x - origin.x) / (float)abs(coords.y - origin.y)) * 180.f / PI;

      if (coords.x < origin.x)
      {
        if (coords.y > origin.y)
          angle = 90.f - angle;
        else
          angle = 270.f + angle;
      }
      else
      {
        if (coords.y > origin.y)
          angle = 90.f + angle;
        else
          angle = 270.f - angle;
      }
    }

    angleMap[angle].push_back(coords);
  }

  for (auto & angleMapPair : angleMap)
  {
    std::sort(angleMapPair.second.begin(), angleMapPair.second.end(), [&origin](const auto & coord1, const auto & coord2)
    {
      return ManhattanDistance(origin, coord1) > ManhattanDistance(origin, coord2);
    });
  }
}

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<vector<char>> asteroidMap;
  asteroidMap.push_back({});
  int row = 0;

  while (!in.eof())
  {
    char c;
    in >> c;

    if (asteroidMap[row].size() == mapSize)
    {
      if (row < mapSize - 1)
      {
        row++;
        asteroidMap.push_back({ c });
      }
      else
        break;
    }
    else
      asteroidMap[row].push_back(c);
  }

  int maxCount = 0, rowIndex = 0, colIndex = 0;

  for (int i = 0; i < mapSize; i++)
  {
    for (int j = 0; j < mapSize; j++)
    {
      if (asteroidMap[i][j] == '#')
      {
        int count = DetectFrom(asteroidMap, i, j);
        if (count > maxCount)
        {
          rowIndex = i;
          colIndex = j;
          maxCount = count;
        }
      }
    }
  }

  for (int i = 0; i < mapSize; i++)
  {
    for (int j = 0; j < mapSize; j++)
      cout << asteroidMap[i][j] << " ";
    cout << endl;
  }

  cout << endl << colIndex << " " << rowIndex << endl << maxCount << endl;

  // Part 2
  map<float, vector<AsteroidCoords>> angleMap;

  BuildAngleMap(asteroidMap, angleMap, { rowIndex, colIndex });

  int index = 0;

  for (auto & [angle, coords] : angleMap)
  {
    if (index == 199)
    {
      if (coords.empty())
        continue;

      cout << coords.back().y * 100 + coords.back().x << endl;
      break;
    }

    if (!coords.empty())
    {
      coords.pop_back();
      index++;
    }
  }

  return 0;
}
