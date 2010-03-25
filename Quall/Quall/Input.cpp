#include "input.hpp"


InputHandler::InputHandler(WorldManagerPtr worldManager, Simulation *sim, unsigned long hWnd)
: worldMgr(worldManager)
{
  OIS::ParamList pl;
  pl.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

  mHWnd = hWnd;
  mOis = OIS::InputManager::createInputSystem(pl);
  mMouse = static_cast<OIS::Mouse*>(mOis->createInputObject(OIS::OISMouse, true));
  mKeyboard = static_cast<OIS::Keyboard*>(mOis->createInputObject(OIS::OISKeyboard, true));
  mMouse->setEventCallback(this);
  mKeyboard->setEventCallback(this);
  mSimulation = sim;
}

InputHandler::~InputHandler()
{
  if (mMouse)
    delete mMouse;
  if (mKeyboard)
    delete mKeyboard;
  OIS::InputManager::destroyInputSystem(mOis);
}

void InputHandler::capture()
{
  mMouse->capture();
  mKeyboard->capture();
}

void  InputHandler::setWindowExtents(int width, int height)
{
  //Set Mouse Region.. if window resizes, we should alter this to reflect as well
  const OIS::MouseState &mouseState = mMouse->getMouseState();
  mouseState.width = width;
  mouseState.height = height;
}


// MouseListener
bool InputHandler::mouseMoved(const OIS::MouseEvent &evt)
{
  Ogre::Real mRotate = 0.13;
  worldMgr->getCamera()->getParentSceneNode()->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), Ogre::Node::TS_WORLD);
  worldMgr->getCamera()->getParentSceneNode()->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);
  return true;
}

bool InputHandler::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID btn)
{
  return true;
}

bool InputHandler::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID btn)
{
  return true;
}


// KeyListener
bool InputHandler::keyPressed(const OIS::KeyEvent &evt)
{
  // Poruszanie sie kuleczka.
  switch (evt.key)
  {
  case OIS::KC_W:
    //if (mSimulation->getCurrentState() == SIMULATION)
    worldMgr->setDirectionZ(-1);
    break;
  case OIS::KC_S:
    //if (mSimulation->getCurrentState() == SIMULATION)
    worldMgr->setDirectionZ(1);
    break;
  case OIS::KC_A:
    //if (mSimulation->getCurrentState() == SIMULATION)
    worldMgr->setDirectionX(-1);
    break;
  case OIS::KC_D:
    //if (mSimulation->getCurrentState() == SIMULATION)
    worldMgr->setDirectionX(1);
    break;
  default:
    break;
  }
  return true;
}

bool InputHandler::keyReleased(const OIS::KeyEvent &evt)
{
  switch (evt.key)
  {
  case OIS::KC_ESCAPE:
    mSimulation->requestStateChange(SHUTDOWN);
    break;
  case OIS::KC_W:
  case OIS::KC_S:
    //if (mSimulation->getCurrentState() == SIMULATION)
    worldMgr->setDirectionZ(0);
    break;
  case OIS::KC_A:
  case OIS::KC_D:
    //if (mSimulation->getCurrentState() == SIMULATION)
    worldMgr->setDirectionX(0);
    break;
  default:
    break;
  }
  return true;
}
