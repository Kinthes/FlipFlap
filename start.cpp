#include "start.h"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>

StartDemo::StartDemo()
{
    mDemoShutdown = false;
    pObjectNode =0;
    pObjectEntity =0;
    angX=0;
    angZ=0;
    angX2=0;
    angZ2=0;
}

StartDemo::~StartDemo()
{
    for(int i=0; i<m_cubelist.size(); i++)
        delete m_cubelist[i];
    m_cubelist.clear();
    for(int i=0; i<m_spherelist.size(); i++)
        delete m_spherelist[i];
    m_spherelist.clear();
  //  delete ground;
  //  delete ground2;
    delete OgreInit::getSingletonPtr();
 }
//--------------------------------------------------------------------------------------

bool StartDemo::startTry()
{
    new OgreInit();
    if(!OgreInit::getSingletonPtr()->startOgre("First Shot", this, 0)) return false;

    mDemoShutdown=false;

    OgreInit::getSingletonPtr()->pLog->logMessage("Demo Initialized");

    setupScene();
    return runDemo();
}

//--------------------------------------------------------------------------------------------------
void StartDemo::setupScene()
{
    OgreInit::getSingletonPtr()->pMouse->setEventCallback(this);
    OgreInit::getSingletonPtr()->pKeyboard->setEventCallback(this);

    srand(time(NULL));
    OgreInit::getSingletonPtr()->pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Ogre::Light *pointLight = OgreInit::getSingletonPtr()->pSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(-10,150,80));
    pointLight->setDiffuseColour(1,0.8,0.7);
    pointLight->setSpecularColour(0.5,0.5,0.5);

    pointLight = OgreInit::getSingletonPtr()->pSceneMgr->createLight("pointLight2");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(50,180,10));
    pointLight->setDiffuseColour(0.9,0.8,0.9);
    pointLight->setSpecularColour(0.5,0.5,0.5);

    pointLight = OgreInit::getSingletonPtr()->pSceneMgr->createLight("pointLight3");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(-30,-150,80));
    pointLight->setDiffuseColour(0.8,0.0,0.0);
    pointLight->setSpecularColour(0.5,0.5,0.5);

    OgreInit::getSingletonPtr()->pCamera->setPosition(Ogre::Vector3(0,100,180));
    OgreInit::getSingletonPtr()->pCamera->lookAt(Ogre::Vector3(0,0,0));

    ground = new BCube("ground", OgreInit::getSingletonPtr()->m_world, OgreInit::getSingletonPtr()->pSceneMgr, btVector3(100,1,50), 0);
    ground->getRigidBody()->setCollisionFlags( ground->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    ground->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
    angX = Ogre::Degree(0.0);
    angZ = Ogre::Degree(0.10);
    btTransform mg;
    mg.setRotation(btQuaternion(angX.valueDegrees(),0,angZ.valueDegrees(),1));
    mg.setOrigin(btVector3(0,-70,0));
    ground->getMyMotionState()->setKinematicPos(mg);
    ground->getEntity()->setMaterialName("Ecs/Ground1");
    m_cubelist.push_back(ground);

    ground2 = new BCube("ground2", OgreInit::getSingletonPtr()->m_world, OgreInit::getSingletonPtr()->pSceneMgr, btVector3(70,1,40), 0);
    ground2->getEntity()->setMaterialName("Ecs/Ground2");
    ground2->getRigidBody()->setCollisionFlags( ground2->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    ground2->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
    angX2 = -Ogre::Degree(0.0);
    angZ2 = -Ogre::Degree(0.15);
    btTransform mg2;
    mg2.setRotation(btQuaternion(angX2.valueDegrees(),0,angZ2.valueDegrees(),1));
    mg2.setOrigin(btVector3(0,40,0));
    ground2->getMyMotionState()->setKinematicPos(mg2);
    m_cubelist.push_back(ground2);

pObjectNode=ground->getSceneNode();

 /*   for(int i=0; i<5; i++)
    {
            BCube *cube = new BCube("cube"+Ogre::StringConverter::toString(i), OgreInit::getSingletonPtr()->m_world, OgreInit::getSingletonPtr()->pSceneMgr, btVector3(2,2,2), 300);
            cube->getRigidBody()->translate(btVector3(rand()%20-10, rand()%100+120, rand()%20-10));
            cube->getEntity()->setMaterialName("Examples/SphereMappedRustySteel");
            m_cubelist.push_back(cube);
    }*/

            BSphere *boule = new BSphere("boule1", OgreInit::getSingletonPtr()->m_world, OgreInit::getSingletonPtr()->pSceneMgr, btScalar(2), 150);
            boule->getRigidBody()->translate(btVector3(0, 80, 0));
            boule->getEntity()->setMaterialName("Ecs/Baboule");
            m_spherelist.push_back(boule);

            boule =new BSphere("boule2", OgreInit::getSingletonPtr()->m_world, OgreInit::getSingletonPtr()->pSceneMgr, btScalar(4), 150);
            boule->getRigidBody()->translate(btVector3(0, 20, 0));
            boule->getEntity()->setMaterialName("Ecs/Baboule");
            m_spherelist.push_back(boule);


 //   pObjectEntity = OgreInit::getSingletonPtr()->pSceneMgr->createEntity("head","ogrehead.mesh");
 //   pObjectNode = OgreInit::getSingletonPtr()->pSceneMgr->getRootSceneNode()->createChildSceneNode("headNode");
 //   pObjectNode->attachObject(pObjectEntity);


}//setupDemoScene


//-------------------------------------------------------------------------------------------------
bool StartDemo::runDemo()
{
    OgreInit::getSingletonPtr()->pLog->logMessage("Start main loop..");

    double timeSinceLastFrame = 0;
    double startTime = 0;
    OgreInit::getSingletonPtr()->pRenderWnd->resetStatistics();

    while(!mDemoShutdown && !OgreInit::getSingletonPtr()->isShutdown())
    {
        if(OgreInit::getSingletonPtr()->pRenderWnd->isActive())
        {
            startTime=OgreInit::getSingletonPtr()->pTimer->getMillisecondsCPU();
            OgreInit::getSingletonPtr()->pKeyboard->capture();
            OgreInit::getSingletonPtr()->pMouse->capture();

       //     pObjectNode->yaw(Ogre::Degree(timeSinceLastFrame * 0.028f));
       //     pObjectNode->pitch(Ogre::Degree(timeSinceLastFrame * 0.012f));



            OgreInit::getSingletonPtr()->updateOgre(timeSinceLastFrame);
            OgreInit::getSingletonPtr()->pRoot->renderOneFrame();

            timeSinceLastFrame= OgreInit::getSingletonPtr()->pTimer->getMillisecondsCPU() - startTime;
        }

    }

    OgreInit::getSingletonPtr()->pLog->logMessage("Quit Main Loop.");
    OgreInit::getSingletonPtr()->pLog->logMessage("Shuting Down Ogre");
    return true;
}//runDemo



//----------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        StartDemo essai;

        try {

            essai.startTry();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif


bool StartDemo::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	OgreInit::getSingletonPtr()->keyPressed(keyEventRef);

	if(OgreInit::getSingletonPtr()->pKeyboard->isKeyDown(OIS::KC_F))
	{

	}

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool StartDemo::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreInit::getSingletonPtr()->keyReleased(keyEventRef);

	return true;
}

bool StartDemo::mouseMoved(const OIS::MouseEvent &evt)
{
    Ogre::Degree varZ = -Ogre::Degree(evt.state.X.rel * -0.0001f);
    Ogre::Degree varX = Ogre::Degree(evt.state.Y.rel * -0.0001f);
    angX += varX;
    angZ += varZ;
    btTransform mg;
    mg.setRotation(btQuaternion(angX.valueDegrees(),0,angZ.valueDegrees(),1));
    mg.setOrigin(btVector3(0,-70,0));
    ground->getMyMotionState()->setKinematicPos(mg);


    angX2 -= varX;
    angZ2 -= varZ;
    mg.setRotation(btQuaternion(angX2.valueDegrees(),0,angZ2.valueDegrees(),1));
    mg.setOrigin(btVector3(0,40,0));
    ground2->getMyMotionState()->setKinematicPos(mg);

    return true;
}

bool StartDemo::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

    return true;
}
bool StartDemo::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

    return true;
}

