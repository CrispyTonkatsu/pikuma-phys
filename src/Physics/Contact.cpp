#include "Contact.h"
#include <algorithm>
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

  const float restitution = std::min(a->restitution, b->restitution);

  const float ra_x_n = ra.Cross(normal);
  const float rb_x_n = rb.Cross(normal);

  const float impulse_magniude = (-(1.f + restitution)
                                  * (relative_velocity.Dot(normal)))
                               / (a->inv_mass + b->inv_mass
                                  + (ra_x_n * ra_x_n * a->inv_inertia)
                                  + (rb_x_n * rb_x_n * b->inv_inertia));

  const Vec2 impulse = normal * impulse_magniude;

  // Tangential impulse

  const Vec2 tangent = normal.Normal();

  const float friction = std::min(a->friction, b->friction);

  const float ra_x_t = ra.Cross(tangent);
  const float rb_x_t = rb.Cross(tangent);

  const float tangent_impulse_magniude = (friction * -(1.f + restitution)
                                          * (relative_velocity.Dot(tangent)))
                                       / (a->inv_mass + b->inv_mass
                                          + (ra_x_t * ra_x_t * a->inv_inertia)
                                          + (rb_x_t * rb_x_t * b->inv_inertia));

  const Vec2 tangent_impulse = tangent * tangent_impulse_magniude;

  const Vec2 net_impulse = impulse + tangent_impulse;

  // NOTE: Try to make this not necessary (watch it be like 1 thing in the engine)
  if (net_impulse.Dot(normal) > 0.f) {
    return;
  }

  a->ApplyImpulseAt(net_impulse, end);
  b->ApplyImpulseAt(-net_impulse, start);
}
