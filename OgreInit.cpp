#include "OgreInit.h"

using namespace Ogre;

template<> OgreInit* Ogre::Singleton<OgreInit>::ms_Singleton=0;

//---------------------------------------------------------------------
OgreInit::OgreInit()
{
    mMoveSpeed              = 0.1f;
    mRotateSpeed            = 0.3f;
    mShutdown               = false;
    mNumScreenShots         = 0;
    pLogMgr                 = 0;
    pRoot                   = 0;
    pSceneMgr               = 0;
    pRenderWnd              = 0;
    pCamera                 = 0;
    pViewport               = 0;
    pLog                    = 0;
    pTimer                  = 0;
    pInputMgr               =0;
    pKeyboard               = 0;
    pMouse                  =0;

    pTrayMgr                = 0;
    mFrameEvent             = Ogre::FrameEvent();
}//OgreInit()

//--------------------------------------------------------------------
OgreInit::~OgreInit()
{
    if(pInputMgr) OIS::InputManager::destroyInputSystem(pInputMgr);
    if(pRoot) delete pRoot;
    if(pLogMgr) delete pLogMgr;

    delete m_broadphase;
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_solver;
    delete m_world;
   // if(pTimer) delete pTimer;
   // if(pTrayMgr) delete pTrayMgr;
}//~OgreInit

//---------------------------------------------------------------------
bool OgreInit::startOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener , OIS::MouseListener *pMouseListener)
{
    // Initialisation du monde virtuel utilisé par Bullet
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;
    m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    m_world->setGravity(btVector3(0,-25,0));

    pLogMgr = new Ogre::LogManager();
    pLog = pLogMgr->createLog("LogFile.log", true, true, false);
    pLog->setDebugOutputEnabled(true);

    pRoot = new Ogre::Root();
    if(!pRoot->showConfigDialog()) return false;

    pRenderWnd = pRoot->initialise(true, wndTitle);
    //Initialisation du SceneManager
    pSceneMgr = pRoot->createSceneManager(ST_GENERIC, "SceneManager");
    pSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.4f));

    //Initialisation de la camera
    pCamera=pSceneMgr->createCamera("MainCamera");
    pCamera->setPosition(Vector3(0,60,120));
    pCamera->lookAt(Vector3(0,0,0));
    pCamera->setNearClipDistance(1);
    //Initialisation du viewport
    pViewport=pRenderWnd->addViewport(pCamera);
    pViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.0f,1.0f));
    pCamera->setAspectRatio(Real(pViewport->getActualWidth()) / Real(pViewport->getActualHeight()));
    pViewport->setCamera(pCamera);
    //Initialisation de OIS
    unsigned long hwnd=0;
    OIS::ParamList paramList;
    pRenderWnd->getCustomAttribute("WINDOW",&hwnd);
    paramList.insert(OIS::ParamList::value_type("WINDOW",Ogre::StringConverter::toString(hwnd)));

    pInputMgr=OIS::InputManager::createInputSystem(paramList);
    pKeyboard=static_cast<OIS::Keyboard*>(pInputMgr->createInputObject(OIS::OISKeyboard,true));
    pMouse=static_cast<OIS::Mouse*>(pInputMgr->createInputObject(OIS::OISMouse, true));
    pMouse->getMouseState().height = pRenderWnd->getHeight();
    pMouse->getMouseState().width = pRenderWnd->getWidth();

    if(pKeyListener == 0)
        pKeyboard->setEventCallback(this);
    else
        pKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
        pMouse->setEventCallback(this);
    else
        pMouse->setEventCallback(pMouseListener);

    //Definition des ressources
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName=seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i=settings->begin(); i!=settings->end(); i++)
        {
            typeName=i->first;
            archName=i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    pTimer = new Ogre::Timer();
    pTimer->reset();

    pTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", pRenderWnd , pMouse, this);
    pTrayMgr->hideLogo();
    pTrayMgr->hideFrameStats();
    pTrayMgr->hideCursor();

    pRenderWnd->setActive(true);
    return true;
}//startOgre

//---------------------------------------------------------------------
void OgreInit::updateOgre(double timeSinceLastFrame)
{
    m_world->stepSimulation(timeSinceLastFrame*0.001, 10); //(1.f/60, 10);

    mMoveScale = mMoveSpeed * (float)timeSinceLastFrame;
    mRotateScale = mRotateSpeed * (float)timeSinceLastFrame;

    mTranslateVector = Vector3::ZERO;

    getInput();
    moveCamera();

    mFrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    pTrayMgr->frameRenderingQueued(mFrameEvent);
}//updateOgre

//---------------------------------------------------------------------
void OgreInit::moveCamera()
{
    if(pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        pCamera->moveRelative(mTranslateVector);
    else
        pCamera->moveRelative(mTranslateVector / 10);
}//moveCamera

//--------------------------------------------------------------------
void OgreInit::getInput()
{
    if(pKeyboard->isKeyDown(OIS::KC_A))
        mTranslateVector.x = -mMoveScale;

    if(pKeyboard->isKeyDown(OIS::KC_D))
        mTranslateVector.x = mMoveScale;

    if(pKeyboard->isKeyDown(OIS::KC_W))
        mTranslateVector.z = -mMoveScale;

    if(pKeyboard->isKeyDown(OIS::KC_S))
        mTranslateVector.z = mMoveScale;
}//getInput

//-------------------------------------------------------------------
bool OgreInit::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        mShutdown=true;
        return true;
    }

    if(pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        pRenderWnd->writeContentsToTimestampedFile("ScreenShot_",".jpg");
        return true;
    }

    if(pKeyboard->isKeyDown(OIS::KC_M))
    {
        static int mode = 0;
        if(mode==2)
        {
            pCamera->setPolygonMode(PM_SOLID);
            mode=0;
        }
        else if(mode==0)
        {
            pCamera->setPolygonMode(PM_WIREFRAME);
            mode=1;
        }
        else if(mode==1)
        {
            pCamera->setPolygonMode(PM_POINTS);
            mode=2;
        }
    }

    if(pKeyboard->isKeyDown(OIS::KC_0))
       {
           if(pTrayMgr->isLogoVisible())
           {
               pTrayMgr->hideLogo();
               pTrayMgr->hideFrameStats();
           }
           else
           {
               pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
               pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
           }
       }
    return true;
}//keyPressed

//--------------------------------------------------------------------
bool OgreInit::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}//keyReleased

//------------------------------------------------------------------
bool OgreInit::mouseMoved(const OIS::MouseEvent &evt)
{
    pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
    pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));

    return true;
}//mouseMoved

//-------------------------------------------------------------------
bool OgreInit::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}//mousePressed

//-------------------------------------------------------------------
bool OgreInit::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}//mouseReleased


