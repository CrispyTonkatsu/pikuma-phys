#include "Particle.h"
#include "Constants.h"

Particle::Particle(Vec2 position, float mass, float radius):
    position(position),
    mass(mass),
    inv_mass((mass != 0.f) ? (1.f / mass) : 0.f),
    radius(radius) {}

void Particle::Integrate(float dt) {
  // Gravity can just be added to not do an extra multiplication
  acceleration = (net_force * inv_mass + GRAVITY) * PIXELS_PER_METER;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();
}

void Particle::AddForce(Vec2 force) { net_force += force; }

void Particle::ClearForces() { net_force = Vec2(0.f, 0.f); }
