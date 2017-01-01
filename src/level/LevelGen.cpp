#include "LevelGen.h"

Level *LevelGen::generate(GameData &data) {
  level = new Level(100, 100, data);

  for (size_t x = 0; x < 100; ++x) {
    for (size_t y = 0; y < 100; ++y) {
      auto decision = rand() % 100;
      if (decision < 30)
        level->get(x, y).setData(level->World, data["cave"]);
      else if (decision < 35)
        level->get(x, y).setData(level->World, data["platform"]);

    }
  }
  level->finalize();

  return level;
}
