#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <optional>
#include "Body.h"
#include "Contact.h"

namespace collision_detection {
  [[nodiscard]] std::optional<Contact> IsColliding(Body& a, Body& b);

  [[nodiscard]] std::optional<Contact> CircleCircleCollision(Body& a, Body& b);
}

#endif
