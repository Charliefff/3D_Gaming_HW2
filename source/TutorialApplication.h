//
// Student Name: ³¯¿A©ý 
// Student ID: 311356003
// Student Email Address: charlie88531@gmail.com
//
//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2023/10/05
// 

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"
#include "selection_rectangle.h"
#include "sound.h"
#include "SpecialEffectManager.h"
#include "digit_string_dialogue.h"

using namespace Ogre;

#define VIS_MASK_MINMAP 0x0001

#define MOTION_TYPE_NONE 0
#define MOTION_TYPE_IDLE 1
#define MOTION_TYPE_WALK 2

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);

private:
	PlaneBoundedVolumeListSceneQuery* mVolQuery;
	Real left, top, right, bottom;
	SelectionRectangle* mSelectionRect;
	//
	int mNumOfMovingObj;
	int mNumOfObjectsReachedTarget;
	//
	int mNumofObjects;
	int mMaxNumofObjects;
	SceneNode* mCurrentObject;
	SceneNode* mSceneNodeArr[128];
	Vector3 mObjectDisplacement[128];
	SceneNode* mParticleNode;
	Entity* mEntityArr[128];
	bool mFlgTarget;
	Vector3 mTargetPosition;
	bool mFlgSelectNow;
	AnimationState* mAnimationStateArr[128];
	int mMotionStateArr[128];
	
	SceneNode* mLightSceneNode;
	

	Viewport* mViewport_Map;
	Vector4 mMiniMap_Dimension;
	Camera* mCameraMap;
	SOUND* mSound;
	//bool mFlgAllReached;
	bool mFlgAllObjectsReachedTarget;
	Real mSphere_Radius;
	Vector3 mSphere_Position;
	Real mObject_radius;
	SceneNode* mSphere_Node;
	float mMiniMapFactor ; 
	SpecialEffectManager* mFXManager;

	bool mFlg_EnableParticleSystem;
	Entity* mEntity_Floor;
	Entity* mEntity_Ground;
	SceneNode* mSceneNode_Ground;
	bool mFlg_Floor_Motion;
	Real mFloor_USpeed, mFloor_VSpeed;
	Light* mLight_Point;

	int mNumberOfPets;
	DIGIT_STRING_DIALOGUE* mPetCounter;
	DIGIT_STRING_DIALOGUE* mMovingObjectsCounter;

	bool mEnabledObjectAnimation;
	bool mEnabledLightAnimation;

protected:
	void read_Config_and_DataFile();
	void computeTargetPosition();
	void initParticleSystemForExplosion();
	void setOffParticleSystem(
		Ogre::SceneNode* fNode,
		const Ogre::String& pname,
		const Ogre::Vector3& pos);

	virtual void createPlaneObjectResource();
	virtual void createGround();
	virtual void createFloor();
	virtual void createLights();
	virtual void createScene_Setup();
	virtual void createScene_Setup_FX();
	virtual void createObjects();
	virtual void createSphere();
	virtual void createScene(void);
	//virtual void createScore();
	/*virtual void robotlookAT();*/
	
	virtual void createSky();
	virtual void createfog();

	virtual bool keyPressed(const OIS::KeyEvent& arg);
	virtual void createCamera_Main();
	virtual void createCamera_MiniMap();
	virtual void createCamera();

	virtual void createViewport_Main();
	virtual void createViewport_MiniMap();
	virtual void createViewports();

	int volumeSelection(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	int singleClickSelect(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseMoved(const OIS::MouseEvent& arg);
	virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool frameStarted(const FrameEvent& evt);
	//
	void resolveCollisionObjectPair(int objA, int objB, float rA, float rB, Real dt);

	void resolveCollisionBetweenObjectsAndSphere( Real dt );

	void resolveCollisionForObjects(Real dt);
	//
	void mUpdateMiniMapAspectRatio();
	void updateObjects_Positions(Real dt);
	void updateObjects_Animation(Real dt);
	void checkObjects_ReachingTarget(Real dt);
	//
	//void updateScore();
	void updateObjectInformation();
	void updateLight(Real dt);
	void updateParticleSystems();
	void performCollisionHandling(Real dt);
	void updateObjects(Real dt);
	//
	/*
	bool handleKeyEvents_GetCameraInformation(const OIS::KeyEvent& arg);
	bool handleKeyEvents_Camera(const OIS::KeyEvent& arg);
	bool handleKeyEvents_Viewport(const OIS::KeyEvent& arg);
	bool handleKeyEvents_Animation(const OIS::KeyEvent& arg);
	*/
protected:
	
};

#endif // #ifndef __BasicTutorial_00_h_