//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2023/10/05
//
//
// Purpose: Set up the scenes. 
//
//
//
#include "TutorialApplication.h"
#include "BasicTools.h"
#include "SystemParameter.h"
#include "reader_data.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>

using namespace Ogre;


void BasicTutorial_00::createScene_Setup_FX(void) {
	initParticleSystemForExplosion();
	Vector3 pos;
	pos = mSceneNodeArr[0]->getPosition();
	setOffParticleSystem(mParticleNode, "explosion", pos);


	mFXManager = new SpecialEffectManager(
		mSceneMgr,
		SystemParameter::maxNumParticleSystems);
	mFXManager->init();

	for (int i = 0;
		i < mFXManager->getNumParticleSystems();
		++i) 
	{
		if (i >= mNumofObjects) break;
		mFXManager->setOffParticleSystem(
			mSceneNodeArr[i]->getPosition()
		);
		if (i == 0) break;
	}
}

//
void BasicTutorial_00::createScene_Setup(void)
{
	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);

	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	mSelectionRect = new SelectionRectangle("selectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	mSelectionRect->setLightMask(0);
	mSelectionRect->setCastShadows(false);

	mPetCounter = new DIGIT_STRING_DIALOGUE(mSceneMgr);

	mFlgSelectNow = false;
}
void BasicTutorial_00::createfog() {

	Ogre::ColourValue fogColor(0.9, 0.9, 0.9, 0.2);
	Ogre::Real linearStart = 1400;
	Ogre::Real linearEnd = 1600;

	mWindow->getViewport(0)->setBackgroundColour(fogColor);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fogColor, 0.0, linearStart, linearEnd);
	mCamera->setFarClipDistance(linearEnd + 1000);
}

void BasicTutorial_00::createSky() {
	//Ogre::Real size = 1000.0;
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
}

//void BasicTutorial_00::createScore() {
//	mMovingObjectsCounter = new DIGIT_STRING_DIALOGUE(mSceneMgr);
//
//}

void BasicTutorial_00::createLights()
{	

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	Light* light = mSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setPosition(Vector3(150, 300, 150));
	light->setDiffuseColour(1.0, 1.0, 1.0);		//漫反射顏色為白色
	light->setSpecularColour(1.0, 1.0, 1.0);	

	mLight_Point = light;
	mLightSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLightSceneNode->attachObject(light);
}



void BasicTutorial_00::createPlaneObjectResource()
{
	
	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"ground", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		600, 600, 	// width, height
		2, 2, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5, 5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
	);

}

void BasicTutorial_00::createGround()
{
	Entity* ent = mSceneMgr->createEntity(
		"GroundEntity", "ground");;
	//ent->setQueryFlags(0x0);

	ent->setMaterialName("Examples/BeachStones");

	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
		Vector3(0, -30, 0)
		)
		->attachObject(ent);

	mEntity_Ground = ent;
	
}

void BasicTutorial_00::createFloor()
{
	Entity* ent = mSceneMgr->createEntity(
		"FloorEnity", "ground"
	);

	ent->setMaterialName("Examples/SphereMappedRustySteel");

	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
			Vector3(0, 0, 0)
		)
		->attachObject(ent);

	mEntity_Floor = ent;
}



void BasicTutorial_00::createObjects()
{
	//Ogre::LogManager::getSingletonPtr()->logMessage("*** createObjects( ) ***");
	int i;
	std::string petMeshName = READER_DATA::getMeshName_Pet();
	float scale = READER_DATA::getMeshScale_Pet();
	float yaw_angle_offset = READER_DATA::getYawAngleDegreeOffset_Pet();
	Real x, y, z, r = 250;
	double angle, Pi, fx;
	int numRobots = 50;

	Pi = 3.14;

	for (int k = 0; k < numRobots; ++k, ++mNumofObjects)
	{

		i = k;
		String name;
		genNameUsingIndex("r", i, name);

		mEntityArr[i]
			= mSceneMgr
			->createEntity(name, petMeshName);



		mEntityArr[i]->setVisibilityFlags(VIS_MASK_MINMAP);


		mAnimationStateArr[i] = mEntityArr[i]->getAnimationState("Idle");
		mMotionStateArr[i] = MOTION_TYPE_IDLE;

		mAnimationStateArr[i]->setEnabled(true);
		mAnimationStateArr[i]->setLoop(true);

		mSceneNodeArr[i]
			= mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode();

		mSceneNodeArr[i]->attachObject(mEntityArr[i]);



		fx = i / (double)(numRobots);
		angle = fx * Pi * 2;

		x = r * cos(angle);
		y = -30;
		z = r * sin(angle);
		mSceneNodeArr[i]->setPosition(x, y, z);

		mSceneNodeArr[i]->lookAt(Vector3(0, mSceneNodeArr[i]->getPosition().y, 0), Node::TS_WORLD);
		mSceneNodeArr[i]->yaw(Degree(90));
		if (i == 0) {
			mSceneNodeArr[i]->setScale(2, 2, 2);
		}

		else mSceneNodeArr[i]->setScale(scale, scale, scale);
	}
}



void BasicTutorial_00::createSphere()
{
	mSphere_Radius = 70.0;

	mSphere_Position = Vector3(0.0, 0.0, 0.0);
	mSphere_Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	Entity* sphere_ent
		= mSceneMgr
		->createEntity("sphere", "sphere.mesh");
	mSphere_Node->attachObject(sphere_ent);
	auto bb = sphere_ent->getBoundingBox();
	Real sizeX = bb.getMaximum().x - bb.getMinimum().x;
	Real sf = 2.0 * mSphere_Radius / sizeX;

	mSphere_Node->setScale(sf, sf, sf);
}

void BasicTutorial_00::createScene(void)
{
	createScene_Setup();
	createfog();
	createSky();
	createLights( );
	createPlaneObjectResource();
	createGround();
	createFloor();
	createObjects();
	createSphere();
	createScene_Setup_FX();
}