#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <assert.h>
using namespace std;

void ReadOpcode(int instruction, int & opcode, int & mode1, int & mode2)
{
  if (instruction == 99)
    opcode = instruction;
  else
    opcode = instruction % 10;

  instruction /= 100;
  mode1 = instruction % 10;

  instruction /= 10;
  mode2 = instruction % 10;
}

int GetVal(const vector<int> & v, int val, int mode)
{
  return (mode == 0) ? v[val] : val;
}

int Execute1(vector<int> & v, int prevOutput, int phaseSetting)
{
  int retVal = 0;
  int i = 0, k = 0;

  while (i < v.size())
  {
    int opcode, mode1, mode2;
    ReadOpcode(v[i], opcode, mode1, mode2);

    if (opcode == 99)
      return retVal;
    else if (opcode == 1)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = val1 + val2;
      i += 4;
    }
    else if (opcode == 2)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = val1 * val2;
      i += 4;
    }
    else if (opcode == 3)
    {
      v[v[i + 1]] = (k >= 1) ? prevOutput : phaseSetting;
      k++;
      i += 2;
    }
    else if (opcode == 4)
    {
      retVal = GetVal(v, v[i + 1], mode1);
      i += 2;
    }
    else if (opcode == 5)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      if (val1)
        i = val2;
      else
        i += 3;
    }
    else if (opcode == 6)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      if (!val1)
        i = val2;
      else
        i += 3;
    }
    else if (opcode == 7)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = (val1 < val2);
      i += 4;
    }
    else if (opcode == 8)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = (val1 == val2);
      i += 4;
    }
  }

  return 0;
}

int Execute2(vector<int> & v, vector<int> & instrIndex, int prevOutput, int phaseSetting, vector<bool> & inputPhase, int index, vector<bool> & halt)
{
  int i = instrIndex[index];

  while (i < v.size())
  {
    int opcode, mode1, mode2;
    ReadOpcode(v[i], opcode, mode1, mode2);

    if (opcode == 99)
    {
      halt[index] = true;
      instrIndex[index] = i;
      return prevOutput;
    }

    else if (opcode == 1)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = val1 + val2;
      i += 4;
    }
    else if (opcode == 2)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = val1 * val2;
      i += 4;
    }
    else if (opcode == 3)
    {
      if (inputPhase[index])
      {
        v[v[i + 1]] = phaseSetting;
        inputPhase[index] = false;
      }
      else
      {
        v[v[i + 1]] = prevOutput;
      }

      i += 2;
    }
    else if (opcode == 4)
    {
      instrIndex[index] = i + 2;

      return GetVal(v, v[i + 1], mode1);
    }
    else if (opcode == 5)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      if (val1)
        i = val2;
      else
        i += 3;
    }
    else if (opcode == 6)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      if (!val1)
        i = val2;
      else
        i += 3;
    }
    else if (opcode == 7)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = (val1 < val2);
      i += 4;
    }
    else if (opcode == 8)
    {
      int val1 = GetVal(v, v[i + 1], mode1);
      int val2 = GetVal(v, v[i + 2], mode2);

      v[v[i + 3]] = (val1 == val2);
      i += 4;
    }
  }

  return 0;
}

void PartOne()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<int> n;

  while (!in.eof())
  {
    char c;
    int i;
    in >> i;
    n.push_back(i);

    if (!in.eof())
      in >> c;
  }

  int maxVal = 0;

  for (int i1 = 0; i1 < 5; i1++)
  {
    for (int i2 = 0; i2 < 5; i2++)
    {
      if (i2 == i1)
        continue;

      for (int i3 = 0; i3 < 5; i3++)
      {
        if (i3 == i1 || i3 == i2)
          continue;

        for (int i4 = 0; i4 < 5; i4++)
        {
          if (i4 == i1 || i4 == i2 || i4 == i3)
            continue;

          for (int i5 = 0; i5 < 5; i5++)
          {
            if (i5 == i1 || i5 == i2 || i5 == i3 | i5 == i4)
              continue;

            vector<int> vec{ i1, i2, i3, i4, i5 };

            int prevOutput = 0;

            for (int val : vec)
            {
              auto v = n;
              prevOutput = Execute1(v, prevOutput, val);
            }

            if (prevOutput > maxVal)
              maxVal = prevOutput;
          }
        }
      }
    }
  }

  cout << maxVal;
}

void PartTwo()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<int> n;

  while (!in.eof())
  {
    char c;
    int i;
    in >> i;
    n.push_back(i);

    if (!in.eof())
      in >> c;
  }

  int maxVal = 0;

  for (int i1 = 5; i1 <= 9; i1++)
  {
    for (int i2 = 5; i2 <= 9; i2++)
    {
      if (i2 == i1)
        continue;

      for (int i3 = 5; i3 <= 9; i3++)
      {
        if (i3 == i1 || i3 == i2)
          continue;

        for (int i4 = 5; i4 <= 9; i4++)
        {
          if (i4 == i1 || i4 == i2 || i4 == i3)
            continue;

          for (int i5 = 5; i5 <= 9; i5++)
          {
            if (i5 == i1 || i5 == i2 || i5 == i3 | i5 == i4)
              continue;

            vector<int> vec{ i1, i2, i3, i4, i5 };

            int prevOutput = 0;
            int index = 0;

            vector<bool> inputValPhase{ true, true, true, true, true };
            vector<bool> halt{ false, false, false, false, false };
            vector<int> instrIndex{ 0, 0, 0, 0, 0 };

            while (!all_of(halt.begin(), halt.end(), [](bool haltVal) { return haltVal == true; }))
            {
              auto v = n;
              prevOutput = Execute2(v, instrIndex, prevOutput, vec[index], inputValPhase, index, halt);

              if (index == 4 && halt[index])
              {
                if (prevOutput > maxVal)
                  maxVal = prevOutput;
                break;
              }

              do
              {
                if (index + 1 < vec.size())
                  index++;
                else
                  index = 0;
              } while (halt[index]);
            }
          }
        }
      }
    }
  }

  cout << maxVal;
}

int main()
{
  PartTwo();

  return 0;
}
