#ifndef ARGOS_PLAYER_H
#define ARGOS_PLAYER_H


#include <GameObject.h>
#include <level/Tile.h>

class Character : public GameObject {

  bool looksRight = true;
  b2Body* Body;
  sf::Texture texture;
  Level* level_;

  bool hangsOnCliff = false;
public:
  Character(Level& level, float x, float y);

  enum class MoveDirection {
    LEFT,
    RIGHT,
    NONE
  };
  MoveDirection MoveDir = MoveDirection::NONE;

  Tile& getTile(int dx = 0, int dy = 0);

  virtual void update() override {
    hangsOnCliff = false;
    switch(MoveDir) {
      case MoveDirection::LEFT:
        Body->SetLinearVelocity({-3, Body->GetLinearVelocity().y});
        looksRight = false;
        if (getTile(-1).hasCliff() && hasContactInDirection(3.14, b2_pi)) {
          if ((int) (Body->GetPosition().y * 2) % 2 == 0) {
            if (Body->GetLinearVelocity().y >= 0) {
              Body->SetLinearVelocity({Body->GetLinearVelocity().x, 0});
              Body->ApplyForceToCenter({0, -GRAVITY}, true);
            }
          }
          hangsOnCliff = true;
        }
        break;
      case MoveDirection::RIGHT:
        Body->SetLinearVelocity({3, Body->GetLinearVelocity().y});
        looksRight = true;
        if (getTile(1).hasCliff() && hasContactInDirection(0, b2_pi)) {
          if ((int) (Body->GetPosition().y * 2) % 2 == 0) {
            if (Body->GetLinearVelocity().y >= 0) {
              Body->SetLinearVelocity({Body->GetLinearVelocity().x, 0});
              Body->ApplyForceToCenter({0, -GRAVITY}, true);
            }
          }
          hangsOnCliff = true;
        }
        break;
      case MoveDirection::NONE:
        Body->SetLinearVelocity({0, Body->GetLinearVelocity().y});
        break;
    }
  }

  virtual void render(sf::RenderTarget& target) override {
    sf::Sprite sprite(texture);
    if (looksRight)
      sprite.scale(-1, 1);

    sprite.setOrigin(11.f, 14.f);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite);
  }

  bool hasContactInDirection(double direction, double precision) {
    for (b2ContactEdge* edge = Body->GetContactList(); edge; edge = edge->next) {
      auto contact = edge->contact;
      if (!contact->IsTouching())
        continue;

      auto A = (GameObject*) contact->GetFixtureA()->GetBody()->GetUserData();
      auto B = (GameObject*) contact->GetFixtureB()->GetBody()->GetUserData();
      b2WorldManifold worldManifold;
      contact->GetWorldManifold(&worldManifold);
      double d;
      if (auto Char = dynamic_cast<Character *>(A)) {
        d = std::atan2(worldManifold.normal.y, worldManifold.normal.x);
      } else {
        d = std::atan2(worldManifold.normal.y, worldManifold.normal.x);
      }
      if (std::abs(d - direction) < precision)
        return true;
      if (std::abs(d - direction) - b2_pi < precision)
        return true;
    }
    return false;
  }

  void jump() {

    bool hasFloorContact = false;
    if (!hangsOnCliff) {
      if (Body->GetLinearVelocity().y < 0.02) {
        hasFloorContact = hasContactInDirection(-b2_pi/2, b2_pi * 0.9f);
      }
    }

    if (hangsOnCliff || hasFloorContact) {
      Body->SetLinearVelocity({Body->GetLinearVelocity().x, -7});
    }
  }
};



#endif //ARGOS_PLAYER_H
