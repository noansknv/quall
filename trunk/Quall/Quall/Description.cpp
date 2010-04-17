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
  cube->position(fbl);cube->textureCoord(0,0);
  cube->position(btr.x, fbl.y, fbl.z);cube->textureCoord(1,0);
  cube->position(btr.x, btr.y, fbl.z);cube->textureCoord(1,1);
  cube->position(fbl.x, btr.y, fbl.z);cube->textureCoord(0,1);
  //top
  cube->position(fbl.x, btr.y, fbl.z);cube->textureCoord(0,0);
  cube->position(btr.x, btr.y, fbl.z);cube->textureCoord(1,0);
  cube->position(btr.x, btr.y, btr.z);cube->textureCoord(1,1);
  cube->position(fbl.x, btr.y, btr.z);cube->textureCoord(0,1);
  //bottom
  cube->position(fbl);cube->textureCoord(0,1);
  cube->position(btr.x, fbl.y, fbl.z);cube->textureCoord(1,1);
  cube->position(btr.x, fbl.y, btr.z);cube->textureCoord(1,0);
  cube->position(fbl.x, fbl.y, btr.z);cube->textureCoord(0,0);
  //left
  cube->position(fbl);cube->textureCoord(1,0);
  cube->position(fbl.x, btr.y, fbl.z);cube->textureCoord(1,1);
  cube->position(fbl.x, btr.y, btr.z);cube->textureCoord(0,1);
  cube->position(fbl.x, fbl.y, btr.z);cube->textureCoord(0,0);
  //right
  cube->position(btr);cube->textureCoord(1,1);
  cube->position(btr.x, btr.y, fbl.z);cube->textureCoord(0,1);
  cube->position(btr.x, fbl.y, fbl.z);cube->textureCoord(0,0);
  cube->position(btr.x, fbl.y, btr.z);cube->textureCoord(1,0);
  //back
  cube->position(btr);cube->textureCoord(0,1);
  cube->position(btr.x, fbl.y, btr.z);cube->textureCoord(0,0);
  cube->position(fbl.x, fbl.y, btr.z);cube->textureCoord(1,0);
  cube->position(fbl.x, btr.y, btr.z);cube->textureCoord(1,1);
  // front face
  cube->quad(0, 1, 2, 3);
  // top face
  cube->quad(4, 5, 6, 7);
  // bottom face
  cube->quad(8, 9, 10, 11);
  // left face
  cube->quad(12, 13, 14, 15);
  // right face
  cube->quad(16, 17, 18, 19);
  // back face
  cube->quad(20, 21, 22, 23);
  cube->setCastShadows(true);
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

  // swiatlo i cienie
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  Ogre::Light* l = sceneManager->createLight("MainLight");
  l->setPosition(40, 20, 0);
  sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  // plaszczyzna
  Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
	parserWorld.size.x, parserWorld.size.y, 1, 1, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
  Ogre::Entity *ent = sceneManager->createEntity("GroundEntity", "ground");
  ent->setMaterialName("Quall/Floor");
  Ogre::SceneNode *planeNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  planeNode->setPosition(150, 0, -150);
  planeNode->attachObject(ent);

  // sciany
  Ogre::SceneNode* mRootNode = sceneManager->getRootSceneNode();
  for (int i = 0; i < parserWorld.cubes.size(); ++i)
  {
    Point p1 = parserWorld.cubes[i].p1, p2 = parserWorld.cubes[i].p2;
	Ogre::String wall_material(parserWorld.cubes[i].texture);
	char *str = new char[5];
	sprintf(str, "%d", i);
	Ogre::String id(str);
	Ogre::String name = "Cube" + id;
    Ogre::ManualObject *mCube = createCubeMesh(Ogre::Vector3(p1.x, p1.z, -1 * p1.y), Ogre::Vector3(p2.x, p2.z, -1 * p2.y), name, wall_material);
	mCube->convertToMesh(name);
	Ogre::Entity *ent = sceneManager->createEntity(name, name);
	mRootNode->attachObject(ent);
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