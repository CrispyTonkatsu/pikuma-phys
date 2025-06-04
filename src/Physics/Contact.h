#ifndef CONTACT_H
#define CONTACT_H

#include "Body.h"
#include "Vec2.h"

struct Contact {
  Body* a;
  Body* b;

  Vec2 start{};
  Vec2 end{};

  Vec2 normal{};
  float depth{0.f};

  Contact(Body& a, Body& b, Vec2 start, Vec2 end, Vec2 normal, float depth);
  Contact(const Contact&) = default;
  Contact(Contact&&) = delete;
  Contact& operator=(const Contact&) = delete;
  Contact& operator=(Contact&&) = delete;
  ~Contact() = default;

  void ResolvePenetration() const;
};

#endif
