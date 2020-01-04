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

// Part 1
void DealIntoNewStack(vector<int> & v)
{
  reverse(v.begin(), v.end());
}

void Cut(vector<int> & v, int cut)
{
  auto w{ v };

  if (cut > 0)
  {
    int pos = 0;

    for (int i = 0; i < cut; i++)
    {
      pos = (i + cut) % v.size();
      v[i] = w[pos];
    }

    for (int i = cut, j = pos + 1; i < v.size(); i++, j++)
    {
      v[i] = w[j % v.size()];
    }
  }
  else
  {
    cut = abs(cut);

    for (int i = 0; i < cut; i++)
    {
      v[i] = w[v.size() - cut + i];
    }

    for (int i = cut, j = 0; i < v.size(); i++, j++)
    {
      v[i] = w[j];
    }
  }
}

void DealWithIncrement(vector<int> & v, int increment)
{
  auto w{ v };
  int i = 0, pos = 0;

  while (i < v.size())
  {
    v[pos] = w[i];
    i++;
    pos = (pos + increment) % v.size();
  }
}

// Part 2
using int128 = __int128_t;

const int128 DeckSize     = 119315717514047;
const int128 ShuffleCount = 101741582076661;

struct Matrix2
{
  Matrix2() {}
  Matrix2(int128 x, int128 y, int128 z, int128 t) : x(x), y(y), z(z), t(t) {}

  Matrix2 operator*(const Matrix2 & matrix) const
  {
    return Matrix2(((x * matrix.x + y * matrix.z) % DeckSize + DeckSize) % DeckSize,
                   ((x * matrix.y + y * matrix.t) % DeckSize + DeckSize) % DeckSize,
                   ((z * matrix.x + t * matrix.z) % DeckSize + DeckSize) % DeckSize,
                   ((z * matrix.y + t * matrix.t) % DeckSize + DeckSize) % DeckSize);
  }

  int128 x, y, z, t;
};

int128 FastExp(int128 base, int128 exponent)
{
  if (exponent == 0)
    return 1;

  int128 x = FastExp(base, exponent / 2);
  x = x * x % DeckSize;

  if (exponent & 1)
    return (x * base) % DeckSize;

  return x;
}

Matrix2 FastExp(const Matrix2 & matrix, int128 exponent)
{
  if (exponent == 0)
    return Matrix2(1, 0, 0, 1);

  Matrix2 newMatrix = FastExp(matrix, exponent / 2);
  newMatrix = newMatrix * newMatrix;

  if (exponent & 1)
    return matrix * newMatrix;

  return newMatrix;
}

int128 ModularInverse(int128 base)
{
  return FastExp(base, DeckSize - 2);
}

void DealIntoNewStack(Matrix2 & matrix)
{
  matrix = matrix * Matrix2(-1, DeckSize - 1, 0, 1);
}

void Cut(Matrix2 & matrix, int128 cut)
{
  matrix = matrix * Matrix2(1, cut, 0, 1);
}

void DealWithIncrement(Matrix2 & matrix, int128 increment)
{
  matrix = matrix * Matrix2(ModularInverse(increment), 0, 0, 1);
}

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<string> lines;
  while (!in.eof())
  {
    string line;
    getline(in, line);
    lines.push_back(line);
  }

  // Part 1
  vector<int> v(10007);
  for (int i = 0; i < v.size(); i++)
    v[i] = i;

  for (auto & line : lines)
  {
    if (line == "deal into new stack")
    {
      DealIntoNewStack(v);
    }
    else if (line.find("cut", 0) != string::npos)
    {
      Cut(v, stoi(line.substr(4)));
    }
    else if (line.find("deal with increment") != string::npos)
    {
      DealWithIncrement(v, stoi(line.substr(20)));
    }
  }

  for (int i = 0; i < v.size(); i++)
  {
    if (v[i] == 2019)
    {
      cout << i << endl;
      break;
    }
  }

  // Part 2
  Matrix2 matrix(1, 0, 0, 1);

  for (auto & line : lines)
  {
    if (line == "deal into new stack")
    {
      DealIntoNewStack(matrix);
    }
    else if (line.find("cut", 0) != string::npos)
    {
      Cut(matrix, static_cast<int128>(stoi(line.substr(4))));
    }
    else if (line.find("deal with increment") != string::npos)
    {
      DealWithIncrement(matrix, static_cast<int128>(stoi(line.substr(20))));
    }
  }

  matrix = FastExp(matrix, ShuffleCount);

  int128 result = (matrix.x * 2020 + matrix.y) % DeckSize;

  cout << static_cast<long long>(result) << endl;

  return 0;
}
