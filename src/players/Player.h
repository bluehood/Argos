#ifndef ARGOS_PLAYER_H
#define ARGOS_PLAYER_H


#include <GameObject.h>
#include <level/Tile.h>
#include <iostream>

class Character : public GameObject {

  bool looksRight = true;
  Level* level_;

  double startWalkingTime = 0;
  bool hangsOnCliff = false;

  static constexpr float radius = 14.0f / SCALE;

  bool fallThrough_ = false;
public:
  Character(Level& level, float x, float y);

  enum class MoveDirection {
    LEFT,
    RIGHT,
    NONE
  };
  MoveDirection MoveDir = MoveDirection::NONE;


  void setFallThrough(bool v) {
    if (v != fallThrough_) {
      Body->SetAwake(true);
    }
    fallThrough_ = v;
  }

  Tile& getTile(int dx = 0, int dy = 0);

  void checkPlatsforms() {
    for (b2ContactEdge* edge = Body->GetContactList(); edge; edge = edge->next) {
      auto contact = edge->contact;

      auto A = (GameObject*) contact->GetFixtureA()->GetBody()->GetUserData();
      auto B = (GameObject*) contact->GetFixtureB()->GetBody()->GetUserData();
      b2WorldManifold worldManifold;
      contact->GetWorldManifold(&worldManifold);
      double d;
      Tile* platform = nullptr;
      if (auto T = dynamic_cast<Tile *>(A)) {
        if (!T->platform())
          continue;
        platform = T;
      } else if (auto T = dynamic_cast<Tile *>(A)) {
        if (!T->platform())
          continue;
        platform = T;
      }
      if (!platform)
        continue;


      if (fallThrough_)
        contact->SetEnabled(false);
      else
        contact->SetEnabled(Body->GetPosition().y + radius <= platform->getBody()->GetPosition().y);
    }
  }

  void pushCliff() {
    int yTilePart = ((int) (Body->GetPosition().y * 4)) % 4;

    if (yTilePart == 0) {
      if (Body->GetLinearVelocity().y >= 0) {
        Body->SetLinearVelocity({Body->GetLinearVelocity().x, 0});
        Body->ApplyForceToCenter({0, -GRAVITY}, true);
      }
    }
    hangsOnCliff = true;
  }

  virtual void update() override {
    checkPlatsforms();
    hangsOnCliff = false;
    switch(MoveDir) {
      case MoveDirection::LEFT:
        Body->SetLinearVelocity({-3, Body->GetLinearVelocity().y});
        looksRight = false;
        if (getTile(-1).hasCliff()  && hasTileContact(-1, 0)) {
          pushCliff();
        }
        break;
      case MoveDirection::RIGHT:
        Body->SetLinearVelocity({3, Body->GetLinearVelocity().y});
        looksRight = true;
        if (getTile(1).hasCliff() && hasTileContact(1, 0)) {
          pushCliff();
        }
        break;
      case MoveDirection::NONE:
        Body->SetLinearVelocity({0, Body->GetLinearVelocity().y});
        break;
    }
  }

  virtual void render(sf::RenderTarget& target) override;

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

  bool hasTileContact(int x, int y) {
    for (b2ContactEdge* edge = Body->GetContactList(); edge; edge = edge->next) {
      auto contact = edge->contact;
      if (!contact->IsTouching())
        continue;

      auto A = (GameObject*) contact->GetFixtureA()->GetBody()->GetUserData();
      auto B = (GameObject*) contact->GetFixtureB()->GetBody()->GetUserData();

      Tile *T;
      if ((T = dynamic_cast<Tile *>(A))) {
      } else if ((T = dynamic_cast<Tile *>(B))) {
      } else {
        continue;
      }
      if (&getTile(x, y) == T)  {
        return true;
      }
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

  float getYSpeed();

  void shoot();
};



#endif //ARGOS_PLAYER_H
