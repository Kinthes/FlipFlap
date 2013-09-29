#ifndef __BSPHERE_H_
#define __BSPHERE_H_

#include <btBulletDynamicsCommon.h>
#include <Bullet-C-Api.h>
#include <btBulletCollisionCommon.h>

#include <OgreEntity.h>
#include <OgreSceneManager.h>

#include "MyMotionState.h"

class BSphere //for Bullet
{
    public:
        BSphere(const Ogre::String &name, btDynamicsWorld *world, Ogre::SceneManager *smgr, const btScalar& rayon, float mass);
        virtual ~BSphere();

        btRigidBody* getRigidBody();
        Ogre::SceneNode* getSceneNode();
        Ogre::Entity* getEntity();

    protected:
        btDynamicsWorld         *m_world;
        btCollisionShape        *m_shape;
        btRigidBody             *m_body;

        Ogre::SceneManager      *m_smgr;
        Ogre::Entity            *m_ent;
        Ogre::SceneNode         *m_node;

};

#endif
