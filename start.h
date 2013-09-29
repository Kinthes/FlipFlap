#ifndef __start_h
#define __start_h

#include "OgreInit.h"
#include "BCube.h"
#include "BSphere.h"

class StartDemo : public OIS::KeyListener, public OIS::MouseListener
{
  public:
        StartDemo();
        ~StartDemo();

         bool startTry(void);
         bool runDemo(void);
         void setupScene(void);

        bool keyPressed(const OIS::KeyEvent &keyEventRef);
        bool keyReleased(const OIS::KeyEvent &keyEventRef);
         bool mouseMoved(const OIS::MouseEvent &evt);
         bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
         bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  private:
        bool                        mDemoShutdown;
        Ogre::SceneNode             *pObjectNode;
        Ogre::Entity                *pObjectEntity;

        enum CelShaderParam {SP_SHININESS=1, SP_DIFFUSE, SP_SPECULAR};
        std::vector<BCube*> m_cubelist;
        std::vector<BSphere*> m_spherelist;

        BCube *ground, *ground2;
        Ogre::Degree angX,angZ, angX2, angZ2;
};


#endif
