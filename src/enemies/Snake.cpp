#include "Snake.h"

#include <level/Level.h>

Snake::Snake(Level &level, float x, float y) {
  level_ = &level;
  texture.loadFromFile("snake.png");

  b2BodyDef BodyDef;
  BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
  BodyDef.type = b2_dynamicBody;
  BodyDef.fixedRotation = true;
  Body = level.World.CreateBody(&BodyDef);

  b2CircleShape Shape;
  Shape.m_p.Set(0, 0);
  Shape.m_radius = 7 / SCALE;

  b2FixtureDef FixtureDef;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 0.0f;
  FixtureDef.shape = &Shape;
  Body->CreateFixture(&FixtureDef);
  Body->SetUserData((void*)this);
}

Tile &Snake::getTile(int dx, int dy) {
  int tx = (int) (Body->GetPosition().x + 0.5f) + dx;
  int ty = (int) (Body->GetPosition().y + 0.5f) + dy;
  return level_->get(tx, ty);
}

void Snake::render(sf::RenderTarget &target) {
  sf::Sprite sprite(texture);
  int spriteIndex = 0;
  if ((int) (level_->time * 2) % 2)
    spriteIndex = 1;
  sprite.setTextureRect({18 * spriteIndex, 0, 18, 14});
  if (!looksRight)
    sprite.scale(-1, 1);

  sprite.setOrigin(9.f, 7.f);
  sprite.setPosition(SCALE * Body->GetPosition().x,
                     SCALE * Body->GetPosition().y);
  sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
  target.draw(sprite);
}
