//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2023/10/05
//
//
// Purpose: Update the game state, 
// including object states, game progress, physics simulation, 
// collision detection, collision response, and etc.
//
//
// The functions will be invoked at the runtime stage.
//

#include "TutorialApplication.h"
#include "BasicTools.h"
#include "reader_data.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>

//
// using namespace Ogre;
// Enable to use the functions/structures defined in the namespace Ogre.
//
using namespace Ogre;


//
// For an object pair, push the objects away from each other if they collide.
// objA, objB: object indices
// rA, rB: object radiuses
//
void BasicTutorial_00::resolveCollisionObjectPair(int objA, int objB, float rA, float rB, Real dt)
{
	Vector3 posA = mSceneNodeArr[objA]->getPosition();
	Vector3 posB = mSceneNodeArr[objB]->getPosition();
	float distance = posA.distance(posB);
	float overlap = rA + rB - distance;
	float f;

	if (overlap > 0) {
		Vector3 direction = posA - posB;
		float length = direction.length();
		float f = 0.3;
		Vector3 v = direction / length;
		
		if (mMotionStateArr[objA] == 1 && mMotionStateArr[objB] == 1) {
			f = 0.45;
		}
		else if (mMotionStateArr[objA] == 1) {
			f = 0.9;
		}
		else if (mMotionStateArr[objB] == 1) {
			f = 0.1;
		}
		else {
			f = 0.2;
		}


		Vector3 movement = (overlap * v * f) * dt;
		Vector3 movement1 = (overlap * v * (1-f)) * dt;
		mSceneNodeArr[objA]->translate(movement);
		mSceneNodeArr[objB]->translate(-movement1);
	}
}


//
// Detect and resolve collisions of objects.
//
void BasicTutorial_00::resolveCollisionForObjects(Real dt)
{
	float ri = 25; // object radius
	float rj = 25; // object radius
	for (int i = 0; i < mNumofObjects; ++i)
	{
		for (int j = i + 1; j < mNumofObjects; ++j) {
			resolveCollisionObjectPair(i, j, ri, rj, dt);
		}
	}
}

void BasicTutorial_00::resolveCollisionBetweenObjectsAndSphere(Real dt)
{
	//加入f 一個不動
	Vector3 Sphere_Position = Vector3(0, -30, 0);
	Real Radius = 110;

	for (int i = 0; i < mNumofObjects; ++i) {
		Vector3 objectPosition = mSceneNodeArr[i]->getPosition();
		float rA = 20.0;

		float distance = objectPosition.distance(Sphere_Position);

		float overlap = rA + Radius - distance;

		if (overlap > 0) {
			Vector3 direction = objectPosition - Sphere_Position;
			float length = direction.length();
			Vector3 v = direction / length;
			Vector3 movement = (overlap * v * dt);

			mSceneNodeArr[i]->translate(movement);

		}

	}
}

//
// Perform collision handling.
//
// Step 1. Resolve collision between objects.
// Step 2. Resolve collision between objects and the large sphere.
//
void BasicTutorial_00::performCollisionHandling(Real dt)
{
	for (int i = 0; i < 10; ++i) {
		resolveCollisionForObjects(dt);
	}
	resolveCollisionBetweenObjectsAndSphere(dt);
}

//
// This function is correct.
// Create scene node, mParticleNode.
// Create one particle system.
// Attach the particle system to scene node, mParticleNode.
//
void BasicTutorial_00::initParticleSystemForExplosion() {
	mParticleNode = static_cast<SceneNode*>(
		mSceneMgr->getRootSceneNode()->createChild());

	// Create particle system with name "explosion".
	ParticleSystem* p = mSceneMgr->createParticleSystem(
		"explosion"
		, "Examples/GreenyNimbus");

	ParticleEmitter* e = p->getEmitter(0);
	e->setEnabled(false);
	mParticleNode->attachObject(p);
	mParticleNode->setPosition(Vector3::ZERO);
	mParticleNode->setVisible(false);
}

