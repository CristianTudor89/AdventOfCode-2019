#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <assert.h>
using namespace std;

long long CMMDC(long long a, long long b)
{
  while (a != b)
  {
    if (a > b)
      a -= b;
    else
      b -= a;
  }

  return a;
}

long long CMMMC(long long a, long long b)
{
  return a * b / CMMDC(a, b);
}

struct Vector3
{
  bool operator==(const Vector3 & vec) const
  {
    return x == vec.x && y == vec.y && z == vec.z;
  }

  bool operator!=(const Vector3 & vec) const
  {
    return !(*this == vec);
  }

  int GetAbsLength() const { return abs(x) + abs(y) + abs(z); }

  int x;
  int y;
  int z;
};

class Moon
{
public:
  Moon(const Vector3 & pos) : position(pos) {}

  Vector3 GetPosition() const { return position; }
  Vector3 GetVelocity() const { return velocity; }

  void UpdatePosition()
  {
    position.x += velocity.x;
    position.y += velocity.y;
    position.z += velocity.z;
  }

  void ApplyGravity(Moon & moon)
  {
    if (position.x < moon.position.x)
      velocity.x++;
    else if (position.x > moon.position.x)
      velocity.x--;

    if (position.y < moon.position.y)
      velocity.y++;
    else if (position.y > moon.position.y)
      velocity.y--;

    if (position.z < moon.position.z)
      velocity.z++;
    else if (position.z > moon.position.z)
      velocity.z--;
  }

  int GetPotentialEnergy() const { return position.GetAbsLength(); }
  int GetKineticEnergy() const { return velocity.GetAbsLength(); }

private:
  Vector3 position{};
  Vector3 velocity{};
};

bool IsAlignedOnAxis(const vector<Moon> & initialState, const vector<Moon> & moons, int axisIndex)
{
  if (axisIndex == 0)
  {
    bool isAligned = true;

    for (int i = 0; i < 4; i++)
    {
      if ((initialState[i].GetPosition().x != moons[i].GetPosition().x) ||
          (initialState[i].GetVelocity().x != moons[i].GetVelocity().x))
      {
        isAligned = false;
        break;
      }
    }

    if (isAligned)
      return true;
  }
  else if (axisIndex == 1)
  {
    bool isAligned = true;

    for (int i = 0; i < 4; i++)
    {
      if ((initialState[i].GetPosition().y != moons[i].GetPosition().y) ||
          (initialState[i].GetVelocity().y != moons[i].GetVelocity().y))
      {
        isAligned = false;
        break;
      }
    }

    if (isAligned)
      return true;
  }
  else if (axisIndex == 2)
  {
    bool isAligned = true;

    for (int i = 0; i < 4; i++)
    {
      if ((initialState[i].GetPosition().z != moons[i].GetPosition().z) ||
          (initialState[i].GetVelocity().z != moons[i].GetVelocity().z))
      {
        isAligned = false;
        break;
      }
    }

    if (isAligned)
      return true;
  }

  return false;
}

void PartOne()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<Moon> moons;

  while (!in.eof())
  {
    string s;
    getline(in, s);

    size_t pos1 = s.find('=');
    size_t pos2 = s.find(',');
    int x = std::stoi(s.substr(pos1 + 1, pos2 - pos1 - 1));

    pos1 = s.find('=', pos1 + 1);
    pos2 = s.find(',', pos2 + 1);
    int y = std::stoi(s.substr(pos1 + 1, pos2 - pos1 - 1));

    pos1 = s.find('=', pos1 + 1);
    pos2 = s.find('>', pos2 + 1);
    int z = std::stoi(s.substr(pos1 + 1, pos2 - pos1 - 1));

    moons.push_back(Vector3{ x, y, z });
  }

  int timeSteps = 1000;
  for (int step = 0; step < timeSteps; step++)
  {
    for (int i = 0; i < moons.size() - 1; i++)
    {
      for (int j = i + 1; j < moons.size(); j++)
      {
        moons[i].ApplyGravity(moons[j]);
        moons[j].ApplyGravity(moons[i]);
      }
    }

    for (int i = 0; i < moons.size(); i++)
      moons[i].UpdatePosition();
  }

  int totalEnergy = 0;
  for (int i = 0; i < moons.size(); i++)
  {
    totalEnergy += moons[i].GetPotentialEnergy() * moons[i].GetKineticEnergy();
  }

  cout << totalEnergy << endl;
}

void PartTwo()
{
  ifstream in("in.txt");
  ofstream out("out.txt");

  vector<Moon> moons;

  while (!in.eof())
  {
    string s;
    getline(in, s);

    size_t pos1 = s.find('=');
    size_t pos2 = s.find(',');
    int x = std::stoi(s.substr(pos1 + 1, pos2 - pos1 - 1));

    pos1 = s.find('=', pos1 + 1);
    pos2 = s.find(',', pos2 + 1);
    int y = std::stoi(s.substr(pos1 + 1, pos2 - pos1 - 1));

    pos1 = s.find('=', pos1 + 1);
    pos2 = s.find('>', pos2 + 1);
    int z = std::stoi(s.substr(pos1 + 1, pos2 - pos1 - 1));

    moons.push_back(Vector3{ x, y, z });
  }

  vector<Moon> initialState{ moons };
  vector<long long> timeSteps(3, 0);

  for (int k = 0; k < 3; k++)
  {
    moons = initialState;

    do
    {
      for (int i = 0; i < moons.size() - 1; i++)
      {
        for (int j = i + 1; j < moons.size(); j++)
        {
          moons[i].ApplyGravity(moons[j]);
          moons[j].ApplyGravity(moons[i]);
        }
      }

      for (int i = 0; i < moons.size(); i++)
        moons[i].UpdatePosition();

      timeSteps[k]++;
    } 
    while (!IsAlignedOnAxis(initialState, moons, k));
  }

  long long result = CMMMC(CMMMC(timeSteps[0], timeSteps[1]), timeSteps[2]);

  cout << result << endl;
}

int main()
{
  PartTwo();
}
