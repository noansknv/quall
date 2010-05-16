/**
* Klasa implementujaca glownego bohatera gry - kulke.
* Rozszerza WorldElement by uzyskac zunifikowany interfejs dla wszystkich dynamicznych obiektow
* wystepujacych w grze.
* @author marcin.kanclerz
*/
#pragma once

#include "WorldElement.hpp"
#include "Types.hpp"
#include "obiekty.hpp"

class Ball : public WorldElement
{
public:
  Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos, OgreCameraPtr camera, Ogre::String material, Ogre::Vector3 fin);
  Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos, OgreCameraPtr camera, Ogre::String material, Ogre::String b_name);
  void describeOgreElement();
  void describeBulletElement();
  void oneStep();
  void oneStep_main();
  void oneStep_enemy();
  void setDirectionX(Ogre::Real x);
  void setDirectionZ(Ogre::Real z);
  void jump();
  bool canJump();
  bool main;
private:
  Ogre::Real wysLew;
  Ogre::String ball_material;
  Ogre::String name;
  Ogre::Vector3 start;
  Ogre::Vector3 stop;
  bool opada;
  /**
   * Dodatkowy wezel potrzebny do animacji lewitacji.
   */
  Ogre::SceneNode *nodeLew;
};
