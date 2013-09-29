#ifndef __MYMOTIONSTATE_H_
#define __MYMOTIONSTATE_H_

class MyMotionState : public btMotionState
{
    public:
        MyMotionState(const btTransform &initialPos, Ogre::SceneNode *node)
        {
                mVisibleObj = node;
                mPos1=initialPos;
        }
        virtual ~MyMotionState()
        { }

        void setNode(Ogre::SceneNode *node)
        {
                mVisibleObj=node;
        }

        void setKinematicPos(btTransform &currentPos)
        {
            mPos1 = currentPos;
            btQuaternion rot=mPos1.getRotation();
                mVisibleObj->setOrientation(rot.w(), rot.x(), rot.y(),rot.z());
                btVector3 pos = mPos1.getOrigin();
                mVisibleObj->setPosition(pos.x(), pos.y(), pos.z());

        }

        virtual void getWorldTransform(btTransform &worldTrans) const
        {
                worldTrans=mPos1;

        }

        virtual void setWorldTransform(const btTransform &worldTrans)
        {
                if(mVisibleObj==NULL) return; //silently return before we set a node
                btQuaternion rot=worldTrans.getRotation();
                mVisibleObj->setOrientation(rot.w(), rot.x(), rot.y(),rot.z());
                btVector3 pos = worldTrans.getOrigin();
                mVisibleObj->setPosition(pos.x(), pos.y(), pos.z());
        }

    protected:
        Ogre::SceneNode* mVisibleObj;
        btTransform mPos1;
};

#endif
