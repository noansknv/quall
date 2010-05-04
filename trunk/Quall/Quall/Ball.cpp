#include "ball.hpp"

Ball::Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos)
: wysLew(0), opada(false), WorldElement(o, b, s, pos)
{
}


void Ball::describeOgreElement()
{
  Ogre::Entity* ent = sceneManager->createEntity("ballmesh", "ball.mesh");
  ent->setCastShadows(true);
  ent->setMaterialName("Quall/Ball");
  node = sceneManager->getRootSceneNode()->createChildSceneNode("ball");
  node->setPosition(position);
  nodeLew = node->createChildSceneNode();
  nodeLew->attachObject(ent);
}


void Ball::describeBulletElement()
{
  btCollisionShape* fallShape = new btSphereShape(1);

  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(50,30,-30)));
  btScalar mass = 1;
  btVector3 fallInertia(0,0,0);
  fallShape->calculateLocalInertia(mass,fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
  fallRigidBody = new btRigidBody(fallRigidBodyCI);
//  fallRigidBody->setMassProps(mass, fallInertia);
  btWorld->addRigidBody(fallRigidBody);
}


void Ball::oneStep()
{
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);

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
}


void Ball::setDirectionX(Ogre::Real x)
{
    //fallRigidBody->clearForces();
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);
  
  //btVector3 up = trans.getBasis()[2];
  btVector3 up(x, 0, 0);
  btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(1.0);
  fallRigidBody->applyCentralImpulse(up * magnitude);
}

void Ball::setDirectionZ(Ogre::Real z)
{
  //fallRigidBody->clearForces();
  //direction.z = z;
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);
  
  //btVector3 up = trans.getBasis()[2];
  btVector3 up(0, 0, z);
  btScalar magnitude = (btScalar(1.0)/fallRigidBody->getInvMass()) * btScalar(1.0);
  fallRigidBody->applyCentralImpulse(up * magnitude);
}

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

bool Ball::canJump() 
{
  btTransform trans;
  fallRigidBody->getMotionState()->getWorldTransform(trans);

  return trans.getOrigin().getY() <= 2;
}

