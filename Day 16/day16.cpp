#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
using namespace std;

vector<int> ProcessPhase(const vector<int> & inputSignal)
{
  const int signalSize = inputSignal.size();

  vector<int> partialSums(signalSize);
  vector<int> outputSignal(signalSize);

  partialSums[0] = inputSignal[0];

  for (int i = 1; i < signalSize; i++)
  {
    partialSums[i] = partialSums[i - 1] + inputSignal[i];
  }

  for (int repeatCount = 1; repeatCount <= signalSize; repeatCount++)
  {
    int sum1 = 0, sum2 = 0;

    for (int i = 0; i < signalSize; i += 4 * repeatCount)
    {
      int multiplyFactor = 2 * (repeatCount - 1);
      if (multiplyFactor >= signalSize)
        multiplyFactor = signalSize - 1;

      if (i == 0 && repeatCount == 1)
        sum1 = partialSums[0];
      else
      {
        if (i + repeatCount - 1 < signalSize)
        {
          int val = min(i + 2 * (repeatCount - 1), signalSize - 1);
          sum1 += partialSums[val] - partialSums[i + repeatCount - 2];
        }
      }
    }

    for (int i = 2 + 3 * (repeatCount - 1); i < signalSize; i += 4 * repeatCount)
    {
      int val = min(i + repeatCount - 1, signalSize - 1);
      sum2 += partialSums[val] - partialSums[i - 1];
    }

    outputSignal[repeatCount - 1] = abs(sum1 - sum2) % 10;
  }

  return outputSignal;
}

int main()
{
  ifstream in("in.txt");

  string s;
  in >> s;

  // Part 1
  //vector<int> input(s.size());

  // Part 2
  vector<int> input(s.size() * 10000);

  for (int i = 0; i < input.size(); i++)
  {
    input[i] = s[i % s.size()] - 48;
  }

  stringstream ss;
  copy(input.begin(), input.begin() + 7, ostream_iterator<int>(ss));

  int offset = stoi(ss.str());

  vector<int> output;
  int phaseCount = 100;

  for (int i = 0; i < phaseCount; i++)
  {
    output = ProcessPhase(input);
    input = output;
  }

  for (int i = offset; i < offset + 8; i++)
    cout << output[i];

  return 0;
}
