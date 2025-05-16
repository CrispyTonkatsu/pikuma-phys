#include "Particle.h"
#include "Constants.h"

Particle::Particle(Vec2 position, float mass, float radius):
    position(position), mass(mass), radius(radius) {}

void Particle::Integrate(float dt) {
  acceleration = net_force / mass;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();
}

void Particle::AddForce(Vec2 force) { net_force += force * PIXELS_PER_METER; }

void Particle::ClearForces() { net_force = Vec2(0.f, 0.f); }
