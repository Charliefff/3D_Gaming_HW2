//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2023/10/05
//
#include <string>
#include "SpecialEffectManager.h"
#include "BasicTools.h"

//
// particleSystemNames stores the names of the available particle systems.
// particleSystemNames is used to create particle systems in SpecialEffectManager.
//
const std::string particleSystemNames[] = {
	"Examples/GreenyNimbus",
	//"Examples/Rain",
	"Examples/Aureola",
	"Examples/Snow",
	//"Examples/Smoke"
};


//
// Need a valid scene manager 
// and the number of particle systems that will be created.
// 
SpecialEffectManager::SpecialEffectManager(
	SceneManager* sceneMgr,
	int numParticleSystems
)
{
	mSceneMgr = sceneMgr;
	mNumParticleSystems = numParticleSystems;
	mParticleNodes = new SceneNode * [mNumParticleSystems];
	mParticleEmitters = new ParticleEmitter * [mNumParticleSystems];
}

//
// Create a particle system with index. 
// index refers to the entry in the arrays, 
// including mParticleNodes and mParticleEmitters.
// 
// mParticleNodes is an array of scene node pointers.
// mParticleEmitters is an array of the emitters of the particle systems.
//
void SpecialEffectManager::createParticleSystem(int index)
{
	int i = index;
	int numPNames = sizeof(particleSystemNames) / sizeof(std::string);
	int particlesystemTypeIndex = index % numPNames;

	mParticleNodes[i] = static_cast<SceneNode*>(
		mSceneMgr->getRootSceneNode()->createChild());

	String name;
	genNameUsingIndex("particleSystem", i, name);

	ParticleSystem* p = mSceneMgr->createParticleSystem(
		name, particleSystemNames[particlesystemTypeIndex]);

	ParticleEmitter* e = ((ParticleSystem*)p)->getEmitter(0);
	mParticleEmitters[i] = e;
	e->setEnabled(false);
	mParticleNodes[i]->attachObject(p);
	mParticleNodes[i]->setPosition(Vector3::ZERO);
	mParticleNodes[i]->setVisible(false);
}

//
// Initialize all the particle systems.
// mNumParticleSystems is the number of the particle systems.
//
void SpecialEffectManager::init()
{
	for (int i = 0; i < mNumParticleSystems; ++i) {
		createParticleSystem(i);
	}
}

//
// Set off the particle system with index pIndex at position pos.
//
void SpecialEffectManager::setOffParticleSystem(
	int pIndex,
	const Ogre::Vector3& pos)
{
	int index = pIndex;
	SceneNode* fNode = mParticleNodes[index];

	fNode->setPosition(pos);
	fNode->setVisible(true);

	//p->setVisible(true);
	ParticleEmitter* e = mParticleEmitters[index];
	e->setEnabled(true);

}

//
// Purpose: set off a particle system at position pos
// Return: the index of the particle system in the array.
// 
int SpecialEffectManager::setOffParticleSystem(const Ogre::Vector3& pos)
{
	int index = mCurrentIndex;
	SceneNode* fNode = mParticleNodes[mCurrentIndex];

	fNode->setPosition(pos);
	fNode->setVisible(true);

	//p->setVisible(true);
	ParticleEmitter* e = mParticleEmitters[mCurrentIndex];
	//e->setEnabled(true);

	mCurrentIndex = (mCurrentIndex + 1) % mNumParticleSystems;
	return index;
}

//
// Disable all the particle systems.
//
void SpecialEffectManager::disableAllParticleSystems()
{
	for (int i = 0; i < mNumParticleSystems; ++i) {
		mParticleNodes[i]->setVisible(false);
		ParticleEmitter* e = mParticleEmitters[i];
		e->setEnabled(false);
	}
}

//
// Enable all the particle systems.
//
void SpecialEffectManager::enableAllParticleSystems() {
	for (int i = 0; i < mNumParticleSystems; ++i) {
		mParticleNodes[i]->setVisible(true);
		ParticleEmitter* e = mParticleEmitters[i];

		e->setEnabled(true);
	}
}
