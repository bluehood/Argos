#ifndef ARGOS_CONTACTCHECKER_H
#define ARGOS_CONTACTCHECKER_H


#include <Box2D/Dynamics/b2WorldCallbacks.h>

class ContactChecker : public b2ContactListener {
  void BeginContact(b2Contact* contact);

  void EndContact(b2Contact* contact);
};


#endif //ARGOS_CONTACTCHECKER_H
