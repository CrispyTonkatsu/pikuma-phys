#include "Constraint.h"
#include "matN.h"
#include "Vec2.h"

Constraint::Constraint(Body* a, Body* b): a(a), b(b) {}

matN<float, 6, 6> Constraint::get_inverse_mass_matrix() const {
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
    Constraint(a, b),
    a_point(a->ToLocal(anchor)),
    b_point(b->ToLocal(anchor)) {}

matN<float, 6, 1> JointConstraint::generate_jacobian() const {
  auto cross = [](vec2 a, vec2 b) {
    return (a.at(0, 0) * b.at(0, 1)) - (a.at(0, 1) * b.at(0, 0));
  };

  vec2 aw_point = a->ToWorld(a_point);
  vec2 bw_point = b->ToWorld(b_point);

  vec2 ra = aw_point - vec2(a->position);
  vec2 rb = bw_point - vec2(b->position);

  vec2 b_to_a = aw_point - bw_point;

  vec2 j0 = 2.f * b_to_a;
  float j1 = 2.f * cross(ra, b_to_a);
  vec2 j2 = 2.f * -b_to_a;
  float j3 = 2.f * cross(rb, -b_to_a);

  return matN<float, 6, 1>{
    {
     {
        {j0.at(0, 0)},
        {j0.at(1, 0)},
        {j1},
        {j2.at(1, 0)},
        {j2.at(1, 0)},
        {j3},
      }, }
  };
}

void JointConstraint::Solve() {
  const matN<float, 6, 1> jacobian{generate_jacobian()};

  const vecN<float, 6> velocities{get_velocities()};

  const matN<float, 6, 6> mass_matrix{get_inverse_mass_matrix()};

  // NOTE: This is a 1x1 matrix but the Gauss-Seidel method will solve for
  // matrices of many sizes regardless (There are other ways to solve linear
  // equations so check them depending on the problem)

  // lhs * lambda = rhs
  matN<float, 1, 1> rhs = (-(jacobian * velocities));
  matN<float, 1, 1> lhs = (jacobian * mass_matrix * jacobian.transpose());

  // Solving for lambda
  vecN<float, 1> lambda = solver::solve_gauss_seidel(lhs, rhs);

  // TODO: Apply the lambda impulse to both bodies
}
