#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

void PartOne();
void PartTwo();

int main()
{
  PartTwo();

  return 0;
}

void PartOne()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<int> fuels;
  int sum = 0;

  while (!in.eof())
  {
    int fuel;
    in >> fuel;
    fuel = fuel / 3 - 2;
    sum += fuel;
  }

  out << sum;
}

void PartTwo()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<int> fuels;
  int sum = 0;

  while (!in.eof())
  {
    int fuel;
    in >> fuel;

    while (fuel > 0)
    {
      fuel = fuel / 3 - 2;
      if (fuel > 0)
        sum += fuel;
    }

  }

  out << sum;
}
