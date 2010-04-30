/**
* @author marcin.kanclerz
*/
#pragma once

#include "Ogre.h"
#include "btBulletDynamicsCommon.h"

#include "Types.hpp"

class WorldElement;

typedef boost::shared_ptr<WorldElement> WorldElementPtr;

// Typ listy elementow.
typedef std::deque<WorldElementPtr> ElementDeque;
// Typ wskaznika do listy elementow.
typedef boost::shared_ptr<ElementDeque> ElementDequePtr;

/**
* Abstrakcyjna klasa opisujaca dynamiczne elementy swiata, takie jak np. kulka.
* Opis jest osobny dla Ogra, a osobny dla Bulleta.
*/
class WorldElement
{
public:
  // Inline-owy konstruktor, poniewaz plik .cpp jest zbedny.
	WorldElement(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos)
		: ogWorld(o), btWorld(b), sceneManager(s), direction(Ogre::Vector3::ZERO), move(0.5), position(pos)
  {
  }

  // Opisuje element dla ogra.
  virtual void describeOgreElement() = 0;

  // Opisuje element dla bulleta.
  virtual void describeBulletElement() = 0;

  // Wykonuje pojedynczy krok symulacji.
  virtual void oneStep() = 0;

  // Modyfikuje wektor kierunku ruchu.
  virtual void setDirectionX(Ogre::Real x)
  {
  }

  // Modyfikuje wektor kierunku ruchu.
  virtual void setDirectionZ(Ogre::Real z)
  {
  }

  virtual void jump()
  {
  }

  Ogre::SceneNode* getOgNode() { return node; }

protected:
  OgreRootPtr ogWorld;
  OgreSceneManagerPtr sceneManager;
  BtDiscreteWorldPtr btWorld;

  // Uchwyt elementu w bullecie.
  btRigidBody* fallRigidBody;

  // Uchwyt elementu w ogrze.
  Ogre::SceneNode *node;

  // Punkt startowy pojawienia sie elementu.
  Ogre::Vector3 position;

  // Wspolczynnik predkosci ruchu.
  Ogre::Real move;

  // Wektor kierunku ruchu.
  Ogre::Vector3 direction;
};
