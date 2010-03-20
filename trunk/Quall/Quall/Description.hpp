/**
* @author marcin.kanclerz
*/
#pragma once

#include "InitWorld.hpp"
#include "WorldElement.hpp"
#include "Ball.hpp"

/**
* Opis przykladowego swiata.
* Tutaj - plaska powierzchnia z opadajaca na nia kulka.
*/
class Description : public InitWorld
{
public:
  Description(OgreRootPtr o, OgreRenderWindowPtr w);
  void describeOgreWorld();
  void describeBulletWorld();
  void describeElements();
private:
  /*
  * fbl - front bottom left vertex
  * btr - back top right vertex
  * name - nazwa mesh'a
  * matName - nazwa materialu
  */
  Ogre::ManualObject* createCubeMesh(Ogre::Vector3 fbl, Ogre::Vector3 btr, Ogre::String name, Ogre::String matName);
};
