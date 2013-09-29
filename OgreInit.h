#ifndef __OGRE_INIT_H
#define __OGRE_INIT_H

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <btBulletDynamicsCommon.h>         //
#include <Bullet-C-Api.h>                   //> ------ Bullet Headers
#include <btBulletCollisionCommon.h>        //

#include "SdkTrays.h"




class OgreInit : public Ogre::Singleton<OgreInit>,
                    OIS::KeyListener,
                    OIS::MouseListener,
                    OgreBites::SdkTrayListener
{
   public:
            OgreInit();
            ~OgreInit();

            bool startOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener =0, OIS::MouseListener *pMouseListener=0);
            void updateOgre(double timeSinceLastFrame);
            void moveCamera();
            void getInput();

            bool isShutdown()const{return mShutdown;}

            bool keyPressed(const OIS::KeyEvent &keyEventRef);
            bool keyReleased(const OIS::KeyEvent &keyEventRef);
            bool mouseMoved(const OIS::MouseEvent &evt);
            bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
            bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    //variables, ajout d'accesseurs pour private
            Ogre::Root          *pRoot;
            Ogre::SceneManager  *pSceneMgr;
            Ogre::RenderWindow  *pRenderWnd;
            Ogre::Camera        *pCamera;
            Ogre::Viewport      *pViewport;
            Ogre::Log           *pLog;
            Ogre::Timer         *pTimer;
            Ogre::LogManager    *pLogMgr;

            OIS::InputManager   *pInputMgr;
            OIS::Keyboard       *pKeyboard;
            OIS::Mouse          *pMouse;

            btDbvtBroadphase                       *m_broadphase;
            btDefaultCollisionConfiguration        *m_collisionConfiguration;
            btCollisionDispatcher                  *m_dispatcher;
            btSequentialImpulseConstraintSolver    *m_solver;
            btDiscreteDynamicsWorld                *m_world;

  private:
            bool                mShutdown;
            int                 mNumScreenShots;
            Ogre::FrameEvent    mFrameEvent;
            OgreBites::SdkTrayManager *pTrayMgr;

            Ogre::Vector3       mTranslateVector;
            Ogre::Real          mMoveSpeed;
            Ogre::Degree        mRotateSpeed;
            float               mMoveScale;
            Ogre::Degree        mRotateScale;

            OgreInit(const OgreInit&);
            OgreInit &operator= (const OgreInit&);
};



#endif
