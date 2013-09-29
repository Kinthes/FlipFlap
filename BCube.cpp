#include "BCube.h"

BCube::BCube(const Ogre::String& name, btDynamicsWorld* world,
             Ogre::SceneManager* smgr, const btVector3& halfdim, float mass) :
                                                                m_world(world), m_smgr(smgr)
{
        m_ent = m_smgr->createEntity(name+"_ent", Ogre::SceneManager::PT_CUBE);
        m_node = m_smgr->getRootSceneNode()->createChildSceneNode(name+"_node");
        m_node->attachObject(m_ent);
        m_node->scale(Ogre::Vector3(halfdim.x(), halfdim.y(), halfdim.z()) * (1.f/50));

        m_shape = new btBoxShape(halfdim);
        btVector3 inertia;
        m_shape->calculateLocalInertia(mass, inertia);

        btTransform posMat;
        posMat.setOrigin(btVector3(0,0,0));
        posMat.setRotation(btQuaternion(0,0,0,1));

        motionState = new MyMotionState(posMat, m_node);
        btRigidBody::btRigidBodyConstructionInfo BodyCI(mass, motionState, m_shape, inertia);
        m_body = new btRigidBody(BodyCI);
        m_world->addRigidBody(m_body);
}

BCube::~BCube()
{
        delete m_body->getMotionState();
        m_world->removeRigidBody(m_body);
        delete m_body;
        delete m_shape;
        m_smgr->destroySceneNode(m_node);
        m_smgr->destroyEntity(m_ent);
}

btRigidBody* BCube::getRigidBody()
{
        return m_body;
}

Ogre::Entity* BCube::getEntity()
{
        return m_ent;
}

Ogre::SceneNode* BCube::getSceneNode()
{
        return m_node;
}

MyMotionState* BCube::getMyMotionState()
{
        return motionState;
}
