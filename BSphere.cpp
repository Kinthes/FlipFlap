
#include "BSphere.h"

BSphere::BSphere(const Ogre::String& name, btDynamicsWorld* world,
             Ogre::SceneManager* smgr, const btScalar& rayon, float mass) :
                                                                m_world(world), m_smgr(smgr)
{
        m_ent = m_smgr->createEntity(name+"_ent", Ogre::SceneManager::PT_SPHERE);
        m_node = m_smgr->getRootSceneNode()->createChildSceneNode(name+"_node");
        m_node->attachObject(m_ent);
        m_node->scale(Ogre::Vector3(rayon, rayon, rayon) * (1.f/50));

        m_shape = new btSphereShape(rayon);
        btVector3 inertia;
        m_shape->calculateLocalInertia(mass, inertia);

        btTransform posMat;
        posMat.setOrigin(btVector3(0.f,0.f,0.f));
        posMat.setRotation(btQuaternion(0.f,0.f,0.f,1.f));

        MyMotionState *motionState = new MyMotionState(posMat, m_node);
        btRigidBody::btRigidBodyConstructionInfo BodyCI(mass, motionState, m_shape, inertia);
        m_body = new btRigidBody(BodyCI);
        m_world->addRigidBody(m_body);
}

BSphere::~BSphere()
{
        delete m_body->getMotionState();
        m_world->removeRigidBody(m_body);
        delete m_body;
        delete m_shape;
        m_smgr->destroySceneNode(m_node);
        m_smgr->destroyEntity(m_ent);
}

btRigidBody* BSphere::getRigidBody()
{
        return m_body;
}

Ogre::Entity* BSphere::getEntity()
{
        return m_ent;
}

Ogre::SceneNode* BSphere::getSceneNode()
{
        return m_node;
}

