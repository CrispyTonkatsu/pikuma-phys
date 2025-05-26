#include "Body.h"

Body::Body(std::unique_ptr<Shape> shape, Vec2 position, float mass):
    position(position),
    shape(std::move(shape)),
    mass(mass),
    inv_mass((mass != 0.f) ? (1.f / mass) : 0.f),
    inertia(this->shape->GetMomentOfInertia(mass)),
    inv_inertia((inertia != 0.f) ? (1.f / inertia) : 0.f) {}

void Body::Integrate(float dt) {
  acceleration = net_force * inv_mass;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();

  angular_acceleration = net_torque * inv_inertia;
  angular_velocity += angular_acceleration * dt;
  rotation += angular_velocity * dt;

  ClearTorques();
}

void Body::AddForce(Vec2 force) { net_force += force; }

void Body::AddTorque(float torque) { net_torque += torque; }

void Body::ClearForces() { net_force = Vec2(0.f, 0.f); }

void Body::ClearTorques() { net_torque = 0.f; }
