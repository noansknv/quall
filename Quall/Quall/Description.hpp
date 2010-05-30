/**
* Opis przykladowego swiata.
* Plaska powierzchnia z opadajaca na nia kulka.
* @author marcin.kanclerz filip.mazowiecki
*/
#pragma once

#include <cmath>
#include "InitWorld.hpp"
#include "WorldElement.hpp"
#include "Ball.hpp"
#include "parser.hpp"
#include "Simulation.hpp"


class Description : public InitWorld
{
public:
  Description(OgreRootPtr o, OgreRenderWindowPtr w, Simulation *s);
  void describeOgreWorld();
  void describeBulletWorld();
  void describeElements();
private:
	Simulation *ssim;
  /*
  * fbl - front bottom left vertex
  * btr - back top right vertex
  * name - nazwa mesh'a
  * matName - nazwa materialu
  */
  Ogre::ManualObject* createCubeMesh(Ogre::Vector3 fbl, Ogre::Vector3 btr, Ogre::String name, Ogre::String matName);
};
