#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <thread>
#include <cmath>
#include <assert.h>
using namespace std;

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

class NIC;

vector<NIC> nics;
int NAT_X = 0, NAT_Y = 0;
vector<int> yVals;

class NIC
{
public:
  NIC(const vector<long long> & input) : input(input) {}

  long long GetVal(long long pos, int mode)
  {
    if (mode == 0)
      return input[pos];

    if (mode == 1)
      return pos;

    if (mode == 2)
      return input[relativeBase + pos];

    return 0;
  }

  void SetVal(long long pos, int mode, long long val)
  {
    assert(mode != 1);

    if (mode == 0)
      input[pos] = val;
    else if (mode == 2)
      input[pos + relativeBase] = val;
  }

  void RunProgram(int address)
  {
    int opcode, mode1, mode2, mode3;
    ReadOpcode(input[instrIndex], opcode, mode1, mode2, mode3);

    if (opcode == 99)
      return;

    if (opcode == 1)
    {
      long long val1 = GetVal(input[instrIndex + 1], mode1);
      long long val2 = GetVal(input[instrIndex + 2], mode2);

      SetVal(input[instrIndex + 3], mode3, val1 + val2);
      instrIndex += 4;
    }
    else if (opcode == 2)
    {
      long long val1 = GetVal(input[instrIndex + 1], mode1);
      long long val2 = GetVal(input[instrIndex + 2], mode2);

      SetVal(input[instrIndex + 3], mode3, val1 * val2);
      instrIndex += 4;
    }
    else if (opcode == 3)
    {
      if (boot)
      {
        SetVal(input[instrIndex + 1], mode1, address);
        boot = false;
      }
      else
      {
        triesToReceivePackages = true;

        if (packetQueue.empty())
        {
          isEmptyPackageQueue = true;

          SetVal(input[instrIndex + 1], mode1, -1);

          if (isEmptyPackageQueue && triesToReceivePackages && !triesToSendPackages)
          {
            isIdle = true;

            bool allNetworksAreIdle = all_of(nics.begin(), nics.end(), [](auto & nic)
                                             { return nic.isIdle; });

            if (allNetworksAreIdle)
            {
              nics[0].packetQueue.push({ NAT_X, NAT_Y });
              
              for (int i = 0; i < yVals.size(); i++)
              {
                if (yVals[i] == NAT_Y)
                {
                  cout << "NAT_Y: " << NAT_Y << endl;
                  return;
                }
              }

              yVals.push_back(NAT_Y);
            }
          }

          return;
        }
        else
        {
          isEmptyPackageQueue = false;
          triesToSendPackages = false;

          int val = 0;
          if (readX)
          {
            val = packetQueue.front().first;
            readX = false;
          }
          else
          {
            val = packetQueue.front().second;
            readX = true;

            packetQueue.pop();
          }

          SetVal(input[instrIndex + 1], mode1, val);
        }
      }

      instrIndex += 2;
    }
    else if (opcode == 4)
    {
      triesToSendPackages = true;

      int output = GetVal(input[instrIndex + 1], mode1);

      if (receiveDestAddress)
      {
        destAddress = output;
        receiveDestAddress = false;
        receiveDestX = true;
      }
      else if (receiveDestX)
      {
        destX = output;
        receiveDestX = false;
        receiveDestY = true;
      }
      else if (receiveDestY)
      {
        destY = output;
        receiveDestY = false;
        receiveDestAddress = true;

        if (destAddress == 255)
        {
          // Part 1
          cout << destX << endl;

          // Part 2
          NAT_X = destX;
          NAT_Y = destY;

          return;
        }

        nics[destAddress].packetQueue.push({ destX, destY });
      }

      instrIndex += 2;
    }
    else if (opcode == 5)
    {
      long long val1 = GetVal(input[instrIndex + 1], mode1);
      long long val2 = GetVal(input[instrIndex + 2], mode2);

      if (val1)
        instrIndex = val2;
      else
        instrIndex += 3;
    }
    else if (opcode == 6)
    {
      long long val1 = GetVal(input[instrIndex + 1], mode1);
      long long val2 = GetVal(input[instrIndex + 2], mode2);

      if (!val1)
        instrIndex = val2;
      else
        instrIndex += 3;
    }
    else if (opcode == 7)
    {
      long long val1 = GetVal(input[instrIndex + 1], mode1);
      long long val2 = GetVal(input[instrIndex + 2], mode2);

      SetVal(input[instrIndex + 3], mode3, val1 < val2);
      instrIndex += 4;
    }
    else if (opcode == 8)
    {
      long long val1 = GetVal(input[instrIndex + 1], mode1);
      long long val2 = GetVal(input[instrIndex + 2], mode2);

      SetVal(input[instrIndex + 3], mode3, val1 == val2);
      instrIndex += 4;
    }
    else if (opcode == 9)
    {
      long long val = GetVal(input[instrIndex + 1], mode1);

      relativeBase += val;
      instrIndex += 2;
    }
  }

  vector<long long> input;
  queue<pair<int, int>> packetQueue;
  int instrIndex{ 0 };
  long long relativeBase{ 0 };
  bool boot{ true };
  bool receiveDestAddress{ true };
  bool receiveDestX{ false };
  bool receiveDestY{ false };
  bool readX{ true };
  bool isIdle{ false };
  bool isEmptyPackageQueue{ false };
  bool triesToReceivePackages{ false };
  bool triesToSendPackages{ false };
  int destAddress{ 0 };
  int destX{ 0 };
  int destY{ 0 };
};

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

  v.resize(1000000);
  
  nics.resize(50, { v });

  while (true)
  {
    for (int i = 0; i < nics.size(); i++)
    {
      nics[i].RunProgram(i);
    }
  }
  
  return 0;
}
