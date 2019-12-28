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

vector<long long> instructions;
long long relativeBase = 0;

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

void RunProgram(vector<long long> & v)
{
  int i = 0, instrIndex = 0;

  while (true)
  {
    int opcode, mode1, mode2, mode3;
    ReadOpcode(v[i], opcode, mode1, mode2, mode3);

    if (opcode == 99)
      break;

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
      if (instrIndex < instructions.size())
        SetVal(v, v[i + 1], mode1, instructions[instrIndex++]);

      i += 2;
    }
    else if (opcode == 4)
    {
      char output = static_cast<char>(GetVal(v, v[i + 1], mode1));
      cout << output;
      
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

void AddInstruction(const string & instruction)
{
  for (int i = 0; i < instruction.size(); i++)
  {
    instructions.push_back(static_cast<long long>(instruction[i]));
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

  v.resize(1000000);

  AddInstruction("west\n");
  AddInstruction("take fixed point\n");
  AddInstruction("north\n");
  AddInstruction("take sand\n");
  AddInstruction("south\n");
  AddInstruction("east\n");
  AddInstruction("east\n");
  AddInstruction("north\n");
  AddInstruction("north\n");
  AddInstruction("north\n");
  AddInstruction("take coin\n");
  AddInstruction("south\n");
  AddInstruction("south\n");
  AddInstruction("west\n");
  AddInstruction("north\n");
  AddInstruction("take spool of cat6\n");
  AddInstruction("north\n");
  AddInstruction("west\n");
  AddInstruction("north\n");

  RunProgram(v);

  return 0;
}
