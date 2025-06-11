#include "Contact.h"
#include "Vec2.h"

Contact::Contact(
  Body& a,
  Body& b,
  Vec2 start,
  Vec2 end,
  Vec2 normal,
  float depth
):
    a(&a), b(&b), start(start), end(end), normal(normal), depth(depth) {}

void Contact::ResolvePenetration() const {
  if (a->IsStatic() && b->IsStatic()) {
    return;
  }

  const auto equation = [&](float inv_mass) {
    return depth / (a->inv_mass + b->inv_mass) * inv_mass;
  };

  a->position -= normal * equation(a->inv_mass);
  b->position += normal * equation(b->inv_mass);
}

void Contact::ResolveCollision() const {
  ResolvePenetration();

  const Vec2 ra = end - a->position;
  const Vec2 rb = start - b->position;

  const Vec2 va = a->velocity_at(end);
  const Vec2 vb = b->velocity_at(start);

  const Vec2 relative_velocity = va - vb;

  if (relative_velocity.Dot(normal) < 0.f) {
    return;
  }

  const float restitution = std::min(a->restitution, b->restitution);

  const float ra_x_n = ra.Cross(normal);
  const float rb_x_n = rb.Cross(normal);

  const float impulse_magniude = (-(1.f + restitution)
                                  * (relative_velocity.Dot(normal)))
                               / (a->inv_mass + b->inv_mass
                                  + (ra_x_n * ra_x_n * a->inv_inertia)
                                  + (rb_x_n * rb_x_n * b->inv_inertia));

  const Vec2 impulse = normal * impulse_magniude;

  a->ApplyImpulseAt(impulse, end);
  b->ApplyImpulseAt(-impulse, start);
}