//
// This function is correct.
// Enable the particle system which is attached to 
// scene node fNode.
// pname is the particle system name.
//
void BasicTutorial_00::setOffParticleSystem(
	Ogre::SceneNode* fNode
	, const Ogre::String& pname
	, const Ogre::Vector3& pos)
{
	fNode->setPosition(pos);
	fNode->setVisible(true);

	//Get the movable object with name pname.
	MovableObject* s = fNode->getAttachedObject(pname);

	//
	//Convert the movable object, s, to a particle system.
	//Note that: in initParticleSystemForExplosion(...), 
	//a particle system is attached to the scene node fNode.
	//Thus, the conversion static_cast<ParticleSystem*>(s) makes sense.
	// 
	ParticleSystem* p = static_cast<ParticleSystem*>(s);
	if (p == NULL) return;
	p->setVisible(true);					// set it to be visible
	ParticleEmitter* e = p->getEmitter(0);	// get the emitter with index 0
	e->setEnabled(true);					// enable the emitter
}

Ogre::Quaternion slerp(
	const Ogre::Quaternion& v0 	// current orientation 
	, const Ogre::Quaternion& v1	// desired orientation
	, Ogre::Real t)
{
	//v0.normalise();
	//v1.normalise();

	Ogre::Quaternion u0 = v0;
	Ogre::Quaternion u1 = v1;

	Ogre::Real dot = u0.Dot(u1);
	if (dot < 0.0f) {
		u1 = -u1;
		dot = -dot;
	}
	const Ogre::Real threshold = 0.9;
	if (dot > threshold) {
		Ogre::Quaternion result = u0 + t * (u1 - u0);
		result.normalise();
		return result;
	}

	Ogre::Real theta_0 = acos(dot);
	Ogre::Real theta = theta_0 * t;
	Ogre::Real sin_theta = sin(theta);
	double sin_theta_0 = sin(theta_0);
	double s0 = cos(theta) - dot * sin_theta / sin_theta_0;
	Ogre::Real s1 = sin_theta / sin_theta_0;
	return (s0 * u0) + (s1 * u1);
}


void BasicTutorial_00::updateObjects_Positions(Real dt)
{
	if (mFlgTarget == false) return;

	for (int i = 0; i < mNumofObjects; ++i)
	{
		bool flgShow = mSceneNodeArr[i]->getShowBoundingBox();
		if (flgShow == false) continue;
		
		++mNumOfMovingObj;
		Vector3 d;
		d = mTargetPosition - mSceneNodeArr[i]->getPosition();


		Real len = d.length();
		Real robotTargetDistance = len;
		if (len != 0.0) d.normalise();

		Real walkSpeed = READER_DATA::getWalkSpeed();
		Real walkDistance = walkSpeed * dt;

		if (robotTargetDistance < 1) {
			mMotionStateArr[i] = 1;
			d = robotTargetDistance * 1.05 * d;
			mSceneNodeArr[i]->showBoundingBox(false);
		}
		else {
			mMotionStateArr[i] = 2;
			d = d * walkDistance;
			Vector3 robotLookAtPosition = mTargetPosition;
			robotLookAtPosition.y = mSceneNodeArr[i]->getPosition().y;

			Quaternion q0 = mSceneNodeArr[i]->getOrientation();
			mSceneNodeArr[i]->lookAt(robotLookAtPosition, Node::TS_WORLD);
			mSceneNodeArr[i]->yaw(Degree(90));

			Quaternion q1 = mSceneNodeArr[i]->getOrientation();
			Real t = 0.9;
			mSceneNodeArr[i]->setOrientation(slerp(q0, q1, t * dt));
			

		}

		mObjectDisplacement[i] = d;
		mSceneNodeArr[i]->translate(d);

	}

}

void BasicTutorial_00::mUpdateMiniMapAspectRatio() {
	mCameraMap->setAspectRatio(mMiniMapFactor *
		(Ogre::Real(mViewport_Map->getActualWidth()) / Ogre::Real(mViewport_Map->getActualHeight())));

}

