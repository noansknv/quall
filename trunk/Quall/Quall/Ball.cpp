#include "ball.hpp"

Ball::Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos, OgreCameraPtr camera, Ogre::String material, Ogre::Vector3 fin, Simulation *sim)
: ssim(sim), name("ball"), iloscKlatek(0), main(true), start(pos), stop(fin), ball_material(material), wysLew(0), opada(false), WorldElement(o, b, s, pos, camera)
{
	mainCharacter = this;
    for (int i = 0; i < 15; i++)
		for (int j = 0; j < 17; j++)
			plansza[i][j] = 0;
}

Ball::Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos, OgreCameraPtr camera, Ogre::String material, Ogre::String b_name, WorldElement *pMain)
: name(b_name), main(false), iloscKlatek(0), start(pos), ball_material(material), WorldElement(o, b, s, pos, camera), mainCharacter(pMain)
{
}

void Ball::wyzerujPlansze()
{
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 17; j++)
			if (plansza[i][j] != -1)
				plansza[i][j] = 0;
}
// pole z glownym bohaterem bedzie rowne 1 i potem im dalej tym wiekszy numerek
void Ball::przeliczPlansze()
{
	wyzerujPlansze();
	Ogre::Vector3 pos = node->getPosition();
	int x = ((int)pos.x) / 20;
	int z = ((int)(-1 * pos.z)) / 20;
    plansza[x][z] = 1;
	oznaczSasiadow(x, z);
}

void Ball::oznaczSasiadow(int x, int z)
{
	// dolny sasiad
	if (z > 0)
		if (plansza[x][z - 1] == 0 || plansza[x][z - 1] > plansza[x][z] + 1)
		{
			plansza[x][z - 1] = plansza[x][z] + 1;
			oznaczSasiadow(x, z - 1);
		}
	// gorny sasiad
	if (z < 16)
		if (plansza[x][z + 1] == 0 || plansza[x][z + 1] > plansza[x][z] + 1)
		{
			plansza[x][z + 1] = plansza[x][z] + 1;
			oznaczSasiadow(x, z + 1);
		}
	// lewy sasiad
	if (x > 0)
		if (plansza[x - 1][z] == 0 || plansza[x - 1][z] > plansza[x][z] + 1)
		{
			plansza[x - 1][z] = plansza[x][z] + 1;
			oznaczSasiadow(x - 1, z);
		}
	// prawy sasiad
	if (x < 14)
		if (plansza[x + 1][z] == 0 || plansza[x + 1][z] > plansza[x][z] + 1)
		{
			plansza[x + 1][z] = plansza[x][z] + 1;
			oznaczSasiadow(x + 1, z);
		}
}

Ogre::Vector3 Ball::AI()
{
	/*Ogre::Vector3 pos = node->getPosition();
	int x = ((int)pos.x) / 20;
	int z = ((int)(-1 * pos.z)) / 20;
	if (mainCharacter->plansza[x][y] == 1)*/
		return mainCharacter->node->getPosition();
	// dolny sasiad
	//if (z > 0)
	//	if (plansza[x][z - 1] == 0 || plansza[x][z - 1] > plansza[x][z] + 1)
	//	{
	//		plansza[x][z - 1] = plansza[x][z] + 1;
	//		oznaczSasiadow(x, z - 1);
	//	}
	//// gorny sasiad
	//if (z < 16)
	//	if (plansza[x][z + 1] == 0 || plansza[x][z + 1] > plansza[x][z] + 1)
	//	{
	//		plansza[x][z + 1] = plansza[x][z] + 1;
	//		oznaczSasiadow(x, z + 1);
	//	}
	//// lewy sasiad
	//if (x > 0)
	//	if (plansza[x - 1][z] == 0 || plansza[x - 1][z] > plansza[x][z] + 1)
	//	{
	//		plansza[x - 1][z] = plansza[x][z] + 1;
	//		oznaczSasiadow(x - 1, z);
	//	}
	//// prawy sasiad
	//if (x < 14)
	//	if (plansza[x + 1][z] == 0 || plansza[x + 1][z] > plansza[x][z] + 1)
	//	{
	//		plansza[x + 1][z] = plansza[x][z] + 1;
	//		oznaczSasiadow(x + 1, z);
	//	}
}

/**
 * Kulka korzysta z materialow zdefiniowanych w trakcie inicjalizacji calego systemu.
 */
void Ball::describeOgreElement()
{
  Ogre::String b_name = "ballmesh";
  if (!main) b_name = name;
  Ogre::Entity* ent = sceneManager->createEntity(b_name, "ball.mesh");
  ent->setCastShadows(true);
  if (main)
	  ent->setMaterialName(ball_material);
  else
	  ent->setMaterialName("Quall/Enemy");
  b_name = "ball";
  if (!main) b_name = name;
  node = sceneManager->getRootSceneNode()->createChildSceneNode(b_name);
  node->setPosition(position);
  nodeLew = node->createChildSceneNode();
  nodeLew->attachObject(ent);
}


