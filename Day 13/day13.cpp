#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <assert.h>
using namespace std;

long long relativeBase = 0;
int blockCount = 0, minX = 1000, minY = 1000, maxX = 0, maxY = 0;

void ReadOpcode(int instruction, int& opcode, int& mode1, int& mode2, int& mode3)
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

long long GetVal(const vector<long long>& v, long long pos, int mode)
{
  if (mode == 0)
    return v[pos];

  if (mode == 1)
    return pos;

  if (mode == 2)
    return v[relativeBase + pos];

  return 0;
}

void SetVal(vector<long long>& v, long long pos, int mode, long long val)
{
  assert(mode != 1);

  if (mode == 0)
    v[pos] = val;
  else if (mode == 2)
    v[pos + relativeBase] = val;
}

int GetJoystickInput(int paddlePosY, int ballPosY)
{
  if (paddlePosY < ballPosY)
    return 1;

  if (paddlePosY == ballPosY)
    return 0;

  return -1;
}

void RunProgram(ofstream& out, vector<long long>& v, vector<vector<int>>& matrix)
{
  int i = 0, x = 0, y = 0, tileId = 0, count = 0, paddleY = 0, ballY = 0;
  bool getYPos = true, getXPos = false, getTileId = false;

  while (true)
  {
    int opcode, mode1, mode2, mode3;
    ReadOpcode(v[i], opcode, mode1, mode2, mode3);

    if (opcode == 99)
    {
      // Part 1
      cout << blockCount << endl;
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
      int joystickInput = GetJoystickInput(paddleY, ballY);

      SetVal(v, v[i + 1], mode1, joystickInput);

      i += 2;
    }
    else if (opcode == 4)
    {
      // Part 2
      if (getYPos)
      {
        y = GetVal(v, v[i + 1], mode1);

        getYPos = false;
        getXPos = true;
      }
      else if (getXPos)
      {
        x = GetVal(v, v[i + 1], mode1);

        getXPos = false;
        getTileId = true;
      }
      else if (getTileId)
      {
        if (x == 0 && y == -1)
          count++;

        if (count == 3)
        {
          cout << GetVal(v, v[i + 1], mode1) << endl;
          count = 0;
        }
        else
        {
          tileId = GetVal(v, v[i + 1], mode1);

          if (tileId == 3)
            paddleY = y;
          else if (tileId == 4)
            ballY = y;

          matrix[500 + x][500 + y] = tileId;

          if (500 + x < minX)
            minX = 500 + x;

          if (500 + x > maxX)
            maxX = 500 + x;

          if (500 + y < minY)
            minY = 500 + y;

          if (500 + y > maxY)
            maxY = 500 + y;

          // Part 1
          if (tileId == 2)
            blockCount++;
        }

        getTileId = false;
        getYPos = true;
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

  // Part 2
  v[0] = 2;

  v.resize(1000000);

  vector<vector<int>> matrix(1000, vector<int>(1000, 0));

  RunProgram(out, v, matrix);

  return 0;
}
