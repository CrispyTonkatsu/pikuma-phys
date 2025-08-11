#include <iostream>
#include <ostream>
#include "Physics/matN.h"

int main() {
  {
    std::cout << "Basic 2x2 test" << std::endl;

    mat2 a{
      mat2::WithData({{
        {1.5f, 2.0f},
        {0.5f, 1.f},
      }}
      )
    };

    mat2 b{
      mat2::WithData({{
        {2.f, 0.f},
        {0.f, 2.f},
      }}
      )
    };

    auto c = a * b;

    // Checking the output matrix has the right dimensions
    static_assert(!std::is_same_v<decltype(c), matN<int, 2, 2>>);
    static_assert(std::is_same_v<decltype(c), matN<float, 2, 2>>);

    // Checking calculation
    std::cout << c << std::endl;
  }

  {
    std::cout << "Basic 3x3 test" << std::endl;

    mat3 a{
      mat3::WithData({{
        {3, 3, 8},
        {3, 4, 4},
        {5, 2, 6},
      }}
      )
    };

    mat3 b{
      mat3::WithData({{
        {4, 4, 8},
        {4, 8, 8},
        {3, 8, 4},
      }}
      )
    };

    auto c = a * b;

    // Checking the output matrix has the right dimensions
    static_assert(std::is_same_v<decltype(c), matN<float, 3, 3>>);

    // Checking calculation
    std::cout << c << std::endl;
  }

  {
    std::cout << "2D Affine transformation test" << std::endl;

    mat3 a{
      mat3::WithData({{
        {2, 0, 0},
        {0, 2, 0},
        {3, -1, 1},
      }}
      )
    };

    vec3 b{
      vec3::WithData({{
        {2, 2, 1},
      }}
      )
    };

    auto c = a * b;

    // Checking the output matrix has the right dimensions
    static_assert(std::is_same_v<decltype(c), matN<float, 1, 3>>);

    // Checking calculation
    std::cout << c << std::endl;
  }

  {
    std::cout << "Negative, transpose, Mult test" << std::endl;

    vec3 a{vec3::WithData({{{2, 2, 1}}})};

    auto c = a * -a.transpose();

    // Checking the output matrix has the right dimensions
    static_assert(std::is_same_v<decltype(c), matN<float, 3, 3>>);

    // Checking calculation
    std::cout << c << std::endl;
  }

  return 0;
}
