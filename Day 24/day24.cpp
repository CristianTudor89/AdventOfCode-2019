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

const int sizeX = 5;
const int sizeY = 5;

using Matrix = vector<vector<char>>;

void PrintMatrix(const Matrix & matrix)
{
  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      cout << matrix[i][j];
    }

    cout << endl;
  }

  cout << endl;
}

bool IsEqualLayout(const Matrix & matrix1, const Matrix & matrix2)
{
  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      if (matrix1[i][j] != matrix2[i][j])
        return false;
    }
  }

  return true;
}

int GetCount(char tile)
{
  if (tile == '#')
    return 1;

  return 0;
}

int GetAdjacentCount(const Matrix & matrix, int i, int j)
{
  int count = 0;

  if (i < 4)
    count += GetCount(matrix[i + 1][j]);

  if (i > 0)
    count += GetCount(matrix[i - 1][j]);

  if (j < 4)
    count += GetCount(matrix[i][j + 1]);

  if (j > 0)
    count += GetCount(matrix[i][j - 1]);

  return count;
}

int GetAdjacentCountInChild(const Matrix & matrix, int i, int j)
{
  int count = 0;

  if (i == 1 && j == 2)
  {
    for (int k = 0; k < sizeY; k++)
      count += GetCount(matrix[0][k]);
  }

  if (i == 3 && j == 2)
  {
    for (int k = 0; k < sizeY; k++)
      count += GetCount(matrix[4][k]);
  }

  if (i == 2 && j == 1)
  {
    for (int k = 0; k < sizeX; k++)
      count += GetCount(matrix[k][0]);
  }

  if (i == 2 && j == 3)
  {
    for (int k = 0; k < sizeX; k++)
      count += GetCount(matrix[k][4]);
  }

  return count;
}

int GetAdjacentCountInParent(const Matrix & matrix, int i, int j)
{
  int count = 0;

  if (i == 0)
    count += GetCount(matrix[1][2]);

  if (i == 4)
    count += GetCount(matrix[3][2]);

  if (j == 0)
    count += GetCount(matrix[2][1]);

  if (j == 4)
    count += GetCount(matrix[2][3]);

  return count;
}

int GetAdjacentCountWithLevels(const vector<Matrix> & levels, int level, int i, int j)
{
  int count = GetAdjacentCount(levels[level], i, j);
  count += GetAdjacentCountInChild(levels[level + 1], i, j);
  count += GetAdjacentCountInParent(levels[level - 1], i, j);

  return count;
}

void TransformLayout(Matrix & matrix)
{
  auto tempMatrix{ matrix };

  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      int bugsCount = GetAdjacentCount(tempMatrix, i, j);

      if (tempMatrix[i][j] == '#' && bugsCount != 1)
      {
        matrix[i][j] = '.';
      }
      else if (tempMatrix[i][j] == '.' && (bugsCount == 1 || bugsCount == 2))
      {
        matrix[i][j] = '#';
      }
    }
  }
}

void TransformLayoutWithLevels(vector<Matrix> & currentLevels, const vector<Matrix> & originalLevels, int level)
{
  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      int bugsCount = GetAdjacentCountWithLevels(originalLevels, level, i, j);

      if (originalLevels[level][i][j] == '#' && bugsCount != 1)
      {
        currentLevels[level][i][j] = '.';
      }
      else if (originalLevels[level][i][j] == '.' && (bugsCount == 1 || bugsCount == 2))
      {
        currentLevels[level][i][j] = '#';
      }
    }
  }
}

int GetBiodiversity(const Matrix & matrix)
{
  int sum = 0;

  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      if (matrix[i][j] == '#')
      {
        sum += 1 << (i * sizeX + j);
      }
    }
  }

  return sum;
}

int GetBugCount(const Matrix & matrix)
{
  int sum = 0;

  for (int i = 0; i < sizeX; i++)
  {
    for (int j = 0; j < sizeY; j++)
    {
      if (matrix[i][j] == '#')
      {
        sum++;
      }
    }
  }

  return sum;
}

void PartOne(const Matrix & matrix)
{
  auto currentMatrix{ matrix };

  vector<Matrix> layouts;
  layouts.push_back(currentMatrix);

  bool stop = false;
  while (!stop)
  {
    TransformLayout(currentMatrix);
    layouts.push_back(currentMatrix);

    for (int i = 0; i < layouts.size() - 1; i++)
    {
      for (int j = i + 1; j < layouts.size(); j++)
      {
        if (IsEqualLayout(layouts[i], layouts[j]))
        {
          cout << GetBiodiversity(layouts[i]) << endl;

          i = layouts.size();
          stop = true;
          break;
        }
      }
    }
  }
}

void PartTwo(Matrix & matrix)
{
  matrix[2][2] = '?';

  Matrix emptyGrid(sizeX, vector<char>(sizeY, '.'));
  emptyGrid[2][2] = '?';

  int bugCount = 0, totalBugCount = 0, minutes = 200;
  int lowestLevel = minutes, highestLevel = minutes + 2;

  vector<Matrix> levels;

  for (int i = 0; i <= minutes; i++)
    levels.push_back(emptyGrid);

  levels.push_back(matrix);

  for (int i = 0; i <= minutes; i++)
    levels.push_back(emptyGrid);

  vector<Matrix> currentLevels{ levels };

  for (int minute = 0; minute < minutes; minute++)
  {
    for (int level = lowestLevel; level <= highestLevel; level++)
    {
      TransformLayoutWithLevels(currentLevels, levels, level);
    }

    lowestLevel--;
    highestLevel++;

    levels = currentLevels;
  }

  for (int i = 1; i <= minutes * 2 + 1; i++)
  {
    totalBugCount += GetBugCount(levels[i]);
  }

  cout << totalBugCount << endl;
}

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  Matrix matrix(sizeX, vector<char>(sizeY, '.'));
  int i = 0, j = 0;

  while (!in.eof())
  {
    char c;
    in >> c;

    matrix[i][j] = c;
    j++;

    if (j == sizeY)
    {
      j = 0;
      i++;
      if (i == sizeX)
        break;
    }
  }

  PrintMatrix(matrix);

  PartOne(matrix);
  PartTwo(matrix);

  return 0;
}
