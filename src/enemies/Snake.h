#ifndef ARGOS_SNAKE_H
#define ARGOS_SNAKE_H

#include <GameObject.h>
#include <level/Tile.h>

class Snake : public GameObject {

  bool looksRight = true;
  sf::Texture texture;
  Level* level_;

public:
  Snake(Level& level, float x, float y);

  enum class MoveDirection {
    LEFT,
    RIGHT,
    NONE
  };
  MoveDirection MoveDir = MoveDirection::RIGHT;

  Tile& getTile(int dx = 0, int dy = 0);

  virtual void update() override {
    if (std::abs(Body->GetLinearVelocity().x) < 0.1) {
      if (MoveDir == MoveDirection::LEFT)
        MoveDir = MoveDirection::RIGHT;
      else
        MoveDir = MoveDirection::LEFT;
    }
    switch(MoveDir) {
      case MoveDirection::LEFT:
        Body->SetLinearVelocity({-1, Body->GetLinearVelocity().y});
        looksRight = false;
        break;
      case MoveDirection::RIGHT:
        Body->SetLinearVelocity({1, Body->GetLinearVelocity().y});
        looksRight = true;
        break;
      case MoveDirection::NONE:
        Body->SetLinearVelocity({0, Body->GetLinearVelocity().y});
        break;
    }
  }

  virtual void render(sf::RenderTarget& target) override;

};


#endif //ARGOS_SNAKE_H
