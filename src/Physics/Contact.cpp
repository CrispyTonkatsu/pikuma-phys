#include "Contact.h"

Contact::Contact(
  Body& a,
  Body& b,
  Vec2 start,
  Vec2 end,
  Vec2 normal,
  float depth
):
    a(&a), b(&b), start(start), end(end), normal(normal), depth(depth) {}

// TODO: Implement the functions for the contacts
