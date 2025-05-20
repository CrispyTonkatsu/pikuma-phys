#ifndef FORCE_H
#define FORCE_H

#include "Particle.h"
#include "Vec2.h"

namespace force {
  Vec2 GenerateWeight(const Particle& particle);

  Vec2 GenerateFrictionSimple(const Particle& particle, float k);

  Vec2 GenerateDragSimple(const Particle& particle, float k);

  Vec2 GenerateGravitationalToB(
    const Particle& particle_a,
    const Particle& particle_b,
    float G_mod = 1.f
  );

  Vec2 GenerateSpring(
    const Particle& particle,
    Vec2 anchor,
    float rest_length,
    float spring_constant
  );
}

#endif
