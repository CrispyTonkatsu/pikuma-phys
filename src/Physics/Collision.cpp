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

  if (b.shape->GetType() == ShapeType::CIRCLE && a.shape->IsPoly()) {
    return PolygonCircleCollision(a, b);
  }

  if (a.shape->GetType() == ShapeType::CIRCLE && b.shape->IsPoly()) {
    return PolygonCircleCollision(b, a);
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

  if (!ab_check.has_value() || ab_check->distance >= 0.f) {
    return std::nullopt;
  }

  std::optional<DistanceQuery> ba_check =
    collision_detection::FindSeparation(bp, ap);

  if (!ba_check.has_value() || ba_check->distance >= 0.f) {
    return std::nullopt;
  }

  if (ab_check->distance > ba_check->distance) {
    return std::make_optional<Contact>(
      a,
      b,
      ab_check->start_point,
      ab_check->end_point,
      ab_check->normal,
      -ab_check->distance
    );

  } else {
    return std::make_optional<Contact>(
      b,
      a,
      ba_check->start_point,
      ba_check->end_point,
      ba_check->normal,
      -ba_check->distance
    );
  }
}

std::optional<Contact> collision_detection::PolygonCircleCollision(
  Body& a,
  Body& b
) {
  PolygonShape& ap = *a.shape->as<PolygonShape>();
  CircleShape& bp = *b.shape->as<CircleShape>();

  const Vec2 to_circle = b.position - a.position;

  const auto [start, end] = ap.support_edge(to_circle);
  const Vec2 line_v = end - start;

  const Vec2 projected_v =
    (line_v * (line_v.Dot(b.position - start) / line_v.MagnitudeSquared()));

  Vec2 projected_p = start + projected_v;

  if ((b.position - start).Dot(line_v) < 0.f) {
    projected_p = start;
  }

  if ((b.position - end).Dot(line_v) > 0.f) {
    projected_p = end;
  }

  const Vec2 to_projected = projected_p - b.position;
  const float depth = to_projected.Magnitude() - bp.radius;

  if (depth >= 0.f) {
    return std::nullopt;
  }

  const Vec2 normal = to_projected.UnitVector();

  return std::make_optional<Contact>(
    a,
    b,
    projected_p,
    b.position - (normal * depth),
    normal,
    depth
  );
}

std::optional<collision_detection::DistanceQuery> collision_detection::
  FindSeparation(PolygonShape& a, PolygonShape& b) {

  if (a.local_vertices.empty() || b.local_vertices.empty()) {
    return std::nullopt;
  }

  float max_distance = std::numeric_limits<float>::lowest();
  Vec2 max_normal{};
  Vec2 max_support{};
  Vec2 max_support_projected{};

  for (std::size_t i = 0; i < a.world_vertices.size(); i++) {
    const auto [start, end] = a.get_edge(i);

    const Vec2 line_v = end - start;
    const Vec2 normal = line_v.Normal();

    const Vec2 support = b.support_point(-normal);

    const Vec2 to_support = support - start;
    const Vec2 projection_v =
      line_v * (line_v.Dot(to_support) / line_v.MagnitudeSquared());

    const Vec2 projection_p = start + projection_v;

    const bool side = (support - projection_p).Dot(normal) >= 0.f;

    const float distance =
      (support - projection_p).Magnitude() * (side ? 1.f : -1.f);

    if (distance > max_distance) {
      max_distance = distance;
      max_normal = normal;

      max_support = support;
      max_support_projected = projection_p;
    }
  }

  return DistanceQuery{
    max_normal,
    max_support_projected,
    max_support,
    max_distance
  };
}
