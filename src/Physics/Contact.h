#ifndef CONTACT_H
#define CONTACT_H

#include "Body.h"
#include "Vec2.h"

struct Contact {
  Body* a{nullptr};
  Body* b{nullptr};

  Vec2 start{};
  Vec2 end{};

  Vec2 normal{};
  float depth{0.f};

  Contact() = default;
  Contact(const Contact&) = default;
  Contact(Contact&&) = delete;
  Contact& operator=(const Contact&) = default;
  Contact& operator=(Contact&&) = delete;
  ~Contact() = default;
};

#endif
