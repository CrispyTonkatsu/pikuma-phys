#ifndef MATN_H
#define MATN_H

#include <array>
#include <cstddef>
#include <ostream>

template<typename T, typename Y>
using MULT_RETURN = decltype(std::declval<T>() * std::declval<Y>());

template<typename T, size_t W, size_t H>
class matN;

template<typename T, size_t W, size_t H>
std::ostream& operator<<(std::ostream& os, matN<T, W, H> mat) {
  for (size_t i = 0; i < W; i++) {
    for (size_t j = 0; j < H - 1; j++) {
      os << mat.at_mut(i, j) << " ";
    }
    os << mat.at(i, H - 1) << std::endl;
  }

  return os;
}

template<typename T, size_t W, size_t H>
class matN {
  // TODO: Convert this from a 2d array to a 1d array for locality benefits (if
  // there are any)
  std::array<std::array<T, H>, W> values{};

public:

  explicit matN(): values() {}

  explicit matN(const std::array<std::array<T, H>, W>& values):
      values(values) {}

  const T& at(size_t x, size_t y) const { return values[x][y]; }

  T& at_mut(size_t x, size_t y) { return values[y][x]; }

  template<typename Y, size_t oW>
  auto operator*(const matN<Y, oW, W>& other) const
    -> matN<MULT_RETURN<T, Y>, oW, H> {

    using R = MULT_RETURN<T, Y>;
    matN<R, oW, H> output{};

    for (size_t x = 0; x < oW; x++) {
      for (size_t y = 0; y < H; y++) {
        for (size_t i = 0; i < W; i++) {
          output.at_mut(x, y) += at(i, y) * other.at(x, i);
        }
      }
    }

    return output;
  }
};

template<typename T, size_t N>
using vecN = matN<T, 1, N>;

#endif
