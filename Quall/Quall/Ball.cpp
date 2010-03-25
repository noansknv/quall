#include "ball.hpp"

Ball::Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s, Ogre::Vector3 pos)
: wysLew(0), opada(false), WorldElement(o, b, s, pos)
{
}


void Ball::describeOgreElement()
{
  Ogre::Entity* ent = sceneManager->createEntity("ballmesh", "ball.mesh");
  ent->setCastShadows(true);
  node = sceneManager->getRootSceneNode()->createChildSceneNode("ball");
  node->setPosition(position);
  nodeLew = node->createChildSceneNode();
  nodeLew->attachObject(ent);
}


void Ball::describeBulletElement()
{
  btCollisionShape* fallShape = new btSphereShape(1);

  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,20,0)));
  btScalar mass = 1;
  btVector3 fallInertia(0,0,0);
  fallShape->calculateLocalInertia(mass,fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
  fallRigidBody = new btRigidBody(fallRigidBodyCI);
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

  node->setPosition(node->getPosition().x, trans.getOrigin().getY(), node->getPosition().z);
  // Przesuniecie kulki w poziomie.
  node->translate((direction * move)/* * evt.timeSinceLastFrame*/, Ogre::Node::TS_LOCAL);
  nodeLew->setPosition(0, wysLew, 0);

  // co to jest?! :)
  Ogre::WindowEventUtilities::messagePump();
}


void Ball::setDirectionX(Ogre::Real x)
{
  direction.x = x;
}

void Ball::setDirectionZ(Ogre::Real z)
{
  direction.z = z;
}
