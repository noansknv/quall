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

class Euler
{
public:
    // Constructor which takes yaw, pitch and roll values.
    Euler(Ogre::Radian y = Ogre::Radian(0.0f), Ogre::Radian p = Ogre::Radian(0.0f), Ogre::Radian r = Ogre::Radian(0.0f)):m_yaw(y),m_pitch(p),m_roll(r),m_changed(true) 
    {
    }

    // Get the Yaw angle.
    inline Ogre::Radian getYaw() 
    {
        return m_yaw;
    }

    // Get the Pitch angle.
    inline Ogre::Radian getPitch() 
    {
        return m_pitch;
    }

    // Get the Roll angle.
    inline Ogre::Radian getRoll() 
    {
        return m_roll;
    }

    // Apply a relative yaw. (Adds angle to current yaw)
    // Angles wrap around within the range 0 to 2*PI radians (0 to 360 degrees)
    inline Euler &yaw(Ogre::Radian y) 
    {
        m_yaw += y; 
        if(m_yaw.valueRadians() < 0.0f)
        {
            m_yaw = fmod(m_yaw.valueRadians(), Ogre::Math::PI * 2.0f) + Ogre::Math::PI * 2.0f;
        }
        else if(m_yaw.valueRadians() > Ogre::Math::PI)
        {
            m_yaw = fmod(m_yaw.valueRadians(), Ogre::Math::PI * 2.0f);
        }
        m_changed = true; 
        return *this;
    }

    // Apply a relative pitch. (Adds angle to current pitch)
    inline Euler &pitch(Ogre::Radian p) 
    {
        m_pitch += p; 
        m_changed = true; 
        return *this;
    }

    // Apply a relative roll. (Adds angle to current roll)
    inline Euler &roll(Ogre::Radian r) 
    {
        m_roll += r; 
        m_changed = true; 
        return *this;
    }

    // Set the yaw.
    inline Euler &setYaw(Ogre::Radian y) 
    {
        m_yaw = y; 
        m_changed = true; 
        return *this;
    }

    // Set the pitch.
    inline Euler &setPitch(Ogre::Radian p) 
    {
        m_pitch = p; 
        m_changed = true; 
        return *this;
    }

    // Set the roll.
    inline Euler &setRoll(Ogre::Radian r) 
    {
        m_roll = r; 
        m_changed = true; 
        return *this;
    }

    // Get a vector pointing forwards.
    inline Ogre::Vector3 getForward() 
    {
        return toQuaternion() * Ogre::Vector3::NEGATIVE_UNIT_Z;
    }

    // Get a vector pointing to the right.
    inline Ogre::Vector3 getRight() 
    {
        return toQuaternion() * Ogre::Vector3::UNIT_X;
    }

    // Get a vector pointing up.
    inline Ogre::Vector3 getUp() 
    {
        return toQuaternion() * Ogre::Vector3::UNIT_Y;
    }

    // Calculate the quaternion of a euler object.
    // The result is cached, it is only recalculated when the component euler angles are changed.
    inline Ogre::Quaternion toQuaternion() 
    {
        if(m_changed) 
        {
            m_cachedQuaternion = Ogre::Quaternion(m_yaw, Ogre::Vector3::NEGATIVE_UNIT_Y) * Ogre::Quaternion(m_pitch, Ogre::Vector3::UNIT_X) * Ogre::Quaternion(m_roll, Ogre::Vector3::NEGATIVE_UNIT_Z); 
            m_changed = false;
        }
        return m_cachedQuaternion;
    }

    // Casting operator. This allows any ogre function that wants a Quaternion to accept a Euler instead.
    inline operator Ogre::Quaternion() 
    {
        return toQuaternion();
    }

    // Set the yaw and pitch to face in the given direction.
    // The direction doesn't need to be normalised.
    // Roll is unaffected.
    Euler &setDirection(const Ogre::Vector3 &v)
    {
        Ogre::Vector3 d(v.normalisedCopy());
        m_pitch = asin(d.y);
        m_yaw = atan2(d.z, d.x)+Ogre::Math::PI/2.0;
        m_changed = true;
        
        return *this;
    }

    // Get the angular difference between the current yaw and the specified yaw.
    // Only yaw is considered, pitch and roll are ignored.
    inline Ogre::Radian getYawToDirection(const Ogre::Radian &a)
    {
        Ogre::Real angle = (a - m_yaw).valueRadians();
        if(angle>Ogre::Math::PI)
            angle = -Ogre::Math::PI*2.0f + angle;
        else if(angle<-Ogre::Math::PI)
            angle = Ogre::Math::PI*2.0f + angle;
        return Ogre::Radian(angle);
    }

    // Get the angular difference between the current yaw and the specified direction vector.
    // Only yaw is considered, pitch and roll are ignored.
    inline Ogre::Radian getYawToDirection(const Ogre::Vector3 &v)
    {
        return getYawToDirection(Ogre::Radian(atan2(v.z, v.x) + Ogre::Math::PI/2.0f));
    }

    // Get the angular difference between the current yaw and the specified euler object.
    // Only yaw is considered, pitch and roll are ignored.
    inline Ogre::Radian getYawToDirection(const Euler &e)
    {
        return getYawToDirection(e.m_yaw);
    }

    // Change the yaw to face in the direction of the vector.
    // Only yaw is changed, pitch and roll are ignored.
    inline Euler &yawToDirection(const Ogre::Vector3 &v)
    {
        m_yaw = getYawToDirection(v);
        m_changed = true;
        return *this;
    }

    // Change the yaw to face in the direction of the euler object.
    // Only yaw is changed, pitch and roll are ignored.
    inline Euler &yawToDirection(const Euler &e)
    {
        m_yaw = getYawToDirection(e);
        m_changed = true;
        return *this;
    }

    // stream operator, for printing the euler component angles to a stream
    inline friend std::ostream &operator<<(std::ostream &o, const Euler &e)
    {
        o << "<Y:" << e.m_yaw << ", P:" << e.m_pitch << ", R:" << e.m_roll << ">";
        return o;
    }

protected:
    Ogre::Radian m_yaw;     // Rotation around the Y axis.
    Ogre::Radian m_pitch;   // Rotation around the X axis.
    Ogre::Radian m_roll;    // Rotation around the Z axis.
    Ogre::Quaternion m_cachedQuaternion;    // Cached quaternion equivalent of this euler object.
    bool m_changed;         // Is the cached quaternion out of date?
};

// MouseListener
bool InputHandler::mouseMoved(const OIS::MouseEvent &evt)
{
  Ogre::Real mRotate = 0.13;
  worldMgr->getCamera()->getParentSceneNode()->getParentSceneNode()->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), Ogre::Node::TS_WORLD);
  Ogre::Real yCam = worldMgr->getCamera()->getParentSceneNode()->_getDerivedPosition().y;
  Ogre::Real yMain = worldMgr->getCamera()->getParentSceneNode()->getParentSceneNode()->_getDerivedPosition().y;
  Ogre::Real dist = worldMgr->getCamera()->getParentSceneNode()->_getDerivedPosition().distance(worldMgr->getCamera()->getParentSceneNode()->getParentSceneNode()->_getDerivedPosition());
  Ogre::Radian originalKat = Ogre::Math::ASin((yCam - yMain) / dist);
  Ogre::Radian newKat = originalKat + Ogre::Degree(mRotate * evt.state.Y.rel);

  if (newKat.valueDegrees() > 0 && newKat.valueDegrees() < 70)
    worldMgr->getCamera()->getParentSceneNode()->getParentSceneNode()->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);

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
  case OIS::KC_SPACE:
    //jump
    worldMgr->jump();
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
