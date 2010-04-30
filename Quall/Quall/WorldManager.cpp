#include "WorldManager.hpp"

/**
 * Tworzy zarzadce swiatow.
 * Odpala fizyczny i graficzny opis sceny, nastepnie zbiera uchwyty do najwazniejszych jego czesci.
 */
WorldManager::WorldManager(InitWorldPtr worldDescription)
{
    ogWorld = worldDescription->getOgWorld();
    ogWindow = worldDescription->getOgWindow();

    // opisujemy swiat ogra i zbieramy plony...
    worldDescription->describeOgreWorld();

	// ...nastepnie opisujemy swiat bullet i rowniez zbieramy plony
    worldDescription->describeBulletWorld();
    btWorld = worldDescription->getBtWorld();

    // i dodajemy wszystkie elementy do swiata
    worldDescription->describeElements();
    elementsDeque = worldDescription->getElementsDeque();

	sceneManager = worldDescription->getOgSceneManager();
	mainCharacter = worldDescription->getMainCharacter();
    camera = worldDescription->getOgCamera();
	worldDescription->attachCamera(camera, mainCharacter->getOgNode());
}


void WorldManager::oneStep()
{
    btWorld->stepSimulation(1/60.f,10);

    // iterujemy sie po wszystkich dostepnych elementach ze swiata
    for (ElementDeque::iterator it = elementsDeque->begin(); it != elementsDeque->end(); ++it)
        (*it)->oneStep();

    ogWorld->renderOneFrame();
}


void WorldManager::setDirectionX(Ogre::Real x)
{
    //worldDescription->setDirectionX(x);
        for (ElementDeque::iterator it = elementsDeque->begin(); it != elementsDeque->end(); ++it)
        (*it)->setDirectionX(x);
}


void WorldManager::setDirectionZ(Ogre::Real z)
{
    //worldDescription->setDirectionZ(z);
    for (ElementDeque::iterator it = elementsDeque->begin(); it != elementsDeque->end(); ++it)
        (*it)->setDirectionZ(z);
}

void WorldManager::jump()
{
    for (ElementDeque::iterator it = elementsDeque->begin(); it != elementsDeque->end(); ++it)
        (*it)->jump();
}