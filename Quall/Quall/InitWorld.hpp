/**
* @author marcin.kanclerz
*/
#pragma once

#include <deque>

#include "Ogre.h"
#include "btBulletDynamicsCommon.h"

#include "Types.hpp"
#include "WorldElement.hpp"

class InitWorld;

typedef boost::shared_ptr<InitWorld> InitWorldPtr;

/**
* Abstrakcyjna klasa sluzaca, do opisu poczatkowego stanu sceny.
* Opisuje obiekty (powerupy, spawnpointy, etc), podloze, wszystkie materialy i cala fizyke planszy.
*/
class InitWorld
{
public:
    InitWorld(OgreRootPtr o, OgreRenderWindowPtr w);
  ~InitWorld();

  // Opisuje swiat graficzny. Musi zdefiniowac menadzera sceny oraz kamere.
  virtual void describeOgreWorld() = 0;

  // Opisuje swiat fizyczny. Musi utworzyc swiat Bulleta.
  virtual void describeBulletWorld() = 0;

  // Opisuje wszystkie dynamiczne elementy swiata - dodaje je.
  virtual void describeElements() = 0;
  
  // Gettery i settery.
  OgreRootPtr getOgWorld() { return ogWorld; }
  OgreRenderWindowPtr getOgWindow() { return ogWindow; }
  OgreSceneManagerPtr getOgSceneManager() { return sceneManager; }
  OgreCameraPtr getOgCamera() { return camera; }
  BtDiscreteWorldPtr getBtWorld() { return btWorld; }
  ElementDequePtr getElementsDeque() { return elementsDeque; }

protected:
  OgreSceneManagerPtr sceneManager;
  BtDiscreteWorldPtr btWorld;
  OgreCameraPtr camera;
  OgreRootPtr ogWorld;
  OgreRenderWindowPtr ogWindow;
  
  // Lista elementow dynamicznych znajdujacych sie w danym swiecie
  ElementDequePtr elementsDeque;

  // Dodaje element do swiata.
  virtual void addElement(WorldElementPtr we);
};
