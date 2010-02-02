/**
* @author marcin.kanclerz
*/
#pragma once

#include "WorldElement.hpp"
#include "Types.hpp"

class Ball : public WorldElement
{
public:
  Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s);
  void describeOgreElement();
  void describeBulletElement();
  void oneStep();
  void setDirectionX(Ogre::Real x);
  void setDirectionZ(Ogre::Real z);
private:
  // Uchwyt kulki w bullecie.
  btRigidBody* fallRigidBody;

  // Uchwyt kulki w ogrze.
  Ogre::Node* node;

  // Wspolczynnik predkosci ruchu.
  Ogre::Real move;

  // Wektor kierunku ruchu.
  Ogre::Vector3 direction;
};
