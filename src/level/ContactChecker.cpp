#include <GameObject.h>
#include <objects/Collectable.h>
#include <players/Player.h>
#include "ContactChecker.h"

static GameObject* getGO(b2Fixture* f) {
  return reinterpret_cast<GameObject *>(f->GetUserData());
}

static void checkCollectable(GameObject* A, GameObject* B) {
  if (auto Collect = dynamic_cast<Collectable *>(A)) {
    if (auto Player = dynamic_cast<Character *>(B)) {
      Collect->apply(*Player);
    }
  }
}

void ContactChecker::BeginContact(b2Contact *contact) {
  checkCollectable(getGO(contact->GetFixtureA()), getGO(contact->GetFixtureB()));
  checkCollectable(getGO(contact->GetFixtureB()), getGO(contact->GetFixtureA()));
}

void ContactChecker::EndContact(b2Contact *contact) {
}
