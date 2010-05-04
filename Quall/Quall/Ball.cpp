#include "ball.hpp"

Ball::Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos, OgreCameraPtr camera, Ogre::String material, Ogre::Vector3 fin)
: start(pos), stop(fin), ball_material(material), wysLew(0), opada(false), WorldElement(o, b, s, pos, camera)
{
}

/**
 * Kulka korzysta z materialow zdefiniowanych w trakcie inicjalizacji calego systemu.
 */
void Ball::describeOgreElement()
{
  Ogre::Entity* ent = sceneManager->createEntity("ballmesh", "ball.mesh");
  ent->setCastShadows(true);
  ent->setMaterialName(ball_material);
  node = sceneManager->getRootSceneNode()->createChildSceneNode("ball");
  node->setPosition(position);
  nodeLew = node->createChildSceneNode();
  nodeLew->attachObject(ent);
}


void Ball::describeBulletElement()
{
  btCollisionShape* fallShape = new btSphereShape(1.2);

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


/**
 * Obsluga paru ciekawych ficzerow glownego bohatera, m.in. animacji lewitacji czy faktycznego poruszania sie.
 */
void Ball::oneStep()
{
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);

  Ogre::Vector3 posit(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

  if (posit.squaredDistance(stop) <= 100)
  {

  }

  // pomocne przy animacji lewitacji
  if (opada && wysLew >= 0.01)
    wysLew -= 0.01;
  else if (!opada && wysLew <= 1)
	  wysLew += 0.01;
  else
    if (opada)
	  opada = false;
	else
      opada = true;

  node->setPosition(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()); 
  //node->setPosition(node->getPosition().x, trans.getOrigin().getY(), node->getPosition().z);
  // Przesuniecie kulki w poziomie.
  //node->translate((direction * move)/* * evt.timeSinceLastFrame*/, Ogre::Node::TS_LOCAL);
  nodeLew->setPosition(0, trans.getOrigin().getY() + wysLew, 0);
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
  fallRigidBody->setLinearVelocity(up * magnitude + btVector3(0, old.getY(), 0));

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
    btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(8.0);
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

  return trans.getOrigin().getY() <= 2;
}

