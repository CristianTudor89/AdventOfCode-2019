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
vector<vector<char>> matrix(1000, vector<char>(1000, 0));
int posX = 0, posY = 0, maxY = 0;

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

vector<Point> visited;

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

bool IsVacuumRobot(int output)
{
  return output == '^' || output == '<' || output == 'v' || output == '>';
}

bool IsScaffoldPoint(int i, int j)
{
  return (matrix[i][j] == '#' && matrix[i - 1][j] == '#' && matrix[i + 1][j] == '#' && matrix[i][j - 1] == '#' && matrix[i][j + 1] == '#');
}

Point GetNextPoint(char & vacuumRobot, const Point & pos, char & rotation)
{
  if (vacuumRobot == '^')
  {
    if (pos.x > 0 && matrix[pos.x - 1][pos.y] == '#')
      return Point(pos.x - 1, pos.y);

    if (pos.y > 0 && matrix[pos.x][pos.y - 1] == '#')
    {
      rotation = 'L';
      vacuumRobot = '<';

      return Point(pos.x, pos.y - 1);
    }
    else if (pos.y < maxY && matrix[pos.x][pos.y + 1] == '#')
    {
      rotation = 'R';
      vacuumRobot = '>';

      return Point(pos.x, pos.y + 1);
    }
  }
  else if (vacuumRobot == 'v')
  {
    if (pos.x < posX && matrix[pos.x + 1][pos.y] == '#')
      return Point(pos.x + 1, pos.y);

    if (pos.y > 0 && matrix[pos.x][pos.y - 1] == '#')
    {
      rotation = 'R';
      vacuumRobot = '<';

      return Point(pos.x, pos.y - 1);
    }
    else if (pos.y < maxY && matrix[pos.x][pos.y + 1] == '#')
    {
      rotation = 'L';
      vacuumRobot = '>';

      return Point(pos.x, pos.y + 1);
    }
  }
  else if (vacuumRobot == '<')
  {
    if (pos.y > 0 && matrix[pos.x][pos.y - 1] == '#')
      return Point(pos.x, pos.y - 1);

    if (pos.x > 0 && matrix[pos.x - 1][pos.y] == '#')
    {
      rotation = 'R';
      vacuumRobot = '^';

      return Point(pos.x - 1, pos.y);
    }
    else if (pos.x < posX && matrix[pos.x + 1][pos.y] == '#')
    {
      rotation = 'L';
      vacuumRobot = 'v';

      return Point(pos.x + 1, pos.y);
    }
  }
  else if (vacuumRobot == '>')
  {
    if (pos.y < maxY && matrix[pos.x][pos.y + 1] == '#')
      return Point(pos.x, pos.y + 1);

    if (pos.x > 0 && matrix[pos.x - 1][pos.y] == '#')
    {
      rotation = 'L';
      vacuumRobot = '^';

      return Point(pos.x - 1, pos.y);
    }
    else if (pos.x < posX && matrix[pos.x + 1][pos.y] == '#')
    {
      rotation = 'R';
      vacuumRobot = 'v';

      return Point(pos.x + 1, pos.y);
    }
  }

  return Point(-1, -1);
}

