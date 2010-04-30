/**
* @author marcin.kanclerz
*/
#pragma once

#include "WorldElement.hpp"
#include "Types.hpp"

class Ball : public WorldElement
{
public:
  Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos);
  void describeOgreElement();
  void describeBulletElement();
  void oneStep();
  void setDirectionX(Ogre::Real x);
  void setDirectionZ(Ogre::Real z);
  void jump();
  bool canJump();
private:
  Ogre::Real wysLew;
  bool opada;
  // Dodatkowy wezel potrzebny do animacji lewitacji.
  Ogre::SceneNode *nodeLew;
};
