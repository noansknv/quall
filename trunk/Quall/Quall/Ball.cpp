#include "ball.hpp"

Ball::Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos, OgreCameraPtr camera, Ogre::String material, Ogre::Vector3 fin, Simulation *sim)
: ssim(sim), name("ball"), iloscKlatek(0), main(true), start(pos), stop(fin), ball_material(material), wysLew(0), opada(false), WorldElement(o, b, s, pos, camera)
{
    l = false;
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

Ogre::Vector3 Ball::getMiddlePosition(int x, int z)
{
	Ogre::Vector3 ret = Ogre::Vector3::ZERO;
	ret.x = x * 20 + 10;
	ret.z = -1 * (z * 20 + 10);
	return ret;
}

Ogre::Vector3 Ball::AI()
{
	Ogre::Vector3 pos = node->getPosition();
	int x = ((int)pos.x) / 20;
	int z = ((int)(-1 * pos.z)) / 20;
	// poczatkowo musi byc wiekszy od 15 * 17
	int tmpMax = 400;
	Ogre::Vector3 tmpRet;
	if (mainCharacter->plansza[x][z] == 1)
		return mainCharacter->node->getPosition();
	// dolny sasiad
	if (z > 0)
		if (mainCharacter->plansza[x][z - 1] == 1)
			return mainCharacter->node->getPosition();
		else if (mainCharacter->plansza[x][z - 1] > 1 && mainCharacter->plansza[x][z - 1] < tmpMax)
		{
			tmpMax = mainCharacter->plansza[x][z - 1];
			tmpRet = getMiddlePosition(x, z - 1);
		}
	// gorny sasiad
	if (z < 16)
		if (mainCharacter->plansza[x][z + 1] == 1)
			return mainCharacter->node->getPosition();
		else if (mainCharacter->plansza[x][z + 1] > 1 && mainCharacter->plansza[x][z + 1] < tmpMax)
		{
			tmpMax = mainCharacter->plansza[x][z + 1];
			tmpRet = getMiddlePosition(x, z + 1);
		}
	// lewy sasiad
	if (x > 0)
		if (mainCharacter->plansza[x - 1][z] == 1)
			return mainCharacter->node->getPosition();
		else if (mainCharacter->plansza[x - 1][z] > 1 && mainCharacter->plansza[x - 1][z] < tmpMax)
		{
			tmpMax = mainCharacter->plansza[x - 1][z];
			tmpRet = getMiddlePosition(x - 1, z);
		}
	// prawy sasiad
	if (x < 14)
		if (mainCharacter->plansza[x + 1][z] == 1)
			return mainCharacter->node->getPosition();
		else if (mainCharacter->plansza[x + 1][z] > 1 && mainCharacter->plansza[x + 1][z] < tmpMax)
		{
			tmpMax = mainCharacter->plansza[x + 1][z];
			tmpRet = getMiddlePosition(x + 1, z);
		}
	return tmpRet;
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
  Ogre::Vector3 toPosition;
  if (iloscKlatek++ == 30)
  {
    iloscKlatek = 0;
	toPosition = AI();
  }

  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);

  float x = trans.getOrigin().getX();
  float z = trans.getOrigin().getZ();
  float y = trans.getOrigin().getY();

  btVector3 old = fallRigidBody->getLinearVelocity();
  node->setPosition(x, y, z);
  btVector3 dir(toPosition.x - x, 0, toPosition.z - z);

  dir.normalize();
  btScalar speed = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(1.8);
  dir = dir * speed;

  fallRigidBody->setLinearVelocity(btVector3(dir.getX(), old.getY(), dir.getZ()));
  //fallRigidBody->setLinearVelocity(dir);

  Ogre::Vector3 m = mainCharacter->node->getPosition();
  if (m.squaredDistance(node->getPosition()) <= 1.6 * 1.6 && mainCharacter->l == false) {
  //if ((m.x - x)*(m.x - x) + (m.z - z)*(m.z - z) <= 1.5 * 1.5 && m.y <= 0.7 + 0.8) {
        MessageBoxA(NULL, "Aby zakoñczyæ grê kliknij ok", "Przegrales!", MB_OK);
        mainCharacter->l = true;
  }
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
     MessageBoxA(NULL, "Aby zakoñczyæ grê kliknij ok", "Wygra³eœ, hurra!", MB_OK);
	 ssim->requestStateChange(SHUTDOWN);
  }

  if (l)
      ssim->requestStateChange(SHUTDOWN);

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
  if (up != btVector3(0, 0, 0))
    up.normalize();
  btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(1.8);
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

