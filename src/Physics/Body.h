#ifndef PARTICLE_H
#define PARTICLE_H

#include <memory>
#include "SDL_render.h"
#include "Shape.h"
#include "Vec2.h"

class Body {
public:

  bool isColliding{false};

  // Linear Properties
  Vec2 position{};

  Vec2 velocity{};
  Vec2 acceleration{};
  Vec2 net_force{};

  // Angular Properties (in radians)
  float rotation{0.f};

  float angular_velocity{0.f};
  float angular_acceleration{0.f};
  float net_torque{0.f};

  std::unique_ptr<Shape> shape{nullptr};
  SDL_Texture* texture{nullptr};

  float mass{1.f};
  float inv_mass{1.f};

  float inertia{1.f};
  float inv_inertia{1.f};

  float restitution{0.f};
  float friction{0.f};

  Body(
    std::unique_ptr<Shape> shape,
    Vec2 position,
    float mass,
    float restitution = 0.f,
    float friction = 1.f
  );
  ~Body();

  Body(const Body&) = delete;
  Body(Body&&) = default;
  Body& operator=(const Body&) = delete;
  Body& operator=(Body&&) = default;

  void Update(float dt);

  /**
   * @brief Integrate the new position of the particle using Implicit Euler's
   * Method
   * @param dt Delta Time
   */
  void IntegrateLinear(float dt);

  /**
   * @brief Integrate the new position of the particle using Implicit Euler's
   * Method
   * @param dt Delta Time
   */
  void IntegrateAngluar(float dt);

  /**
   * @brief Adds to the net force of a particle
   * @param force The force to add to the particle
   */
  void AddForce(Vec2 force);

  void AddTorque(float torque);

  void ApplyImpulse(Vec2 impulse);

  void ApplyImpulseAt(Vec2 impulse, Vec2 location);

  /**
   * @brief This will clear all forces (it should only be used once per frame,
   * right after integration)
   */
  void ClearForces();

  void ClearTorques();

  [[nodiscard]] bool IsStatic() const;

  [[nodiscard]] Vec2 velocity_at(Vec2 location) const;

  void SetTexture(const std::string& filepath);
};

#endif
