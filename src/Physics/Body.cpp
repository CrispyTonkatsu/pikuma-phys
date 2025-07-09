#include "../Graphics.h"
#include "Constants.h"
#include "SDL_image.h"
#include "SDL_render.h"
#include "Shape.h"
#include "Vec2.h"
#include "Body.h"
#include <iostream>
#include <memory>
#include <ostream>

Body::Body(
  std::unique_ptr<Shape> shape,
  Vec2 position,
  float mass,
  float restitution,
  float friction
):
    position(position),
    shape(std::move(shape)),
    mass(mass),
    inv_mass((mass != 0.f) ? (1.f / mass) : 0.f),
    inertia(this->shape->GetMomentOfInertia(mass)),
    inv_inertia((inertia != 0.f) ? (1.f / inertia) : 0.f),
    restitution(restitution),
    friction(friction) {}

Body::~Body() { SDL_DestroyTexture(texture); }

void Body::Update(float dt) {
  shape->UpdateVertices(position, rotation);

  // NOTE: Consider how this could be used to lock only certain axes
  if (IsStatic()) {
    return;
  }

  IntegrateLinear(dt);
  IntegrateAngluar(dt);
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
  if (IsStatic()) {
    return;
  }

  velocity += impulse * inv_mass;
}

void Body::ApplyImpulseAt(Vec2 impulse, Vec2 location) {
  if (IsStatic()) {
    return;
  }

  Vec2 r = location - position;

  velocity += impulse * inv_mass;
  angular_velocity += r.Cross(impulse) * inv_inertia;
}

void Body::ClearForces() { net_force = Vec2(0.f, 0.f); }

void Body::ClearTorques() { net_torque = 0.f; }

bool Body::IsStatic() const { return std::abs(inv_mass - 0.0f) < EPSILON; }

Vec2 Body::velocity_at(Vec2 location) const {
  Vec2 to_location = location - position;
  // NOTE: This is going to do the cross product of the vector in 2D for the
  // angular components
  return velocity + (Vec2(-to_location.y, to_location.x) * angular_velocity);
}

void Body::SetTexture(const std::string& filepath) {
  SDL_Surface* surface{IMG_Load(filepath.c_str())};

  if (surface != nullptr) {
    texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
  } else {
    std::cout << "Failed to load texture at " << filepath << std::endl;
  }
}
