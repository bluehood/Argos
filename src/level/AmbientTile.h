#ifndef ARGOS_AMBIENTTILE_H
#define ARGOS_AMBIENTTILE_H


#include <SFML/Graphics.hpp>
#include <gamedata/GameData.h>
#include <Constants.h>

class AmbientTile {

  int x, y;

public:
  AmbientTile() {}
  AmbientTile(int x, int y) : x(x), y(y){}

  std::string spriteName;

  void render(GameData& data, sf::RenderTarget& t) {
    if (spriteName.empty())
      return;
    sf::Sprite sprite = data.getSprite(spriteName);
    sprite.setOrigin(SCALE / 2, SCALE / 2);
    sprite.setPosition(x * SCALE, y * SCALE);
    t.draw(sprite);
  }

};


#endif //ARGOS_AMBIENTTILE_H
