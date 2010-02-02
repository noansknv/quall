/**
 * Definicje typow.
 *
 * @author marcin.kanclerz
 */
#pragma once

#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<Ogre::Root> OgreRootPtr;
typedef boost::shared_ptr<Ogre::SceneManager> OgreSceneManagerPtr;
typedef boost::shared_ptr<Ogre::Camera> OgreCameraPtr;
typedef boost::shared_ptr<Ogre::RenderWindow> OgreRenderWindowPtr;

typedef boost::shared_ptr<btDiscreteDynamicsWorld> BtDiscreteWorldPtr;
