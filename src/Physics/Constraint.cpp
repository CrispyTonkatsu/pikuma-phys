#include "Constraint.h"
#include "matN.h"

Constraint::Constraint(Body* a, Body* b): a(a), b(b) {}

matN<float, 6, 6> Constraint::get_inverse_mass() const {
  matN<float, 6, 6> output{matN<float, 6, 6>::Filled(0.f)};

  output.at_mut(0, 0) = a->inv_mass;
  output.at_mut(1, 1) = a->inv_mass;
  output.at_mut(2, 2) = a->inv_inertia;

  output.at_mut(3, 3) = b->inv_mass;
  output.at_mut(4, 4) = b->inv_mass;
  output.at_mut(5, 5) = b->inv_inertia;

  return output;
}

vecN<float, 6> Constraint::get_velocities() const {
  vecN<float, 6> output;

  output.at_mut(0, 0) = a->velocity.x;
  output.at_mut(1, 1) = a->velocity.y;
  output.at_mut(2, 2) = a->angular_velocity;

  output.at_mut(3, 3) = b->velocity.x;
  output.at_mut(4, 4) = b->velocity.y;
  output.at_mut(5, 5) = b->angular_velocity;

  return output;
}

JointConstraint::JointConstraint(Body* a, Body* b, Vec2 anchor):
    Constraint(a, b), aPoint(a->ToLocal(anchor)), bPoint(b->ToLocal(anchor)) {}

void JointConstraint::Solve() {
  // TODO: Load the Jacobian

  // TODO: Get the velocities

  // TODO: Get the inverse mass matrix

  // TODO: Compute the lambda value

  // TODO: Apply the lambda impulse to both bodies
}
