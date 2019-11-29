#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
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

  int sum = 0;

  while (!in.eof())
  {
    string s;
    in >> s;

    if (!s.empty())
    {
      assert(s[0] == '+' || s[0] == '-');
      sum += std::stoi(s);
    }
  }

  out << sum;
}

void PartTwo()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<int> freqChanges, partialFreqs{ 0 };

  while (!in.eof())
  {
    string s;
    in >> s;

    if (!s.empty())
    {
      assert(s[0] == '+' || s[0] == '-');
      freqChanges.push_back(std::stoi(s));
    }
  }
  
  int sum = 0, index = 0;

  while (true)
  {
    sum += freqChanges[index];

    bool found = std::any_of(partialFreqs.begin(), partialFreqs.end(), [sum](int freq) { return sum == freq; });
    if (found)
      break;

    partialFreqs.push_back(sum);
    if (index == freqChanges.size() - 1)
      index = 0;
    else
      index++;
  }

  out << sum;
}
