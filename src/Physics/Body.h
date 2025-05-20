#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

class Body {
public:

  Vec2 position{};
  Vec2 velocity{};
  Vec2 acceleration{};
  Vec2 net_force{};

  // TODO: Angular Properties

  // TODO: std::unique_ptr<Shape> shape{nullptr};

  float mass{1.f};
  float inv_mass{1.f};
  float radius{1.f};

  Body(Vec2 position, float mass, float radius);
  ~Body() = default;

  Body(const Body&) = default;
  Body(Body&&) = delete;
  Body& operator=(const Body&) = default;
  Body& operator=(Body&&) = delete;

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
