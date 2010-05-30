#include "Description.hpp"

Description::Description(OgreRootPtr o, OgreRenderWindowPtr w, Simulation *sim) : InitWorld(o, w), ssim(sim)
{
}

/**
* Po poprawieniu koordynatow do nakladania tekstur oraz ustawieniu cieni, zwracamy gotowego cube'a.
*/
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
* Utworzenie sceny: ustawienie swiata, wspolczynnikow kamery, managera i masy innych rzeczy.
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
  sceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 10);

  // swiatlo i cienie
  sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  Ogre::Light* l = sceneManager->createLight("MainLight");
  l->setPosition(-10, 500, 10);
  sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  // plaszczyzna
  Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
	parserWorld.size.x, parserWorld.size.z, 1, 1, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
  Ogre::Entity *ent = sceneManager->createEntity("GroundEntity", "ground");
  ent->setMaterialName(parserWorld.floor);
  Ogre::SceneNode *planeNode = sceneManager->getRootSceneNode()->createChildSceneNode();
  planeNode->setPosition(parserWorld.size.x / 2, 0, -1 * (parserWorld.size.z / 2));
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
    Ogre::ManualObject *mCube = createCubeMesh(Ogre::Vector3(p1.x, p1.y, -1 * p1.z), Ogre::Vector3(p2.x, p2.y, -1 * p2.z), name, wall_material);
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

  btWorld->setGravity(btVector3(0,-30,0));

  //plaszczyzna
  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

  btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
  btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
  btWorld->addRigidBody(groundRigidBody);

  //sciany
  for (int i = 0; i < parserWorld.cubes.size(); ++i)
  {
      Point p1 = parserWorld.cubes[i].p1;
      Point p2 = parserWorld.cubes[i].p2;
      btVector3 v(abs(p2.x - p1.x)/2, abs(p2.y - p1.y)/2, abs(p2.z - p1.z)/2);

      btCollisionShape* cubeShape = new btBoxShape(v);
      btDefaultMotionState* cubeMotionState = new 
         btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3((p2.x + p1.x)/2, (p2.y + p1.y)/2, (-1 * p2.z - p1.z)/2)));
      //btDefaultMotionState* cubeMotionState = new 
        //  btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(p1.x, p1.y, -1 * p1.z)));
      //cubeMotionState->getWorldTransform(->setOrigin(btVector3(p1.x, p1.y, p1.z);

      btRigidBody::btRigidBodyConstructionInfo
                cubeRigidBodyCI(0, cubeMotionState, cubeShape, btVector3(0,0,0));
      btRigidBody* cubeRigidBody = new btRigidBody(cubeRigidBodyCI);
      btWorld->addRigidBody(cubeRigidBody);
  }

  int i = 1;
  //Ogre::String ball_name;
  for (i = 1; (i+1) < parserWorld.spawnPoints.size(); i++)
  {
	  //char *str = new char[5];
	  //sprintf(str, "%d", i);
	  //Ogre::String id(str);
	  //ball_name = "ball" + id;
	  Point p = parserWorld.spawnPoints[i+1].p;
      btCollisionShape* fallShape = new btSphereShape(0.65);

    btDefaultMotionState* fallMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(p.x, p.y, p.z)));
    btScalar mass = 20;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
//  fallRigidBody->setMassProps(mass, fallInertia);
    btWorld->addRigidBody(fallRigidBody);
	  //pos.x = p.x;
	  //pos.y = p.y;
	  //pos.z = (-1) * p.z;
	 // ball.reset(new Ball(ogWorld, btWorld, sceneManager, pos, camera, parserWorld.ball, ball_name));
	  //addElement(ball);
  }

  /*btVector3 v(5, 5, 5);

  btCollisionShape* cubeShape = new btBoxShape(v);
  btDefaultMotionState* cubeMotionState = new
    btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(49, 6, -29)));

  btRigidBody::btRigidBodyConstructionInfo
  cubeRigidBodyCI(0, cubeMotionState, cubeShape, btVector3(0,0,0));
  btRigidBody* cubeRigidBody = new btRigidBody(cubeRigidBodyCI);
  btWorld->addRigidBody(cubeRigidBody);*/

}

/**
 * Wstawienie spawnpointu, dynamicznych elementow planszy i glownego bohatera - kuleczki.
 */
void Description::describeElements()
{
  WorldElementPtr ball;
  Point p = parserWorld.spawnPoints[0].p;
  Point p2 = parserWorld.spawnPoints[1].p;
  Ogre::Vector3 pos(p.x, p.y, -1 * p.z);
  Ogre::Vector3 fin(p2.x, p2.y, -1 * p2.z);
  ball.reset(new Ball(ogWorld, btWorld, sceneManager, pos, camera, parserWorld.ball, fin, ssim));
  for (int x = 0; x < 15; ++x)
	  for (int z = 0; z < 17; ++z)
	  {
		  if (parserWorld.tab[x][z])
			ball->plansza[x][z] = 0;
		  else
			  ball->plansza[x][z] = -1;
	  }
  addElement(ball);
  // ustawiamy kuleczke jako glowna postac gry
  mainCharacter = ball;

  int i = 1;
  Ogre::String ball_name;
  for (i = 1; (i+1) < parserWorld.spawnPoints.size(); i++)
  {
	  char *str = new char[5];
	  sprintf(str, "%d", i);
	  Ogre::String id(str);
	  ball_name = "ball" + id;
	  p = parserWorld.spawnPoints[i+1].p;
	  pos.x = p.x;
	  pos.y = p.y;
	  pos.z = (-1) * p.z;
	  ball.reset(new Ball(ogWorld, btWorld, sceneManager, pos, camera, parserWorld.ball, ball_name, mainCharacter.get()));
	  addElement(ball);
  }

  Ogre::Entity *ent;

  for (i = 0; i < parserWorld.meshes.size(); i++)
  {
	Ogre::SceneNode *node;

	char *str = new char[5];
	sprintf(str, "%d", i);
	Ogre::String id(str);
	ball_name = "meshek" + id;
	ent = sceneManager->createEntity(ball_name, parserWorld.meshes[i].mesh_name);
	ent->setMaterialName(parserWorld.meshes[i].material_name);
	ent->setCastShadows(true);
	
	Ogre::Vector3 position(parserWorld.meshes[i].p.x, parserWorld.meshes[i].p.y, (-1) * parserWorld.meshes[i].p.z);

	node = sceneManager->getRootSceneNode()->createChildSceneNode(ball_name);
	node->setPosition(position);
	Ogre::SceneNode* nodeLew = node->createChildSceneNode();
	nodeLew->attachObject(ent);
  }

}
