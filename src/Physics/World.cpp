#include "World.h"
#include "Collision.h"
#include "Force.h"

World::World(Vec2 gravity): gravity(gravity) {}

Body& World::AddBody(std::unique_ptr<Body> body) {
  bodies.push_back(std::move(body));
  return *bodies.back();
}

std::vector<std::unique_ptr<Body>>& World::GetBodies() { return bodies; }

const std::vector<Contact>& World::GetContacts() const { return contacts; }

void World::AddForce(Vec2 force) { forces.push_back(force); }

void World::AddTorque(float torque) { torques.push_back(torque); }

void World::Update(float dt) {
  contacts.clear();

  // Consider abstracting this into a function so that there can be global
  // forces
  for (auto& body: bodies) {
    body->AddForce(force::GenerateWeight(*body));
  }

  for (auto& body: bodies) {
    body->IntegrateForces(dt);
  }

  // TODO: Solve constraints
  for (auto& constraint: constraints) {
    constraint->Solve();
  }

  for (auto& body: bodies) {
    body->IntegrateVelocities(dt);
  }

  ResolveCollisions();
}

void World::ResolveCollisions() {
  for (size_t i = 0; i < bodies.size() - 1; i++) {
    for (size_t j = i + 1; j < bodies.size(); j++) {

      auto contact_opt =
        collision_detection::IsColliding(*bodies[i], *bodies[j]);

      if (contact_opt.has_value()) {
        bodies[i]->isColliding = true;
        bodies[j]->isColliding = true;
        contacts.push_back(contact_opt.value());
      }
    }
  }

  for (auto& contact: contacts) {
    contact.ResolveCollision();
  }
}
