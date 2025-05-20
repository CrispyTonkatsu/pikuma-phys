#include "Body.h"

Body::Body(Vec2 position, float mass, float radius):
    position(position),
    mass(mass),
    inv_mass((mass != 0.f) ? (1.f / mass) : 0.f),
    radius(radius) {}

void Body::Integrate(float dt) {
  // Gravity can just be added to not do an extra multiplication
  acceleration = net_force * inv_mass;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();
}

void Body::AddForce(Vec2 force) { net_force += force; }

void Body::ClearForces() { net_force = Vec2(0.f, 0.f); }
