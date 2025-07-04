#ifndef WORLD_H
#define WORLD_H

#include <functional>
#include <memory>
#include <vector>
#include "Body.h"
#include "Contact.h"
#include "Vec2.h"

class World {
private:

  Vec2 gravity{0.f, 9.81f};

  std::vector<std::unique_ptr<Body>> bodies{};
  std::vector<Vec2> forces{};
  std::vector<float> torques{};
  std::vector<Contact> contacts{};

public:

  explicit World(Vec2 gravity);

  ~World() = default;
  World(const World&) = delete;
  World(World&&) = delete;
  World& operator=(const World&) = delete;
  World& operator=(World&&) = delete;

  Body& AddBody(std::unique_ptr<Body> body);

  [[nodiscard]] std::vector<std::unique_ptr<Body>>& GetBodies();

  [[nodiscard]] const std::vector<Contact>& GetContacts() const;

  void AddForce(Vec2 force);

  void AddTorque(float torque);

  void Update(float dt);
  void ResolveCollisions();
};

#endif
