#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <optional>
#include "Body.h"
#include "Contact.h"
#include "Shape.h"
#include "Vec2.h"

namespace collision_detection {
  [[nodiscard]] std::optional<Contact> IsColliding(Body& a, Body& b);

  [[nodiscard]] std::optional<Contact> CircleCircleCollision(Body& a, Body& b);

  [[nodiscard]] std::optional<Contact> PolygonPolygonCollision(
    Body& a,
    Body& b
  );

  [[nodiscard]] std::optional<Contact> PolygonCircleCollision(
    Body& a,
    Body& b
  );

  struct DistanceQuery {
    Vec2 normal;
    Vec2 start_point;
    Vec2 end_point;
    float distance;
  };

  [[nodiscard]] std::optional<DistanceQuery> FindSeparation(
    PolygonShape& a,
    PolygonShape& b
  );
}

#endif
