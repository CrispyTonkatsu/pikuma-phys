#include "Particle.h"
#include "Constants.h"

Particle::Particle(Vec2 position, float mass, float radius):
    position(position), mass(mass), radius(radius) {}

void Particle::Integrate(float dt) {
  acceleration = Vec2(0.f, 9.81f) * PIXELS_PER_METER;
  velocity += acceleration * dt;
  position += velocity * dt;
}
