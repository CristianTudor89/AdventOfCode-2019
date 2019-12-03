#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <assert.h>
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

  v[1] = 12;
  v[2] = 2;

  int i = 0;
  while (i < v.size())
  {
    if (v[i] == 99)
      break;
    else if (v[i] == 1)
      v[v[i + 3]] = v[v[i + 1]] + v[v[i + 2]];
    else if (v[i] == 2)
      v[v[i + 3]] = v[v[i + 1]] * v[v[i + 2]];

    i += 4;
  }

  out << v[0];
}

void PartTwo()
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

  for (int i = 0; i < 100; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      int first = i, second = j;

      vector<int> vec{ v };
      vec[1] = i;
      vec[2] = j;

      int k = 0;
      while (k + 3 < vec.size())
      {
        if (vec[k] == 99)
          break;
        else if (vec[k] == 1)
          vec[vec[k + 3]] = vec[vec[k + 1]] + vec[vec[k + 2]];
        else if (vec[k] == 2)
          vec[vec[k + 3]] = vec[vec[k + 1]] * vec[vec[k + 2]];

        if (vec[0] == 19690720)
        {
          out << first * 100 + second;
          i = 100;
          j = 100;
          break;
        }

        k += 4;
      }
    }
  }
}
