#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

class Particle {
public:

  Vec2 position{};
  Vec2 velocity{};
  Vec2 acceleration{};
  Vec2 net_force{};

  float mass{1.f};
  float inv_mass{1.f};
  float radius{1.f};

  Particle(Vec2 position, float mass, float radius);
  ~Particle() = default;

  Particle(const Particle&) = default;
  Particle(Particle&&) = delete;
  Particle& operator=(const Particle&) = default;
  Particle& operator=(Particle&&) = delete;

  /**
   * @brief Integrate the new position of the particle using Implicit Euler's
   * Method
   * @param dt Delta Time
   */
  void Integrate(float dt);

  /**
   * @brief Adds to the net force of a particle
   * @param force The force to add to the particle
   */
  void AddForce(Vec2 force);

  /**
   * @brief This will clear all forces (it should only be used once per frame,
   * right after integration)
   */
  void ClearForces();
};

#endif
