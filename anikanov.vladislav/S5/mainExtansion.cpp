#include "mainExtansion.hpp"
#include <fstream>
#include <iostream>

bool anikanov::loadTreeFromFile(std::string &fileName, BinarySearchTree< int, std::string, std::less<> > &tree)
{
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "Could not open file " << fileName << "\n";
    return false;
  }

  int key;
  std::string value;
  while (file >> key >> value) {
    tree.push(key, value);
  }
  file.close();
  return true;
}

void anikanov::printInvalidUsage(std::ostream &os, char **argv)
{
  os << "Usage: " << argv[0] << " [ascending|descending|breadth] filename\n";
}

void anikanov::printInvalidMode(std::ostream &os)
{
  os << "Invalid traversal mode\n";
}

void anikanov::printEmpty(std::ostream &os)
{
  os << "<EMPTY>\n";
}
