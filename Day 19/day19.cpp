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
vector<vector<char>> matrix(50, vector<char>(50, 0));

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

void RunProgram(ofstream & out, vector<long long> & v, int posX, int posY, bool & found)
{
  int i = 0;
  bool setX = true;

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
      if (setX)
      {
        SetVal(v, v[i + 1], mode1, posX);
      }
      else
      {
        SetVal(v, v[i + 1], mode1, posY);
      }

      setX = !setX;

      i += 2;
    }
    else if (opcode == 4)
    {
      int output = GetVal(v, v[i + 1], mode1);
      if (output == 1)
      {
        found = true;
        return;
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

  v.resize(5000000);

  // Part 1
  int sum = 0;

  for (int i = 0; i < 50; i++)
  {
    for (int j = 0; j < 50; j++)
    {
      auto w{ v };
      bool found = false;
      RunProgram(out, w, i, j, found);

      if (found)
        sum++;
    }
  }

  cout << sum << endl;

  // Part 2
  int startPosX = 5, startPosY = 5;

  while (true)
  {
    bool isBeamLeft = false;
    bool isBeamRight = false;

    auto w{ v };
    RunProgram(out, w, startPosX, startPosY, isBeamLeft);

    w = v;
    RunProgram(out, w, startPosX, startPosY + 99, isBeamRight);

    // no beam horizontally
    if (!isBeamLeft || !isBeamRight)
    {
      // try next row
      startPosX++;

      bool isBeam = false;
      while (!isBeam)
      {
        w = v;
        RunProgram(out, w, startPosX, startPosY, isBeam);

        // continue horizontally until a beam point is found
        if (!isBeam)
          startPosY++;
      }
      continue;
    }

    w = v;
    bool isBeam = false;
    RunProgram(out, w, startPosX + 99, startPosY, isBeam);

    if (!isBeam)
    {
      startPosY++;
      continue;
    }

    cout << startPosX * 10000 + startPosY << endl;

    break;
  }

  return 0;
}
