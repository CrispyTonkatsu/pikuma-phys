#ifndef MATN_H
#define MATN_H

#include <array>
#include <cstddef>
#include <iostream>

template<typename T, typename Y>
using MULT_RETURN = decltype(std::declval<T>() * std::declval<Y>());

template<typename T, size_t W, size_t H>
class matN;

template<typename T, size_t W, size_t H>
std::ostream& operator<<(std::ostream& os, matN<T, W, H> mat) {
  for (size_t y = 0; y < H; y++) {
    for (size_t x = 0; x < W - 1; x++) {
      os << mat.at_mut(x, y) << " ";
    }
    os << mat.at(W - 1, y) << std::endl;
  }

  return os;
}

template<typename T, size_t W, size_t H>
class matN {
  // TODO: Convert this from a 2d array to a 1d array for locality benefits (if
  // there are any given std::array doesn't)
  std::array<std::array<T, H>, W> values{};

public:

  matN(): values() {}

  [[nodiscard]] static auto Filled(const T& value) -> matN {
    matN output{};

    for (size_t x = 0; x < W; x++) {
      for (size_t y = 0; y < H; y++) {
        output.at_mut(x, y) = value;
      }
    }

    return output;
  }

  [[nodiscard]] static auto Diagonal(const T& value) -> matN {
    static_assert(W == H, "Only able to generate diagonal for square matrices");

    matN output{};

    for (size_t i = 0; i < W; i++) {
      output.at_mut(i, i) = value;
    }

    return output;
  }

  explicit matN(const std::array<std::array<T, H>, W>& values):
      values(values) {}

  matN(const matN& other) = default;
  matN& operator=(const matN& other) = default;
  matN(matN&&) = default;
  matN& operator=(matN&&) = default;
  ~matN() = default;

  [[nodiscard]] auto at(size_t x, size_t y) const -> const T& {
    return values[x][y];
  }

  [[nodiscard]] auto at_mut(size_t x, size_t y) -> T& { return values[x][y]; }

  template<typename Y>
  [[nodiscard]] auto operator+(const matN<Y, W, H>& other) const
    -> matN<MULT_RETURN<T, Y>, W, H> {

    using R = MULT_RETURN<T, Y>;
    matN<R, W, H> output{};

    for (size_t x = 0; x < W; x++) {
      for (size_t y = 0; y < H; y++) {
        output.at_mut(x, y) = at(x, y) + other.at(x, y);
      }
    }

    return output;
  }

  [[nodiscard]] auto operator-() const -> matN {
    matN<T, W, H> output{};

    for (size_t x = 0; x < W; x++) {
      for (size_t y = 0; y < H; y++) {
        output.at_mut(x, y) = -at(x, y);
      }
    }

    return output;
  }

  template<typename Y>
  [[nodiscard]] auto operator-(const matN<Y, W, H>& other) const
    -> matN<MULT_RETURN<T, Y>, W, H> {

    using R = MULT_RETURN<T, Y>;
    matN<R, W, H> output{};

    for (size_t x = 0; x < W; x++) {
      for (size_t y = 0; y < H; y++) {
        output.at_mut(x, y) = at(x, y) - other.at(x, y);
      }
    }

    return output;
  }

  template<typename Y, size_t oW>
  [[nodiscard]] auto operator*(const matN<Y, oW, W>& other) const
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

  [[nodiscard]] auto operator*(float scale) const -> matN {
    matN output{};

    for (size_t x = 0; x < W; x++) {
      for (size_t y = 0; y < H; y++) {
        output.at_mut(x, y) = at(x, y) * scale;
      }
    }

    return output;
  }

  [[nodiscard]] auto transpose() const -> matN<T, H, W> {
    matN<T, H, W> output{};

    for (size_t x = 0; x < W; x++) {
      for (size_t y = 0; y < H; y++) {
        output.at_mut(y, x) = at(x, y);
      }
    }

    return output;
  }

  [[nodiscard]] auto is_square() const -> bool { return W == H; }
};

// TODO: Make this nice to use
template<typename T, size_t N>
using vecN = matN<T, 1, N>;

#endif
