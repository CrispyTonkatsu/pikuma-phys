#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

class Particle {
public:

  Vec2 position{};
  Vec2 velocity{};
  Vec2 acceleration{};

  float mass;

  Particle(Vec2 position, float mass);
  ~Particle() = default;

  Particle(const Particle&) = default;
  Particle(Particle&&) = delete;
  Particle& operator=(const Particle&) = default;
  Particle& operator=(Particle&&) = delete;
};

#endif
