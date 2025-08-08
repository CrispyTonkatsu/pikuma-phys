#include <iostream>
#include <ostream>
#include "Physics/matN.h"

int main() {
  std::cout << "Basic 2x2 test" << std::endl;

  matN<float, 2, 2> a{{{
    {1.5f, 2.0f},
    {0.5f, 1.f},
  }}};

  matN<int, 2, 2> b{{{
    {2, 0},
    {0, 2},
  }}};

  std::cout << a * b << std::endl;

  // std::cout << "Basic 3x3 test" << std::endl;

  return 0;
}
