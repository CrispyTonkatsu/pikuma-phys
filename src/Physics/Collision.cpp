#include "Collision.h"
#include <optional>
#include "Body.h"
#include "Shape.h"

std::optional<Contact> collision_detection::IsColliding(Body& a, Body& b) {
  if (a.shape->GetType() == ShapeType::CIRCLE
      && b.shape->GetType() == ShapeType::CIRCLE) {
    return CircleCircleCollision(a, b);
  }

  return std::nullopt;
}

std::optional<Contact> collision_detection::CircleCircleCollision(
  Body& a,
  Body& b
) {
  Vec2 to_other = b.position - a.position;

  CircleShape& ac = *a.shape->as<CircleShape>();
  CircleShape& bc = *b.shape->as<CircleShape>();
  float radius_sum = ac.radius + bc.radius;

  return to_other.MagnitudeSquared() <= (radius_sum * radius_sum);
}
