/**
* Glowna klasa odpowiedzialna za input gry.
* @author Pawel Kozlowski
*/
#pragma once

#include <boost/shared_ptr.hpp>
#include "OIS\OISEvents.h"
#include "OIS\OISInputManager.h"
#include "OIS\OISMouse.h"
#include "OIS\OISKeyboard.h"
#include "OgreStringConverter.h"

#include "WorldManager.hpp"
#include "Simulation.hpp"

class InputHandler;

typedef boost::shared_ptr<InputHandler> InputHandlerPtr;

class InputHandler :
  public OIS::MouseListener,
  public OIS::KeyListener
{
private:
  OIS::InputManager *mOis;
  OIS::Mouse *mMouse;
  OIS::Keyboard *mKeyboard;
  unsigned long mHWnd;
  Simulation *mSimulation;
  WorldManagerPtr worldMgr;
public:
  InputHandler(WorldManagerPtr worldMgr, Simulation *sim, unsigned long hWnd);
  ~InputHandler();

  void setWindowExtents(int width, int height);
  /** Zlapanie odpowiednich eventow - u nas: klawiatury i myszki. */
  void capture();

  /** Event poruszenia mysza. */
  bool mouseMoved(const OIS::MouseEvent &evt);
  /** Event wcisniecia przycisku myszy */
  bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID);
  bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID);

  /** Event wcisniecia przycisku klawiatury. */
  bool keyPressed(const OIS::KeyEvent &evt);
  /** Event puszczenia przycisku klawiatury. */
  bool keyReleased(const OIS::KeyEvent &evt);
};
