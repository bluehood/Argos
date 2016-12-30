#ifndef ARGOS_GAMEOBJECT_H
#define ARGOS_GAMEOBJECT_H


#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Level;

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 32.f;
static const float GRAVITY = 9.8f * 2;

class GameObject {
public:
  virtual void update() {}
  virtual void render(sf::RenderTarget& target) {}
};

#endif //ARGOS_GAMEOBJECT_H
