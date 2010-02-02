#include "initWorld.hpp"

InitWorld::InitWorld(OgreRootPtr o, OgreRenderWindowPtr w)
: ogWorld(o), ogWindow(w)
{
    elementsDeque.reset(new ElementDeque());
}


InitWorld::~InitWorld() {}


void InitWorld::addElement(WorldElementPtr worldElement)
{
    worldElement->describeOgreElement();
    worldElement->describeBulletElement();

    // dodaje element do listy obiektow danego swiata
    elementsDeque->push_back(worldElement);
}