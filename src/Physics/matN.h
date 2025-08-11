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
  std::array<std::array<T, H>, W> values{};

  explicit matN(const std::array<std::array<T, H>, W>& values):
      values(values) {}

public:

  matN(): values() {}

  matN(const matN& other) = default;
  matN& operator=(const matN& other) = default;
  matN(matN&&) = default;
  matN& operator=(matN&&) = default;
  ~matN() = default;

  [[nodiscard]] static auto WithData(
    const std::array<std::array<T, H>, W>& values
  ) -> matN {
    return matN{values};
  }

  [[nodiscard]] static auto FromArray(const std::array<T, W * H>& values)
    -> matN {
    matN output;

    for (size_t x = 0; x < W; x++) {
      for (size_t y = 0; y < H; y++) {
        output.at_mut(x, y) = values[x + (y * W)];
      }
    }

    return output;
  }

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

  [[nodiscard]] consteval auto is_square() const -> bool { return W == H; }
};

template<typename T, size_t N>
using vecN = matN<T, 1, N>;

using vec2 = vecN<float, 2>;
using vec3 = vecN<float, 3>;
using vec4 = vecN<float, 4>;

using mat2 = matN<float, 2, 2>;
using mat3 = matN<float, 3, 3>;
using mat4 = matN<float, 4, 4>;

#endif
