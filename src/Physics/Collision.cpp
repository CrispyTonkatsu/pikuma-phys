#include "Collision.h"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <optional>
#include "Body.h"
#include "Contact.h"
#include "Shape.h"
#include "Vec2.h"

std::optional<Contact> collision_detection::IsColliding(Body& a, Body& b) {
  if (a.shape->GetType() == ShapeType::CIRCLE
      && b.shape->GetType() == ShapeType::CIRCLE) {
    return CircleCircleCollision(a, b);
  }

  if (a.shape->IsPoly() && b.shape->IsPoly()) {
    return PolygonPolygonCollision(a, b);
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

  Vec2 normal = to_other.UnitVector();

  return std::make_optional<Contact>(
    a,
    b,
    b.position - (normal * bc.radius),
    a.position + (normal * ac.radius),
    normal,
    std::abs(depth)
  );
}

std::optional<Contact> collision_detection::PolygonPolygonCollision(
  Body& a,
  Body& b
) {
  PolygonShape& ap = *a.shape->as<PolygonShape>();
  PolygonShape& bp = *b.shape->as<PolygonShape>();

  std::optional<DistanceQuery> ab_check =
    collision_detection::FindSeparation(ap, bp);

  if (!ab_check.has_value() || ab_check.value().distance > 0.f) {
    return std::nullopt;
  }

  std::optional<DistanceQuery> ba_check =
    collision_detection::FindSeparation(bp, ap);

  if (!ba_check.has_value() || ba_check.value().distance > 0.f) {
    return std::nullopt;
  }

  DistanceQuery& to_use = ab_check->distance > ba_check->distance
                          ? ab_check.value()
                          : ba_check.value();

  return std::make_optional<Contact>(
    a,
    b,
    to_use.start_point,
    to_use.end_point,
    to_use.normal,
    to_use.distance
  );
}

std::optional<collision_detection::DistanceQuery> collision_detection::
  FindSeparation(PolygonShape& a, PolygonShape& b) {

  if (a.local_vertices.empty() || b.local_vertices.empty()) {
    return std::nullopt;
  }

  float max_distance = -std::numeric_limits<float>::max();
  Vec2 max_normal{};
  Vec2 max_support{};
  std::size_t vertex_i = 0;

  for (std::size_t i = 0; i < a.world_vertices.size(); i++) {
    const Vec2 start = a.world_vertices[i];
    const Vec2 end = a.world_vertices[(i + 1) % a.world_vertices.size()];

    const Vec2 line_v = end - start;
    const Vec2 normal = line_v.Normal();
    const float line_c = normal.Dot(start);

    const Vec2 support = b.support_point(-normal);
    const float side = normal.Dot(support) - line_c;

    const Vec2 to_support = support - start;
    const Vec2 projection_v =
      line_v.UnitVector() * line_v.UnitVector().Dot(to_support);

    const Vec2 projection_p = start + projection_v;
    const float distance =
      (support - projection_p).Magnitude() * (side < 0 ? -1.f : 1.f);

    if (distance > max_distance) {
      max_distance = distance;
      max_normal = normal;

      vertex_i = i;
      max_support = support;
    }
  }

  return DistanceQuery{
    max_normal.UnitVector(),
    a.world_vertices[vertex_i],
    max_support,
    max_distance
  };
}
