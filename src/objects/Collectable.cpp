#include "Collectable.h"

Collectable::Collectable(Level &level, float x, float y) {
  sprite_ = level.getData().getSprite("hearth");

  sprite_.setOrigin(sprite_.getLocalBounds().width / 2,
                    sprite_.getLocalBounds().height / 2);

  b2BodyDef BodyDef;
  BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
  BodyDef.type = b2_dynamicBody;
  BodyDef.fixedRotation = true;
  Body = level.World.CreateBody(&BodyDef);

  b2PolygonShape Shape;

  Shape.SetAsBox((sprite_.getLocalBounds().width / 2) / SCALE,
                 (sprite_.getLocalBounds().height / 2) / SCALE);

  {
    b2FixtureDef FixtureDef;
    FixtureDef.density = 5.f;
    FixtureDef.friction = 0.9f;
    FixtureDef.shape = &Shape;
    FixtureDef.userData = (void *) this;
    FixtureDef.filter.categoryBits = B2D_ITEM;
    FixtureDef.filter.maskBits = B2D_LEVEL;
    Body->CreateFixture(&FixtureDef);
  }

  {
    b2FixtureDef FixtureDef;
    FixtureDef.isSensor = true;
    FixtureDef.shape = &Shape;
    FixtureDef.userData = (void *) this;
    FixtureDef.filter.categoryBits = B2D_ITEM;
    FixtureDef.filter.maskBits = B2D_PLAYER;
    Body->CreateFixture(&FixtureDef);
  }

  Body->SetUserData((void *) this);
}

void Collectable::apply(Character &c) {
  Logger l;
  l << "Foo" << l;
}
