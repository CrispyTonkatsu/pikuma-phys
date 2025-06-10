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
  if (a->IsStatic() && b->IsStatic()) {
    return;
  }

  const auto equation = [&](float inv_mass) {
    return (depth / (a->inv_mass + b->inv_mass)) * inv_mass;
  };

  a->position -= normal * equation(a->inv_mass);
  b->position += normal * equation(b->inv_mass);
}

void Contact::ResolveCollision() const {
  ResolvePenetration();

  const float restitution = std::min(a->restitution, b->restitution);
  const Vec2 relative_velocity = a->velocity - b->velocity;

  const float impulse_magniude =
    (-(1.f + restitution) * (relative_velocity.Dot(normal)))
    / (a->inv_mass + b->inv_mass);

  const Vec2 impulse = normal * impulse_magniude;

  a->ApplyImpulse(impulse);
  b->ApplyImpulse(-impulse);
}
