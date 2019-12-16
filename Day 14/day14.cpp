#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Chemical
{
  Chemical(const string & name, int quantity) : name(name), quantity(quantity) {}

  bool operator<(const Chemical & chemical) const
  {
    return tie(name, quantity) < tie(chemical.name, chemical.quantity);
  }

  string name;
  long long quantity;
};

map<Chemical, vector<Chemical>> chemicalsMap;
map<string, int> leftOversMap;
vector<Chemical> chemicalsToReduce;
vector<string> alreadyReducedChems;

int GetQuantityMultiplier(const Chemical & neededChemical, const Chemical & outputChemical)
{
  long long neededQuantity = neededChemical.quantity;
  if (leftOversMap.find(outputChemical.name) != leftOversMap.end())
  {
    neededQuantity -= leftOversMap[outputChemical.name];
    if (neededQuantity < 0)
      neededQuantity = 0;
  }

  if (neededQuantity < outputChemical.quantity)
  {
    leftOversMap[neededChemical.name] += outputChemical.quantity - neededQuantity;
    return 1;
  }
  else if (neededQuantity % outputChemical.quantity == 0)
  {
    return neededQuantity / outputChemical.quantity;
  }
  else if (neededQuantity % outputChemical.quantity != 0)
  {
    int quantity = neededQuantity / outputChemical.quantity + 1;
    leftOversMap[neededChemical.name] += quantity * outputChemical.quantity - neededQuantity;

    return quantity;
  }

  return 1;
}

int GetQuantityMultiplier(int quantity, const Chemical & outputChemical)
{
  if (quantity < outputChemical.quantity)
  {
    return 1;
  }
  else if (quantity % outputChemical.quantity == 0)
  {
    return quantity / outputChemical.quantity;
  }
  else if (quantity % outputChemical.quantity != 0)
  {
    return quantity / outputChemical.quantity + 1;
  }

  return 1;
}

bool Reduce(const Chemical & inputChemical)
{
  for (auto & outputChem : chemicalsMap)
  {
    if (outputChem.first.name == inputChemical.name)
    {
      if (outputChem.second.size() == 1 && outputChem.second[0].name == "ORE")
        return false;

      int multiplier = GetQuantityMultiplier(inputChemical, outputChem.first);

      for (auto & inputChem : outputChem.second)
      {
        inputChem.quantity *= multiplier;

        auto it = std::find_if(chemicalsToReduce.begin(), chemicalsToReduce.end(), [&inputChem](auto & chemToReduce) 
                              { return inputChem.name == chemToReduce.name; });

        if (it == chemicalsToReduce.end())
          chemicalsToReduce.push_back(inputChem);
        else
          it->quantity += inputChem.quantity;
      }

      return true;
    }
  }

  return false;
}

long long Run(long long fuelToTest)
{
  chemicalsToReduce.clear();
  alreadyReducedChems.clear();

  for (auto & fuelChem : chemicalsMap)
  {
    if (fuelChem.first.name == "FUEL")
    {
      for (auto & inputChem : fuelChem.second)
      {
        inputChem.quantity *= fuelToTest;
        chemicalsToReduce.push_back(inputChem);
      }

      break;
    }
  }

  while (true)
  {
    auto tempChemicalsToReduce{ chemicalsToReduce };

    bool canStillReduce = false;

    for (auto & chemical : tempChemicalsToReduce)
    {
      for (auto & chem : chemicalsMap)
      {
        if (chem.first.name == chemical.name)
        {
          bool alreadyReduced = any_of(alreadyReducedChems.begin(), alreadyReducedChems.end(), [&chemical](auto & chemToCheck)
                                      { return chemical.name == chemToCheck; });

          if (!alreadyReduced && Reduce(chemical))
          {
            alreadyReducedChems.push_back(chemical.name);
            canStillReduce = true;
          }

          break;
        }
      }
    }

    if (!canStillReduce)
      break;
  }

  long long count = 0;

  for (auto & chemicalToReduce : chemicalsToReduce)
  {
    bool alreadyReduced = any_of(alreadyReducedChems.begin(), alreadyReducedChems.end(), [&chemicalToReduce](auto& chemToCheck)
                                { return chemicalToReduce.name == chemToCheck; });
    if (alreadyReduced)
      continue;

    for (auto & chem : chemicalsMap)
    {
      if (chem.first.name == chemicalToReduce.name)
      {
        count += chem.second[0].quantity * GetQuantityMultiplier(chemicalToReduce.quantity, chem.first);
        break;
      }
    }
  }

  return count;
}

int main()
{
  ifstream in("in.txt");

  while (!in.eof())
  {
    vector<Chemical> chemicals;

    string line;
    getline(in, line);

    size_t startPos = 0;
    size_t spacePos = line.find(' ', startPos);
    size_t equalPos = line.find('=', 0);

    while (startPos < equalPos)
    {
      int quantity = stoi(line.substr(startPos, spacePos - startPos));
      string name;

      size_t commaPos = line.find(',', spacePos);
      if (commaPos == string::npos)
      {
        name = line.substr(spacePos + 1, equalPos - spacePos - 2);

        startPos = line.find(' ', equalPos) + 1;
        spacePos = line.find(' ', startPos + 1);
      }
      else
      {
        name = line.substr(spacePos + 1, commaPos - spacePos - 1);

        startPos = commaPos + 2;
        spacePos = line.find(' ', startPos);
      }

      chemicals.emplace_back(name, quantity);
    }

    int quantity = stoi(line.substr(startPos, spacePos - startPos));
    string name = line.substr(spacePos + 1);

    Chemical chemical(name, quantity);
    chemicalsMap[chemical] = chemicals;
  }

  // Part 1
  long long oreCount = Run(1);
  cout << oreCount << endl;

  // Part 2
  long long a = 1;
  long long b = 100000000000;

  while (a <= b)
  {
     long long fuelToTest = a + (b - a) / 2;

     long long oreCount = Run(fuelToTest);

     if (oreCount == 1000000000000)
     {
       cout << fuelToTest << endl;
     }
     else if (oreCount < 1000000000000)
     {
       a = fuelToTest + 1;
     }
     else
     {
       b = fuelToTest - 1;
     }
  }

  return 0;
}
