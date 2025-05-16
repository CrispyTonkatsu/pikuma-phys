// This file includes all the constants that the engine will rely on
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Vec2.h"

// FPS constants, these are used to implement a delta time process
const int FPS{60};
const int MILISECONDS_PER_FRAME{1000 / FPS};

// The maximum allowed delta time
const float MAX_DELTA_TIME{0.016};

// Standardizing units
const int PIXELS_PER_METER{50};

// Standard accelerations
const Vec2 GRAVITY{0.f, 9.81f};

#endif
