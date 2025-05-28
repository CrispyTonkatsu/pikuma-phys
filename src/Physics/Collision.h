#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Body.h"

namespace collision_detection {
  [[nodiscard]] bool IsColliding(Body& a, Body& b);

  [[nodiscard]] bool CircleCircleCollision(Body& a, Body& b);
}

#endif
