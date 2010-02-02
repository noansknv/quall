/**
* @author marcin.kanclerz
*/
#pragma once

#include "btBulletDynamicsCommon.h"
#include "Ogre.h"

#include "InitWorld.hpp"

class WorldManager;

typedef boost::shared_ptr<WorldManager> WorldManagerPtr;

/**
* Klasa wspolodpowiedzialna za synchronizacje swiatow: fizycznego oraz graficznego.
* Chociaz InitWorld tworzy podstawowy uklad swiata, to WorldManager jest odpowiedzialny
* za wszelkie pozniejsze operacje na nim wespol z MotionState-ami
*/
class WorldManager
{
public:
  WorldManager(InitWorldPtr worldDesc);

  // Wykonuje pojedynczy krok calego swiata.
  void oneStep();

  void setDirectionX(Ogre::Real x);
  void setDirectionZ(Ogre::Real z);

private:
  // Uchwyt swiata fizycznego Bulleta.
  BtDiscreteWorldPtr btWorld;

  // Uchwyt swiata graficznego Ogra.
  OgreRootPtr ogWorld;

  // Uchwyt okna wyswietlajacego.
  OgreRenderWindowPtr ogWindow;

  // Uchwyt opisu swiatow.
  //InitWorldPtr worldDescription;

  // Uchwyt kolejki wszystkich elementow swiata.
  ElementDequePtr elementsDeque;

  // Zarzadca sceny Ogra.
  OgreSceneManagerPtr sceneManager;

  // Aktualna kamera. Byc moze tymczasowe rozwiazanie.
  OgreCameraPtr camera;
};
