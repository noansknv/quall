#include "Description.hpp"

/*
* Implementacja konstruktora. Smart.
*/
Description::Description(OgreRootPtr o, OgreRenderWindowPtr w) : InitWorld(o, w)
{
}

// daje nam cube'a ladnego, trzeba updatenac troche tylko :)
Ogre::ManualObject* Description::createCubeMesh(Ogre::Vector3 fbl, Ogre::Vector3 btr, Ogre::String name, Ogre::String matName)
{
  Ogre::ManualObject* cube = new Ogre::ManualObject(name);
  cube->begin(matName);
  //front
  cube->position(fbl);
  cube->position(btr.x, fbl.y, fbl.z);
  cube->position(btr.x, btr.y, fbl.z);
  cube->position(fbl.x, btr.y, fbl.z);
  //back
  cube->position(btr);
  cube->position(btr.x, fbl.y, btr.z);
  cube->position(fbl.x, fbl.y, btr.z);
  cube->position(fbl.x, btr.y, btr.z);
  // front face
  cube->quad(0, 1, 2, 3);
  // back face
  cube->quad(4, 5, 6, 7);
  // top face
  cube->quad(3, 2, 4, 7);
  // bottom face
  cube->quad(0, 6, 5, 1);
  // left face
  cube->quad(0, 3, 7, 6);
  // right face
  cube->quad(1, 5, 4, 2);
  cube->end();
  return cube;
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

  //Ogre::SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode("CamNode", Ogre::Vector3(0, 20, 20));
  //node->attachObject(camera);
  camera->setPosition(Ogre::Vector3(0, 20, 20));
  camera->lookAt(Ogre::Vector3(0, 0, 0));
  Ogre::Viewport* vp = ogWindow->addViewport(camera.get());
  vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

  // 4:3
  camera->setAspectRatio((Ogre::Real)1.333333);

  // pokazmy kuleczke
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  Ogre::Light* l = sceneManager->createLight("MainLight");
  l->setPosition(20, 80, 50);
  sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  // plaszczyzna
  /*Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
    20,20,20,20,true,1,5,5,Ogre::Vector3::UNIT_Z);

  Ogre::Entity *ent2 = sceneManager->createEntity("GroundEntity", "ground");
  sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(ent2);
  ent2->setMaterialName("Gruby/Red");*/

  // mesh by code
  Ogre::SceneNode* mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  mNode->attachObject(createCubeMesh(Ogre::Vector3(-20, -10, 0), Ogre::Vector3(-10, 0, -10), "Cube", "myMaterial"));
  mNode->setPosition(0,0,0);
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
  WorldElementPtr ball;
  ball.reset(new Ball(ogWorld, btWorld, sceneManager));
  addElement(ball);
  // ustawiamy kuleczke jako glowna postac gry
  mainCharacter = ball;
}
