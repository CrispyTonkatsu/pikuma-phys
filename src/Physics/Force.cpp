#include "Force.h"
#include "Constants.h"

Vec2 force::GenerateWeight(const Body& body) {
  return (GRAVITY * PIXELS_PER_METER) * body.mass;
}

Vec2 force::GenerateFrictionSimple(const Body& body, float k) {
  return -body.velocity.UnitVector() * k;
}

Vec2 force::GenerateDragSimple(const Body& body, float k) {

  float speed_2 = body.velocity.MagnitudeSquared();

  if (speed_2 > 0.f) {
    return -body.velocity.UnitVector() * speed_2 * k;
  }

  return {0.f, 0.f};
}

Vec2 force::GenerateGravitationalToB(
  const Body& body_a,
  const Body& body_b,
  float G_mod
) {
  Vec2 between = body_b.position - body_a.position;

  return between.UnitVector()
       * ((body_a.mass * body_b.mass) / between.MagnitudeSquared())
       * GRAVITATIONAL_CONSTANT * G_mod;
}

Vec2 force::GenerateSpring(
  const Body& body,
  Vec2 anchor,
  float rest_length,
  float spring_constant
) {
  Vec2 between = body.position - anchor;
  float stretch_length = between.Magnitude() - rest_length;

  return between.UnitVector() * stretch_length * -spring_constant;
}
