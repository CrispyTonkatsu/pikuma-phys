#include "Contact.h"

Contact::Contact(
  Body& a,
  Body& b,
  Vec2 start,
  Vec2 end,
  Vec2 normal,
  float depth
):
    a(&a), b(&b), start(start), end(end), normal(normal), depth(depth) {}

// TODO: Implement the functions for the contacts

void Contact::ResolvePenetration() const {
  if (a->isStatic() && b->isStatic()) {
    return;
  }

  const auto equation = [&](float inv_mass) {
    return depth / (a->inv_mass + b->inv_mass) * inv_mass;
  };

  a->position -= normal * equation(a->inv_mass);
  b->position += normal * equation(b->inv_mass);
}
