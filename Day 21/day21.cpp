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
      long long output = GetVal(v, v[i + 1], mode1);
      if (output > 127)
        cout << output;
      else
        cout << static_cast<char>(output);

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

  AddInstruction("NOT A T\n");
  AddInstruction("OR T J\n");
  AddInstruction("NOT B T\n");
  AddInstruction("OR T J\n");
  AddInstruction("NOT C T\n");
  AddInstruction("OR T J\n");
  AddInstruction("AND D J\n");

  // AddInstruction("WALK\n");

  // Part 2
  AddInstruction("NOT H T\n");
  AddInstruction("NOT T T\n");
  AddInstruction("OR E T\n");
  AddInstruction("AND T J\n");

  AddInstruction("RUN\n");

  RunProgram(v);

  return 0;
}
