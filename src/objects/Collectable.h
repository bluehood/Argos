#ifndef ARGOS_COLLECTABLE_H
#define ARGOS_COLLECTABLE_H

#include <players/Player.h>
#include "../GameObject.h"
#include "../level/Level.h"

class Collectable : public GameObject {

  b2Body *Body;

  sf::Sprite sprite_;

public:
  Collectable(Level &level, float x, float y);

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
