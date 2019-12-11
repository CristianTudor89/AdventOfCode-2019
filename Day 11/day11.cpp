#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <assert.h>
using namespace std;

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

long long relativeBase = 0;

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

struct Point
{
  int x;
  int y;
};

vector<Point> points;

void GetDirection(long long val, char & currentDir, int & posX, int & posY)
{
  assert(val == 0 || val == 1);

  if (val == 0)
  {
    if (currentDir == '^')
    {
      currentDir = '<';
      posY--;
    }
    else if (currentDir == '>')
    {
      currentDir = '^';
      posX--;
    }
    else if (currentDir == 'v')
    {
      currentDir = '>';
      posY++;
    }
    else if (currentDir == '<')
    {
      currentDir = 'v';
      posX++;
    }
  }
  else if (val == 1)
  {
    if (currentDir == '^')
    {
      currentDir = '>';
      posY++;
    }
    else if (currentDir == '>')
    {
      currentDir = 'v';
      posX++;
    }
    else if (currentDir == 'v')
    {
      currentDir = '<';
      posY--;
    }
    else if (currentDir == '<')
    {
      currentDir = '^';
      posX--;
    }
  }
}

void RunProgram(ofstream & out, vector<long long> & v, vector<vector<char>> & matrix)
{
  int i = 0;
  char currentDir = '^';
  int posX = 5000, posY = 5000;
  bool paintColor = true, firstTime = true;

  while (true)
  {
    int opcode, mode1, mode2, mode3;
    ReadOpcode(v[i], opcode, mode1, mode2, mode3);

    if (opcode == 99)
    {
      out << points.size() << endl;

      // Part 2
      auto minXIt = min_element(points.begin(), points.end(), [](const Point & p1, const Point & p2) { return p1.x < p2.x; });
      auto maxXIt = min_element(points.begin(), points.end(), [](const Point & p1, const Point & p2) { return p1.x > p2.x; });
      auto minYIt = min_element(points.begin(), points.end(), [](const Point & p1, const Point & p2) { return p1.y < p2.y; });
      auto maxYIt = min_element(points.begin(), points.end(), [](const Point & p1, const Point & p2) { return p1.y > p2.y; });

      for (int k = minXIt->x; k <= maxXIt->x; k++)
      {
        for (int l = minYIt->y; l <= maxYIt->y; l++)
          out << matrix[k][l] << " ";
        out << endl;
      }

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
      // Part 2
      if (firstTime)
      {
        if (posX == 5000 && posY == 5000)
        {
          SetVal(v, v[i + 1], mode1, 1);
          firstTime = false;
          i += 2;
          continue;
        }
      }

      if (matrix[posX][posY] == '.')
        SetVal(v, v[i + 1], mode1, 0);
      else if (matrix[posX][posY] == '#')
        SetVal(v, v[i + 1], mode1, 1);

      i += 2;
    }
    else if (opcode == 4)
    {
      if (paintColor)
      {
        if (GetVal(v, v[i + 1], mode1) == 0)
          matrix[posX][posY] = '.';
        else if (GetVal(v, v[i + 1], mode1) == 1)
          matrix[posX][posY] = '#';
        else
          assert(0);

        bool isUnique = none_of(points.begin(), points.end(), [posX, posY](const Point & p) { return p.x == posX && p.y == posY; });
        if (isUnique)
          points.push_back({ posX, posY });

        paintColor = false;
      }
      else
      {
        GetDirection(GetVal(v, v[i + 1], mode1), currentDir, posX, posY);

        paintColor = true;
      }

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

  while (!in.eof())
  {
    char c;
    long long i;
    in >> i;
    v.push_back(i);

    if (!in.eof())
      in >> c;
  }

  v.resize(100000000);

  vector<vector<char>> matrix(10000, vector<char>(10000, '.'));
  matrix[5000][5000] = '^';

  RunProgram(out, v, matrix);

  return 0;
}
