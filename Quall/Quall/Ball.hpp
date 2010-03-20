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
};
