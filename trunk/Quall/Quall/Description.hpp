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
  WorldElementPtr ball;
};
