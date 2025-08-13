#ifndef MATN_H
#define MATN_H

#include <array>
#include <cstddef>
#include <iostream>
#include <utility>

template<typename T, typename Y>
using ADD_RETURN = decltype(std::declval<T>() + std::declval<Y>());

template<typename T, typename Y>
using SUB_RETURN = decltype(std::declval<T>() - std::declval<Y>());

template<typename T, typename Y>
using MULT_RETURN = decltype(std::declval<T>() * std::declval<Y>());

template<typename T, typename Y>
using DIV_RETURN = decltype(std::declval<T>() / std::declval<Y>());

template<typename T>
using DOT_ELEM =
  decltype(std::declval<MULT_RETURN<T, T>>() + std::declval<MULT_RETURN<T, T>>());

template<typename T>
using CROSS_ELEM =
  decltype(std::declval<MULT_RETURN<T, T>>() - std::declval<MULT_RETURN<T, T>>());

template<typename T, size_t W, size_t H>
class matN;

template<typename T, size_t N>
using vecN = matN<T, 1, N>;

using vec2 = vecN<float, 2>;
using vec3 = vecN<float, 3>;
using vec4 = vecN<float, 4>;

using mat2 = matN<float, 2, 2>;
using mat3 = matN<float, 3, 3>;
using mat4 = matN<float, 4, 4>;

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
public:

  std::array<std::array<T, H>, W> values{};

  explicit matN(const std::array<std::array<T, H>, W>& values):
      values(values) {}

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

  // TODO: add functions that will do compile time filling so no time is
  // lost during the solving later (this is for the improved library)

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

  // TODO: Implement these matrices for easier transformations (consider making
  // these compile time generators)
  [[nodiscard]] static auto Scale(const T&) -> matN {}

  [[nodiscard]] static auto Rotate(const T&) -> matN {}

  [[nodiscard]] static auto Translate(const T&) -> matN {}

  [[nodiscard]] auto at(size_t x, size_t y) const -> const T& {
    return values[x][y];
  }

  [[nodiscard]] auto at_mut(size_t x, size_t y) -> T& { return values[x][y]; }

  template<typename Y>
  [[nodiscard]] auto operator+(const matN<Y, W, H>& other) const
    -> matN<ADD_RETURN<T, Y>, W, H> {

    using R = ADD_RETURN<T, Y>;
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
    -> matN<SUB_RETURN<T, Y>, W, H> {

    using R = SUB_RETURN<T, Y>;
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
          output.at_mut(x, y) = output.at(x, y) + (at(i, y) * other.at(x, i));
        }
      }
    }

    return output;
  }

  template<typename Y>
  [[nodiscard]] auto operator*(Y scale) const
    -> matN<decltype(std::declval<T>() * std::declval<Y>()), W, H> {

    using R = decltype(std::declval<T>() * std::declval<Y>());
    matN<R, W, H> output{};

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

  [[nodiscard]] auto dot(matN other) const -> DOT_ELEM<T> {
    static_assert(W == 1, "Dot product only works on vectors");

    DOT_ELEM<T> output{};

    for (size_t y = 0; y < H; y++) {
      output = output + at(0, y) * other.at(0, y);
    }

    return output;
  }

  [[nodiscard]] auto cross(matN other) const -> vecN<CROSS_ELEM<T>, H> {
    static_assert(W == 1, "Cross product only works on vectors");

    vecN<CROSS_ELEM<T>, H> output{};

    auto height_wrap = [](size_t index) { return index % H; };

    for (size_t y = 0; y < H; y++) {
      output.at_mut(0, y) =
        (at(0, height_wrap(y + 1)) * other.at(0, height_wrap(y + 2)))
        - (at(0, height_wrap(y + 2)) * other.at(0, height_wrap(y + 1)));
    }

    return output;
  }

  [[nodiscard]] consteval auto is_square() const -> bool { return W == H; }
};

#endif
