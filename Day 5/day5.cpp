#include <fstream>
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

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<int> v;

  while (!in.eof())
  {
    char c;
    int i;
    in >> i;
    v.push_back(i);

    if (!in.eof())
      in >> c;
  }

  int i = 0;
  while (i < v.size())
  {
    int opcode, mode1, mode2;
    ReadOpcode(v[i], opcode, mode1, mode2);

    if (opcode == 99)
      break;

    if (opcode == 1)
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
      // Part 1
      // v[v[i + 1]] = 1;

      // Part 2
      v[v[i + 1]] = 5;

      i += 2;
    }
    else if (opcode == 4)
    {
      out << v[v[i + 1]] << " ";
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
