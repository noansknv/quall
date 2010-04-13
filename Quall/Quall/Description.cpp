#include "Description.hpp"
#include "parser.hpp"

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
  // 4:3
  camera->setAspectRatio((Ogre::Real)1.333333);
  Ogre::Viewport* vp = ogWindow->addViewport(camera.get());
  vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

  //Ogre::SceneNode *cameraNode = sphereNode->createChildSceneNode("CameraNode", Ogre::Vector3(0, 200, 1000));
  //cameraNode->attachObject(camera.get());

  // pokazmy kuleczke
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  Ogre::Light* l = sceneManager->createLight("MainLight");
  l->setPosition(40, 20, 0);
  sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  //Examples/GrassFloor
  // plaszczyzna
  Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
    300, 300, 1, 1, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
  Ogre::Entity *ent = sceneManager->createEntity("GroundEntity", "ground");
  ent->setMaterialName("Examples/GrassFloor");
  Ogre::SceneNode *planeNode = sceneManager->getRootSceneNode()/*->createChildSceneNode()*/;
  
  //planeNode->setPosition(150, 0, -150);
  planeNode->attachObject(ent);
  //ent2->setMaterialName("Gruby/Red");

  // mesh by code
  //Ogre::SceneNode* mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  //mNode->attachObject(createCubeMesh(Ogre::Vector3(-40, 0, 0), Ogre::Vector3(-20, 10, -20), "Cube", "myMaterial"));
  //mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  //mNode->attachObject(createCubeMesh(Ogre::Vector3(-40, 0, 30), Ogre::Vector3(-20, 10, 10), "Cube", "myMaterial"));
  //mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  //mNode->attachObject(createCubeMesh(Ogre::Vector3(20, 0, -10), Ogre::Vector3(30, 10, -30), "Cube", "myMaterial"));
  //mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  //mNode->attachObject(createCubeMesh(Ogre::Vector3(20, 0, 20), Ogre::Vector3(40, 10, 0), "Cube", "myMaterial"));
  
  //mNode->setPosition(0,0,0);
  //mNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  //Point p1 = w.cubes[0].p1, p2 = w.cubes[0].p2;
  //mNode->attachObject(createCubeMesh(Ogre::Vector3(p1.x, p1.z, -1 * p1.y), Ogre::Vector3(p2.x, p2.z, -1 * p2.y), "Cube", "wall"));
  
  Ogre::SceneNode* mNode;
  for (int i = 0; i < parserWorld.cubes.size(); ++i)
  {
	mNode = sceneManager->getRootSceneNode()/*->createChildSceneNode()*/;
    Point p1 = parserWorld.cubes[i].p1, p2 = parserWorld.cubes[i].p2;
	//Ogre::MeshPtr ptr = createCubeMesh(Ogre::Vector3(p1.x, p1.z, -1 * p1.y), Ogre::Vector3(p2.x, p2.z, -1 * p2.y), "Cube" + i, "wall")->convertToMesh("");
	//Ogre::Entity *ent = new Ogre::Entity("ble", ptr);
	char *str = new char[5];
	sprintf(str, "%d", i);
	Ogre::String id(str);
	Ogre::String name = "Cube" + id;
	mNode->attachObject(createCubeMesh(Ogre::Vector3(p1.x, p1.z, -1 * p1.y), Ogre::Vector3(p2.x, p2.z, -1 * p2.y), name, "Examples/GrassFloor"));
  }
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
  Point p = parserWorld.spawnPoints[0].p;
  Ogre::Vector3 pos(p.x, p.z, -1 * p.y);
  ball.reset(new Ball(ogWorld, btWorld, sceneManager, pos));
  addElement(ball);
  // ustawiamy kuleczke jako glowna postac gry
  mainCharacter = ball;
}
