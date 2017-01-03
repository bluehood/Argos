#include "Player.h"
#include <level/Level.h>
#include <Projectile.h>

Character::Character(Level &level, float x, float y) {
  level_ = &level;

  b2BodyDef BodyDef;
  BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
  BodyDef.type = b2_dynamicBody;
  BodyDef.fixedRotation = true;
  Body = level.World.CreateBody(&BodyDef);

  level.World.SetContactListener(&level.hasContact);

  float bottomWidth = width * 0.95f;
  float bottomHeight = height * 0.95f;

  b2Vec2 vertices[6];
  vertices[0].Set(-width,  -height);
  vertices[1].Set( width,  -height);
  vertices[2].Set( width, bottomHeight);
  vertices[3].Set( bottomWidth,  height);
  vertices[4].Set(-bottomWidth,  height);
  vertices[5].Set(-width,  bottomHeight);

  b2PolygonShape Shape;
  Shape.Set(vertices, 6);


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
  if (hangsOnCliff) {

    sf::Sprite sprite = level_->getData().getSprite("player_cliff");

    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    if (looksRight)
      sprite.scale(-1, 1);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    target.draw(sprite);
    return;
  }

  {
    sf::Sprite sprite = level_->getData().getSprite("player_body");

    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height);
    if (looksRight)
      sprite.scale(-1, 1);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    target.draw(sprite);
  }

  {
    sf::Sprite sprite;
    if (!hasContactInDirection(-b2_pi/2, b2_pi * 0.9f)) {
      startWalkingTime = level_->time;
      sprite = level_->getData().getSprite("player_jump");
    } else if (MoveDir != MoveDirection::NONE) {
      int walkSpriteId = ((int) ((level_->time - startWalkingTime) * 10)) % 7 + 1;
      sprite = level_->getData().getSprite(
          "player_walk_" + std::to_string(walkSpriteId));
    } else {
      startWalkingTime = level_->time;
      sprite = level_->getData().getSprite("player_idle");
    }

    sprite.setOrigin(sprite.getLocalBounds().width / 2, 0);
    if (looksRight)
      sprite.scale(-1, 1);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    target.draw(sprite);
  }

  {
    sf::Sprite sprite = level_->getData().getSprite("weapon_shotgun");

    if (looksRight)
      sprite.scale(-1, 1);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y - 1);
    target.draw(sprite);
  }
  {
    sf::Sprite sprite = level_->getData().getSprite("player_arm");

    if (looksRight)
      sprite.scale(-1, 1);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y - 1);
    target.draw(sprite);
  }
}

void Character::shoot() {
  level_->add(new Projectile(*level_, getBody()->GetPosition().x + 11 / SCALE, getBody()->GetPosition().y - 2 / SCALE,
                             {(looksRight ? 1 : -1) * 10.0F, 0}));
}
