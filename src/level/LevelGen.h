#ifndef ARGOS_LEVELGEN_H
#define ARGOS_LEVELGEN_H


#include "Level.h"

class LevelGen {

  Level* level;

public:
  LevelGen() {
  }

  Level* generate(GameData& data);

};


#endif //ARGOS_LEVELGEN_H