void RunProgram(ofstream & out, vector<long long> & v, const string & mainRoutine, const vector<string> functions, bool part2)
{
  int i = 0, routineIndex = 0, functionIndex = 0, indexInsideFunction = 0, videoFeedIndex = 0, dust = 0;

  while (true)
  {
    int opcode, mode1, mode2, mode3;
    ReadOpcode(v[i], opcode, mode1, mode2, mode3);

    if (opcode == 99)
    {
      if (part2)
        cout << "Dust: " << dust << endl;
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
      if (part2)
      {
        if (routineIndex < mainRoutine.size())
        {
          SetVal(v, v[i + 1], mode1, mainRoutine[routineIndex]);
          routineIndex++;
        }
        else if (routineIndex == mainRoutine.size())
        {
          SetVal(v, v[i + 1], mode1, 10);
          routineIndex++;
        }
        else if (functionIndex < functions.size())
        {
          if (indexInsideFunction < functions[functionIndex].size())
          {
            SetVal(v, v[i + 1], mode1, functions[functionIndex][indexInsideFunction]);
            indexInsideFunction++;
          }
          else if (indexInsideFunction == functions[functionIndex].size())
          {
            SetVal(v, v[i + 1], mode1, 10);
            indexInsideFunction = 0;
            functionIndex++;
          }
        }
        else if (videoFeedIndex == 0)
        {
          SetVal(v, v[i + 1], mode1, 'n');
          videoFeedIndex++;
        }
        else
          SetVal(v, v[i + 1], mode1, 10);
      }

      i += 2;
    }
    else if (opcode == 4)
    {
      int output = GetVal(v, v[i + 1], mode1);

      if (part2)
      {
        dust = output;
      }
      else
      {
        if (output == '\n')
        {
          posX++;
          posY = 0;
        }
        else if (output == '#')
        {
          matrix[posX][posY] = '#';
          posY++;
          if (posY > maxY)
            maxY = posY;
        }
        else if (output == '.')
        {
          matrix[posX][posY] = '.';
          posY++;
          if (posY > maxY)
            maxY = posY;
        }
        else if (IsVacuumRobot(output))
        {
          matrix[posX][posY] = output;
          posY++;
          if (posY > maxY)
            maxY = posY;
        }
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

  v.resize(10000);

  auto w{ v };
  RunProgram(out, w, {}, {}, false);

  // Part 1
  for (int i = 0; i <= posX; i++)
  {
    for (int j = 0; j <= maxY; j++)
    {
      cout << matrix[i][j];
    }

    cout << endl;
  }

  int sum = 0;

  for (int i = 0; i <= posX; i++)
  {
    for (int j = 0; j <= maxY; j++)
    {
      if (i > 0 && j > 0 && i < posX && j < maxY)
      {
        if (IsScaffoldPoint(i, j))
          sum += i * j;
      }
    }
  }

  cout << sum << endl << endl;

  // Part 2
  int scaffoldCount = 0;
  char vacuumRobot = 0;

  for (int i = 0; i <= posX; i++)
  {
    for (int j = 0; j <= maxY; j++)
    {
      if (matrix[i][j] == '#')
        scaffoldCount++;

      if (IsVacuumRobot(matrix[i][j]))
      {
        scaffoldCount++;
        vacuumRobot = matrix[i][j];
        visited.emplace_back(i, j);
      }
    }
  }

  string path;
  int forwardCount = 0;

  while (true)
  {
    char rotation{};
    Point nextPoint = GetNextPoint(vacuumRobot, visited.back(), rotation);

    if (nextPoint.x == -1 && nextPoint.y == -1)
    {
      path += ',' + to_string(forwardCount);
      break;
    }

    if (rotation == 'L' || rotation == 'R')
    {
      if (forwardCount > 0)
        path += ',' + to_string(forwardCount) + ',';

      path += rotation;
      forwardCount = 0;
    }

    visited.push_back(nextPoint);
    forwardCount++;
  }

  // R,8,L,12,R,8,R,12,L,8,R,10,R,12,L,8,R,10,R,8,L,12,R,8,R,8,L,8,L,8,R,8,R,10,R,8,L,12,R,8,R,8,L,12,R,8,R,8,L,8,L,8,R,8,R,10,R,12,L,8,R,10,R,8,L,8,L,8,R,8,R,10
  cout << path << endl << endl;

  vector<string> functions = { "R,8,L,12,R,8", "R,12,L,8,R,10", "R,8,L,8,L,8,R,8,R,10" };

  // A,B,B,A,C,A,A,C,B,C
  string mainRoutine = path;
  
  for (char c = 'A'; c <= 'C'; c++)
  {
    int startPos = 0;
    while (startPos != string::npos)
    {
      startPos = mainRoutine.find(functions[c - 'A'], startPos);
      if (startPos != string::npos)
        mainRoutine.replace(startPos, functions[c - 'A'].size(), string(1, c));
    }
  }

  v[0] = 2;

  RunProgram(out, v, mainRoutine, functions, true);

  return 0;
}
