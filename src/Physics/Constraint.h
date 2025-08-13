#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "Vec2.h"
#include "matN.h"

class Constraint {
public:

  Body* a{nullptr};
  Body* b{nullptr};

  Constraint(Body* a, Body* b);

  virtual ~Constraint() = default;
  Constraint(const Constraint&) = default;
  Constraint(Constraint&&) = delete;
  Constraint& operator=(const Constraint&) = default;
  Constraint& operator=(Constraint&&) = delete;

  [[nodiscard]] matN<float, 6, 6> get_inverse_mass() const;

  [[nodiscard]] vecN<float, 6> get_velocities() const;

  virtual void Solve() = 0;
};

class JointConstraint : public Constraint {
public:

  explicit JointConstraint(Body* a, Body* b, Vec2 anchor);

  // local space for the anchor point relative to body a
  vec2 aPoint{{{{0.f, 0.f}}}};

  // local space for the anchor point relative to body b
  vec2 bPoint{{{{0.f, 0.f}}}};

  matN<float, 6, 1> Jacobian{};

  void Solve() override;
};

class DistanceConstraint : public Constraint {
  // TODO: matN<> Jacobian();
  // TODO: void Solve() override;
};

class PenetrationConstraint : public Constraint {
  // TODO: matN<> Jacobian();
  // TODO: void Solve() override;
};

#endif
