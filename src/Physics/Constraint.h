#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "matN.h"

class Constraint {
public:

  Body* a;
  Body* b;

  [[nodiscard]] matN<int, 1, 1> GetInvM() const;

  void Solve();
};

#endif
