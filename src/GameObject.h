#ifndef ARGOS_GAMEOBJECT_H
#define ARGOS_GAMEOBJECT_H

#include <Constants.h>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Level;

class GameObject {
public:
  virtual void update() {}
  virtual void render(sf::RenderTarget& target) {}
};

#endif //ARGOS_GAMEOBJECT_H
