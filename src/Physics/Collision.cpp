#include "Collision.h"
#include <optional>
#include "Body.h"
#include "Contact.h"
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

  float depth = to_other.Magnitude() - radius_sum;

  if (depth > 0.f) {
    return std::nullopt;
  }

  Vec2 normal = -to_other.UnitVector();

  return std::make_optional<Contact>(
    a,
    b,
    a.position + (-normal * ac.radius),
    b.position + (normal * bc.radius),
    normal,
    depth
  );
}
