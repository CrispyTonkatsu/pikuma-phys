#ifndef FORCE_H
#define FORCE_H

#include "Body.h"
#include "Constants.h"
#include "Vec2.h"

namespace force {
  Vec2 GenerateWeight(const Body& body, Vec2 gravity = GRAVITY);

  Vec2 GenerateFrictionSimple(const Body& body, float k);

  Vec2 GenerateDragSimple(const Body& body, float k);

  Vec2 GenerateGravitationalToB(
    const Body& body_a,
    const Body& body_b,
    float G_mod = 1.f
  );

  Vec2 GenerateSpring(
    const Body& body,
    Vec2 anchor,
    float rest_length,
    float spring_constant
  );
}

#endif
