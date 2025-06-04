#include "Body.h"

Body::Body(std::unique_ptr<Shape> shape, Vec2 position, float mass):
    position(position),
    shape(std::move(shape)),
    mass(mass),
    inv_mass((mass != 0.f) ? (1.f / mass) : 0.f),
    inertia(this->shape->GetMomentOfInertia(mass)),
    inv_inertia((inertia != 0.f) ? (1.f / inertia) : 0.f) {}

void Body::Update(float dt) {
  // NOTE: Consider how this could be used to lock only certain axes
  if (isStatic()) {
    return;
  }

  IntegrateLinear(dt);
  IntegrateAngluar(dt);

  if (shape->GetType() == ShapeType::BOX
      || shape->GetType() == ShapeType::POLYGON) {
    shape->as<BoxShape>()->UpdateVertices(position, rotation);
  }
}

void Body::IntegrateLinear(float dt) {
  acceleration = net_force * inv_mass;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();
}

void Body::IntegrateAngluar(float dt) {
  angular_acceleration = net_torque * inv_inertia;
  angular_velocity += angular_acceleration * dt;
  rotation += angular_velocity * dt;

  ClearTorques();
}

void Body::AddForce(Vec2 force) { net_force += force; }

void Body::AddTorque(float torque) { net_torque += torque; }

void Body::ApplyImpulse(Vec2 impulse) {
  if (isStatic()) {
    return;
  }

  velocity += impulse * inv_mass;
}

void Body::ClearForces() { net_force = Vec2(0.f, 0.f); }

void Body::ClearTorques() { net_torque = 0.f; }

bool Body::isStatic() const { return std::abs(inv_mass - 0.0f) < EPSILON; }
