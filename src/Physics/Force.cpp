#include "Force.h"
#include "Constants.h"

Vec2 force::GenerateWeight(const Particle& particle) {
  return (GRAVITY * PIXELS_PER_METER) * particle.mass;
}

Vec2 force::GenerateFrictionSimple(const Particle& particle, float k) {
  return -particle.velocity.UnitVector() * k;
}

Vec2 force::GenerateDragSimple(const Particle& particle, float k) {

  float speed_2 = particle.velocity.MagnitudeSquared();

  if (speed_2 > 0.f) {
    return -particle.velocity.UnitVector() * speed_2 * k;
  }

  return {0.f, 0.f};
}

Vec2 force::GenerateGravitationalToB(
  const Particle& particle_a,
  const Particle& particle_b,
  float G_mod
) {
  Vec2 between = particle_b.position - particle_a.position;

  return between.UnitVector()
       * ((particle_a.mass * particle_b.mass) / between.MagnitudeSquared())
       * GRAVITATIONAL_CONSTANT * G_mod;
}

Vec2 force::GenerateSpring(
  const Particle& particle,
  Vec2 anchor,
  float rest_length,
  float spring_constant
) {
  Vec2 between = particle.position - anchor;
  float stretch_length = between.Magnitude() - rest_length;

  return between.UnitVector() * stretch_length * -spring_constant;
}
