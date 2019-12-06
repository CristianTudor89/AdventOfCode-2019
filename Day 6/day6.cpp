#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <cmath>
#include <assert.h>
using namespace std;

int main()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  unordered_map<string, vector<string>> orbits;

  while (!in.eof())
  {
    string line;
    in >> line;

    size_t pos = line.find(")");
    string object = line.substr(0, pos);
    string inOrbit = line.substr(pos + 1, line.size());

    orbits.insert({ inOrbit, { object } });
  }

  int count = 0;
  for (const auto& [inOrbit, center] : orbits)
  {
    if (center.size() > 0)
    {
      int i = 1;
      string newOrbit = center[0];
      while (orbits[newOrbit].size() > 0)
      {
        newOrbit = orbits[newOrbit][0];
        orbits[inOrbit].push_back(newOrbit);
        i++;
      }

      count += i;
    }
  }

  // Part 1
  out << count << endl;

  // Part 2
  auto v1 = orbits["YOU"];
  auto v2 = orbits["SAN"];

  auto it = std::mismatch(v1.rbegin(), v1.rend(), v2.rbegin(), v2.rend());

  out << (v1.rend() - it.first) + (v2.rend() - it.second);

  return 0;
}
