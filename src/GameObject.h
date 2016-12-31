#ifndef ARGOS_GAMEOBJECT_H
#define ARGOS_GAMEOBJECT_H

#include <Constants.h>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Level;

class GameObject {
protected:
  bool ShouldBeRemoved_ = false;
  b2Body* Body = nullptr;
public:
  virtual ~GameObject() {
    if (Body)
    Body->GetWorld()->DestroyBody(Body);
  }
  virtual void update() {}
  virtual void render(sf::RenderTarget& target) {}
  bool shouldBeRemoved() const {
    return ShouldBeRemoved_;
  }

  b2Body* getBody() {
    return Body;
  }

};

#endif //ARGOS_GAMEOBJECT_H