void Ball::describeBulletElement()
{
  btCollisionShape* fallShape = new btSphereShape(0.77);

  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(start.x, start.y, start.z)));
  btScalar mass = 20;
  btVector3 fallInertia(0,0,0);
  fallShape->calculateLocalInertia(mass,fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
  fallRigidBody = new btRigidBody(fallRigidBodyCI);
//  fallRigidBody->setMassProps(mass, fallInertia);
  btWorld->addRigidBody(fallRigidBody);
}

void Ball::oneStep_enemy()
{
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);

  node->setPosition(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}


/**
 * Obsluga paru ciekawych ficzerow glownego bohatera, m.in. animacji lewitacji czy faktycznego poruszania sie.
 */
void Ball::oneStep_main()
{
  if (iloscKlatek++ == 30)
  {
	iloscKlatek = 0;
	przeliczPlansze();
	this->plansza;
  }
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);

  Ogre::Vector3 posit(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

  if (posit.squaredDistance(stop) <= 100)
  {
     MessageBoxA(NULL, "Aby zako�czy� gr� kliknij ok", "Wygra�e�, hurra!", MB_OK);
	 ssim->requestStateChange(SHUTDOWN);
  }

  // pomocne przy animacji lewitacji
  if (opada && wysLew >= 0.01)
    wysLew -= 0.01;
  else if (!opada && wysLew <= 0.3)
	  wysLew += 0.01;
  else
    if (opada)
	  opada = false;
	else
      opada = true;

  node->setPosition(trans.getOrigin().getX(), trans.getOrigin().getY() + wysLew, trans.getOrigin().getZ()); 
  //node->setPosition(node->getPosition().x, trans.getOrigin().getY(), node->getPosition().z);
  // Przesuniecie kulki w poziomie.
  //node->translate((direction * move)/* * evt.timeSinceLastFrame*/, Ogre::Node::TS_LOCAL);
  //nodeLew->setPosition(0, trans.getOrigin().getY() + wysLew, 0);
  //btVector3 przesuniecie((direction * move).x, (direction * move).y, (direction * move).z);
  //btVector3 nowe = trans.getOrigin() + przesuniecie;
  //trans.setOrigin(nowe);

  // co to jest?! :)
  Ogre::WindowEventUtilities::messagePump();

  //fallRigidBody->getMotionState()->setWorldTransform(trans);
  //m_rigidBody->setCenterOfMassTransform (trans);

  Ogre::Vector3 v = camera->getRealDirection();
  btVector3 old = fallRigidBody->getLinearVelocity();
//  old.setX(0)
//  old.setZ(0);
  btVector3 up(v.x, 0, v.z);
  up = up * btVector3(direction.z, 0, direction.z);
  btVector3 up2(-1 * v.z, 0, v.x);
  up2 = up2 * btVector3(direction.x, 0, direction.x);
  up += up2;
  btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(1.0);
  //btVector3 old = fallRigidBody->getLinearVelocity();
  if (canJump())
      fallRigidBody->setLinearVelocity(up * magnitude + btVector3(0, old.getY(), 0));
  if (!canJump()) {
      //setDirectionX(0);
      //setDirectionZ(0);
      fallRigidBody->setLinearVelocity(btVector3(old.getX(), old.getY(), old.getZ()));
  }

}

//void Ball:o

void Ball::oneStep()
{
	if (main)
		oneStep_main();
	else
		oneStep_enemy();
}

void Ball::setDirectionX(Ogre::Real x)
{
  direction.x = x;
/*  if (x == 0.0) {
      fallRigidBody->setLinearVelocity(btVector3(0, 0, 0));
  } else {
      Ogre::Vector3 v = camera->getRealDirection();
      btVector3 up(-1 * v.z, 0, v.x);
      up = up * btScalar(x);
      btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(1.0);
      btVector3 old = fallRigidBody->getLinearVelocity();
      fallRigidBody->setLinearVelocity(up * magnitude + old);
  }*/
}

void Ball::setDirectionZ(Ogre::Real z)
{
  direction.z = -1 * z;
  /*if (z == 0.0) {
      fallRigidBody->setLinearVelocity(btVector3(0, 0, 0));
  } else { 
      Ogre::Vector3 v = camera->getRealDirection();
      btVector3 up(v.x, 0, v.z);
      up = up * btScalar(-1 * z);
      btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(1.0);
      btVector3 old = fallRigidBody->getLinearVelocity();
      fallRigidBody->setLinearVelocity(up * magnitude + old);
  }*/
}


/**
 * Uczymy sie skakac - przykladamy odpowiednia sile o odpowiednim zwrocie i kierunku do naszego obiektu.
 */
void Ball::jump()
{
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);
  
  if (canJump()) {
    //btVector3 up = trans.getBasis()[1];
    btVector3 up(0, 1, 0);
    btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(14.0);
    fallRigidBody->applyCentralImpulse(up * magnitude);
  }
}

/**
 * Blokujemy mozliwosc skakania zbyt wysoko.
 */
bool Ball::canJump() 
{
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);

  return trans.getOrigin().getY() <= 0.9 && main;
}

