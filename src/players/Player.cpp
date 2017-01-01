#include "Player.h"
#include <level/Level.h>

Character::Character(Level &level, float x, float y) {
  level_ = &level;

  b2BodyDef BodyDef;
  BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
  BodyDef.type = b2_dynamicBody;
  BodyDef.fixedRotation = true;
  Body = level.World.CreateBody(&BodyDef);

  level.World.SetContactListener(&level.hasContact);

  b2CircleShape Shape;
  Shape.m_p.Set(0, 0);
  Shape.m_radius = radius;

  b2FixtureDef FixtureDef;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 0.0f;
  FixtureDef.shape = &Shape;
  FixtureDef.userData = (void *) this;
  FixtureDef.filter.categoryBits = B2D_PLAYER;
  FixtureDef.filter.maskBits = B2D_ENEMY | B2D_ITEM | B2D_LEVEL;
  Body->CreateFixture(&FixtureDef);
  Body->SetUserData((void*)this);
}

Tile &Character::getTile(int dx, int dy) {
  int tx = (int) (Body->GetPosition().x + 0.5f) + dx;
  int ty = (int) (Body->GetPosition().y + 0.5f) + dy;
  return level_->get(tx, ty);
}

float Character::getYSpeed() {
  return Body->GetLinearVelocity().y;
}

void Character::render(sf::RenderTarget &target) {
  sf::Sprite sprite;
  if (MoveDir != MoveDirection::NONE) {
    int walkSpriteId = ((int) ((level_->time - startWalkingTime) * 10)) % 7 + 1;
    sprite = level_->getData().getSprite("player_walk_" + std::to_string(walkSpriteId));
  } else {
    startWalkingTime = level_->time;
    sprite = level_->getData().getSprite("player_idle");
  }
  if (looksRight)
    sprite.scale(-1, 1);

  sprite.setOrigin(11.f, 14.f);
  sprite.setPosition(SCALE * Body->GetPosition().x,
                     SCALE * Body->GetPosition().y);
  sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
  target.draw(sprite);
}
