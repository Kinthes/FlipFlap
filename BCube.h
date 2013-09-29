#ifndef __BCUBE_H_
#define __BCUBE_H_

#include <btBulletDynamicsCommon.h>
#include <Bullet-C-Api.h>
#include <btBulletCollisionCommon.h>

#include <OgreEntity.h>
#include <OgreSceneManager.h>

#include "MyMotionState.h"

class BCube //for Bullet
{
    public:
        BCube(const Ogre::String &name, btDynamicsWorld *world, Ogre::SceneManager *smgr, const btVector3 &dim, float mass);
        virtual ~BCube();

        btRigidBody* getRigidBody();
        Ogre::SceneNode* getSceneNode();
        Ogre::Entity* getEntity();

        MyMotionState* getMyMotionState();
    protected:
        btDynamicsWorld         *m_world;
        btCollisionShape        *m_shape;
        btRigidBody             *m_body;

        Ogre::SceneManager      *m_smgr;
        Ogre::Entity            *m_ent;
        Ogre::SceneNode         *m_node;

        MyMotionState           *motionState;
};

#endif
