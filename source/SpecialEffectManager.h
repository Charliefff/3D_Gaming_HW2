
//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2023/10/05
// 
// Purpose: 
// Use SpecialEffectManager to create N particle systems, where N is a given number. 
// The particle systems are stored in an array.
// 
// The particle systems are cycled when they are used.
// That is, when setOffParticleSystem( ) is invoked, 
// SpecialEffectManager uses the next particle system 
// in the array and then sets it off, 
// no matter whether the particle is being used or not.
//
// How to use SpecialEffectManager?
// Simple Method:
// Step 1. Create or instantiate an object, 
//         namely mSpecialEffectMgr , of SpecialEffectManager.
// Step 2. invoke init( ) for mSpecialEffectMgr
// Step 3. invoke setOffParticleSystem( ) to set off a particle system
//

#ifndef __ParticleSystemManager_h_
#define __ParticleSystemManager_h_
 
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
using namespace Ogre;

class SpecialEffectManager {
protected:
    SceneManager *mSceneMgr;
    int mNumParticleSystems;
    SceneNode **mParticleNodes;
    ParticleEmitter **mParticleEmitters;
    int mCurrentIndex;
    void createParticleSystem(int index);
public:
    SpecialEffectManager(
        SceneManager *sceneMgr,
        int numParticleSystems);

    void init();
    int getNumParticleSystems() const { return mNumParticleSystems; }
    int setOffParticleSystem(const Ogre::Vector3 &pos);
    void setOffParticleSystem(int pIndex, const Ogre::Vector3 &pos);
    void disableAllParticleSystems();
    void enableAllParticleSystems();
};

#endif
