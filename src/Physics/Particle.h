#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

class Particle {
public:

  Vec2 position{};
  Vec2 velocity{};
  Vec2 acceleration{};

  float mass{1.f};
  float radius{1.f};

  Particle(Vec2 position, float mass, float radius);
  ~Particle() = default;

  Particle(const Particle&) = default;
  Particle(Particle&&) = delete;
  Particle& operator=(const Particle&) = default;
  Particle& operator=(Particle&&) = delete;

  /**
   * @brief Integrate the new position of the particle using Implicit Euler's Method
   */
  void Integrate(float dt);
};

#endif
