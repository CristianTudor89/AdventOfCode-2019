#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <assert.h>
using namespace std;

void PartOne()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<vector<vector<int>>> layers;
  int pixelCount = 0, rowCount = 0;

  while (!in.eof())
  {
    char i;
    in >> i;
    if (i == 0)
      break;

    i = i - 48;

    if (pixelCount % 150 == 0)
    {
      layers.push_back({});
      layers[pixelCount / 150].push_back({});
      layers[pixelCount / 150][0].push_back(i);
      rowCount = 0;
    }
    else
    {
      layers[pixelCount / 150][rowCount].push_back(i);
      if (layers[pixelCount / 150][rowCount].size() == 25)
      {
        rowCount++;
        if (rowCount < 6)
          layers[pixelCount / 150].push_back({});
      }
    }

    pixelCount++;
  }

  int min = 100000, result = 0, index = 0;

  for (int i = 0; i < layers.size(); i++)
  {
    int count = 0;
    for (int j = 0; j < 6; j++)
      for (int k = 0; k < 25; k++)
      {
        if (layers[i][j][k] == 0)
          count++;
      }

    if (count < min)
    {
      min = count;
      index = i;
    }
  }

  int count1 = 0, count2 = 0;

  for (int j = 0; j < 6; j++)
    for (int k = 0; k < 25; k++)
    {
      if (layers[index][j][k] == 1)
        count1++;
      else if (layers[index][j][k] == 2)
        count2++;
    }

  out << count1 * count2;
}

void PartTwo()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<vector<vector<int>>> layers;
  int pixelCount = 0, rowCount = 0;

  while (!in.eof())
  {
    char i;
    in >> i;
    if (i == 0)
      break;

    i = i - 48;

    if (pixelCount % 150 == 0)
    {
      layers.push_back({});
      layers[pixelCount / 150].push_back({});
      layers[pixelCount / 150][0].push_back(i);
      rowCount = 0;
    }
    else
    {
      layers[pixelCount / 150][rowCount].push_back(i);
      if (layers[pixelCount / 150][rowCount].size() == 25)
      {
        rowCount++;
        if (rowCount < 6)
          layers[pixelCount / 150].push_back({});
      }
    }

    pixelCount++;
  }

  int finalImage[6][25];

  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 25; j++)
    {
      for (int k = 0; k < layers.size(); k++)
      {
        if (layers[k][i][j] == 2)
          continue;

        finalImage[i][j] = layers[k][i][j];
        break;
      }
    }

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 25; j++)
    {
      if (finalImage[i][j] == 1)
        out << finalImage[i][j];
      else
        out << " ";
    }

    out << endl;
  }
}

int main()
{
  PartTwo();

  return 0;
}
