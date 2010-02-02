#include "Description.hpp"

/*
* Implementacja konstruktora. Smart.
*/
Description::Description(OgreRootPtr o, OgreRenderWindowPtr w) : InitWorld(o, w)
{
}


/**
* Skrajnie hardkodowane utworzenie menedzera sceny, kamery i ustawienia kolorow.
* Nastepnie SILOWO tworzona jest kuleczka.
*/
void Description::describeOgreWorld()
{
  sceneManager.reset(ogWorld->createSceneManager(Ogre::ST_GENERIC));
  camera.reset(sceneManager->createCamera("camera"));
  camera->setNearClipDistance(5);

  // testy
  //Ogre::Entity *sphereEnt = sceneManager->createEntity("Sphere", "sphere.mesh");
  //Ogre::SceneNode *sphereNode = sceneManager->getRootSceneNode()->createChildSceneNode("SphereNode", Ogre::Vector3(0, 200, 0));
  //sphereNode->attachObject(sphereEnt);

  //Ogre::SceneNode *cameraNode = sphereNode->createChildSceneNode("CameraNode", Ogre::Vector3(0, 200, 1000));
  //cameraNode->attachObject(camera.get());

  camera->setPosition(Ogre::Vector3(0,20,20));
  camera->lookAt(Ogre::Vector3(0,0,0));
  Ogre::Viewport* vp = ogWindow->addViewport(camera.get());
  vp->setBackgroundColour(Ogre::ColourValue(0.5,0.5,0.5));

  // 4:3
  camera->setAspectRatio((Ogre::Real)1.333333);

  // pokazmy kuleczke
  sceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));
  sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  // plaszczyzna
  Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
    20,20,20,20,true,1,5,5,Ogre::Vector3::UNIT_Z);

  Ogre::Entity *ent2 = sceneManager->createEntity("GroundEntity", "ground");
  sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(ent2);
  ent2->setMaterialName("Gruby/Red");
}


/**
* Opis fizyczny obiektow istniejacych w formie graficznej w Ogrze.
*/
void Description::describeBulletWorld() {
  btVector3 worldAabbMin(-10000,-10000,-10000);
  btVector3 worldAabbMax(10000,10000,10000);
  int maxProxies = 1024;
  btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

  btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

  //BtDiscreteWorldPtr 
  btWorld.reset(new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration));

  btWorld->setGravity(btVector3(0,-1,0));


  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

  btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
  btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
  btWorld->addRigidBody(groundRigidBody);
}


void Description::describeElements()
{
  ball.reset(new Ball(ogWorld, btWorld, sceneManager));
  addElement(ball);
}
