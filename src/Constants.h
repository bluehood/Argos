#ifndef ARGOS_CONSTANTS_H
#define ARGOS_CONSTANTS_H

#include <cstdint>

static const uint16_t B2D_LEVEL =  1 << 0;
static const uint16_t B2D_PLAYER = 1 << 1;
static const uint16_t B2D_ENEMY =  1 << 2;
static const uint16_t B2D_ITEM =   1 << 3;

/** We need this to easily convert between pixel and real-world coordinates*/
static constexpr float SCALE = 32.f;
static constexpr float GRAVITY = 9.8f * 2;


#endif //ARGOS_CONSTANTS_H
