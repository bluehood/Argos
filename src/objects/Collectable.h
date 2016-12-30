#ifndef ARGOS_COLLECTABLE_H
#define ARGOS_COLLECTABLE_H

#include <players/Player.h>
#include "../GameObject.h"
#include "../level/Level.h"

class Collectable : public GameObject {

  sf::Sprite sprite_;

public:
  enum Type {
    Health,
    Armor,
    Mana,
    Gem
  };
private:
  Type type_;
public:

  Collectable(Level &level, float x, float y, Type type);

  virtual void update() override {}

  virtual void render(sf::RenderTarget &target) override {
    sprite_.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite_.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite_);
  }

  void apply(Character& c);
};

#endif //ARGOS_COLLECTABLE_H
