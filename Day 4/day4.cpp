#include <iostream>
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
  int minVal = 138241;
  int maxVal = 674034;

  int count = 0;
  for (int i = minVal; i <= maxVal; i++)
  {
    string s = to_string(i);
    bool a = false, b = true;

    for (int j = 0; j < 5; j++)
      if (s[j] == s[j + 1])
      {
        a = true;
        break;
      }

    for (int j = 0; j < 5; j++)
      if (s[j] > s[j + 1])
      {
        b = false;
        break;
      }

    if (a && b)
      count++;
  }

  std::cout << count;
}

void PartTwo()
{
  int minVal = 138241;
  int maxVal = 674034;

  int count = 0;
  for (int i = minVal; i <= maxVal; i++)
  {
    string s = to_string(i);
    bool a = false, b = true;

    for (int j = 0; j < 5; j++)
      if (s[j] == s[j + 1])
      {
        if (j == 0 && s[j + 1] != s[j + 2])
        {
          a = true;
          break;
        }
        else if (j == 4 && s[j - 1] != s[j])
        {
          a = true;
          break;
        }
        else if (j > 0 && j < 4 && s[j - 1] != s[j] && s[j + 1] != s[j + 2])
        {
          a = true;
          break;
        }
      }

    for (int j = 0; j < 5; j++)
      if (s[j] > s[j + 1])
      {
        b = false;
        break;
      }

    if (a && b)
      count++;
  }

  std::cout << count;
}