void BasicTutorial_00::updateObjects_Animation(Real dt)
{
	Real walkSpeed = READER_DATA::getWalkSpeed();
	if (!mEnabledObjectAnimation) return;
	for (int i = 0; i < mNumofObjects; ++i) {


		bool flgShow = mSceneNodeArr[i]->getShowBoundingBox();

		if ( mMotionStateArr[i] == 1){
			if (mAnimationStateArr[i]) {
				mAnimationStateArr[i]->setEnabled(false);
				mAnimationStateArr[i]->setLoop(false);
			}
			mAnimationStateArr[i] = mEntityArr[i]->getAnimationState("Idle");
			mAnimationStateArr[i]->setEnabled(true);
			mAnimationStateArr[i]->setLoop(true);

		}
		else {

			if (mAnimationStateArr[i]) {
				mAnimationStateArr[i]->setEnabled(false);
				mAnimationStateArr[i]->setLoop(false);
			}
			//被選取是走路
			mAnimationStateArr[i] = mEntityArr[i]->getAnimationState("Walk");
			mAnimationStateArr[i]->setEnabled(true);
			mAnimationStateArr[i]->setLoop(true);
		}
	}

	for (int i = 0; i < mNumofObjects; ++i) {
		mAnimationStateArr[i]->addTime(dt * 0.9);
	}
}

void BasicTutorial_00::checkObjects_ReachingTarget(Real dt)
{
	
	for (int i = 0; i < mNumofObjects; ++i) {
		if (mSceneNodeArr[i]->getShowBoundingBox()) {

			Vector3 position = mSceneNodeArr[i]->getPosition();
			Real distanceToDestination = position.distance(mTargetPosition);

			if (distanceToDestination < 0.01) {
				mSceneNodeArr[i]->showBoundingBox(false);
			}

		}
	}

}

//
// Update the objects.
//
void BasicTutorial_00::updateObjects(Real dt)
{
	updateObjects_Positions(dt);

	checkObjects_ReachingTarget(dt);

	updateObjects_Animation(dt);
}


//
// Update the particle systems
//
void BasicTutorial_00::updateParticleSystems()
{
	Vector3 pos;
	pos = mSceneNodeArr[0]->getPosition();
	setOffParticleSystem(mParticleNode, "explosion", pos);

	if (mFlg_EnableParticleSystem) {
		for (int i = 0; i < mNumofObjects; ++i)
		{
			mFXManager->setOffParticleSystem(
				i,
				mSceneNodeArr[i]->getPosition()
			);
		}
	}

	else {
		mFXManager->disableAllParticleSystems();
	}
}

//
// change the light position
//
void BasicTutorial_00::updateLight(Real dt) {
	if (!mEnabledLightAnimation) return;

	Real rotationSpeed = 0.6;
	Real rotationRadius = 300;

	Vector3 currentPosition = mLight_Point->getPosition();

	Real angleChange = rotationSpeed * dt;
	Real currentAngle = atan2(currentPosition.x, currentPosition.z);
	Real newAngle = currentAngle + angleChange;

	Real x = rotationRadius * sin(newAngle);
	Real z = rotationRadius * cos(newAngle);

	mLight_Point->setPosition(x, 300, z);
}

//
// Show the information about the scene
// Example:
// The number of objects, selected objects, etc.
//
void BasicTutorial_00::updateObjectInformation() {

		int numChoose = 0;
	for (int i = 0; i < mNumofObjects; ++i) {
		if (mSceneNodeArr[i]->getShowBoundingBox()) {
			numChoose += 1;
		}
	}

	mPetCounter->setScore(numChoose, 0.05, 0.05);

}

//
// frameStarted(...) is the main function 
// which updates the game logic/animation/life points/energy etc.
//
bool BasicTutorial_00::frameStarted(const FrameEvent& evt)
{
	Real dt = evt.timeSinceLastFrame;

	updateObjects(dt);
	performCollisionHandling(dt);
	updateParticleSystems();
	updateLight(dt);
	updateObjectInformation();

	return BaseApplication::frameStarted(evt);
}

