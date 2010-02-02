#include "ball.hpp"

Ball::Ball(OgreRootPtr o, BtDiscreteWorldPtr b, OgreSceneManagerPtr s)
: WorldElement(o, b, s), direction(Ogre::Vector3::ZERO), move(0.1)
{
}


void Ball::describeOgreElement()
{
  Ogre::Entity* ent = sceneManager->createEntity("ballmesh", "ball.mesh");
  ent->setCastShadows(true);
  sceneManager->getRootSceneNode()->createChildSceneNode("ball")->attachObject(ent);
  node = sceneManager->getRootSceneNode()->getChild("ball");
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

  node->setPosition(node->getPosition().x, trans.getOrigin().getY(), node->getPosition().z);
  // Przesuniecie kulki w poziomie.
  node->translate((direction * move)/* * evt.timeSinceLastFrame*/, Ogre::Node::TS_LOCAL);
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
