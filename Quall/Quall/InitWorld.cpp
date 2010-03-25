#include "initWorld.hpp"
#include "parser.hpp"

InitWorld::InitWorld(OgreRootPtr o, OgreRenderWindowPtr w)
: ogWorld(o), ogWindow(w)
{
  Parser p("..\\..\\resource\\plansza.xml");
  p.readFile();
  parserWorld = p.getWorld();

  elementsDeque.reset(new ElementDeque());
}


InitWorld::~InitWorld() {}

void InitWorld::attachCamera(OgreCameraPtr cam, Ogre::SceneNode *node)
{
  node->attachObject(cam.get());
  cam->setPosition(Ogre::Vector3(0, 20, 20));
  cam->lookAt(node->getPosition());
}


void InitWorld::addElement(WorldElementPtr worldElement)
{
  worldElement->describeOgreElement();
  worldElement->describeBulletElement();

  // dodaje element do listy obiektow danego swiata
  elementsDeque->push_back(worldElement);
}