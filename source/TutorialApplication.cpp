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
//
// Purpose: Set up objects/items which may be used at the runtime stage.
// 
// Example: 
// set up the scene manager, cameras, viewports, 
// minimap default configuration, data reader, 
// particle system manager, sound manager, etc
// 
//

#include "TutorialApplication.h"
#include "BasicTools.h"
#include "reader_config.h"
#include "reader_data.h"

using namespace Ogre;
using namespace std;

//
// Constructor. 
// Initialize the data members and set up the application.
//
BasicTutorial_00::BasicTutorial_00(void):
mFlg_Floor_Motion(false)
, mEntity_Floor(nullptr)
, mEntity_Ground(nullptr)
, mFloor_USpeed(0.05)
, mFloor_VSpeed(0.0)
, mEnabledObjectAnimation( true )
, mEnabledLightAnimation( true )
{
mSound = new SOUND;
	mSound->init();
    mSound->play();
	//
    mFXManager = 0;
    mFlg_EnableParticleSystem = true;
	//
	mFlgAllObjectsReachedTarget = false;
	//
	mPetCounter = nullptr;
	mNumberOfPets = 0;
	//
	mSphere_Radius = 70;
	mObject_radius = 20.0;
	//
	mNumofObjects = 0;
	mMaxNumofObjects = 128;
	for (int i = 0; i < mMaxNumofObjects; ++i) {
		mAnimationStateArr[i] = nullptr;
	}
	//
	for (int i = 0; i < 128; ++i) {
		mSceneNodeArr[i] = nullptr;
	}
	//
	read_Config_and_DataFile();
}

//
// Read config file.
// Get the data file name from READER_CONFIG.
// Use READER_DATA to read the data file. 
//
void BasicTutorial_00::read_Config_and_DataFile()
{
	READER_CONFIG::readData();
	READER_DATA::readData( READER_CONFIG::getDataFileName() );
}

// Create the main camera for the main scene.
// The main camera looks at the scene. 
// It can see all the objects in the scene.
void BasicTutorial_00::createCamera_Main(void)
{
	mCamera = mSceneMgr->createCamera("PlayerCamera");

	mCamera->setPosition(Ogre::Vector3(0, 700, 600));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));

	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

//
// Create the minimap camera for the minimap of the main scene. 
// The minimap camera takes the birdview of the main scene.
// 
void BasicTutorial_00::createCamera_MiniMap(void)
{
	mCameraMap = mSceneMgr->createCamera("MapCamera");

	mCameraMap->setPosition(Ogre::Vector3(0, 1500, 500));
	mCameraMap->lookAt(Ogre::Vector3(0, 0, 0));

	mCameraMap->setNearClipDistance(5);
	mCamera->setFarClipDistance(1600);

}

//
// Create the main camera for the main scene.
// Create the minimap camera for the minimap of the main scene.
//
void BasicTutorial_00::createCamera(void)
{
	createCamera_Main();
	createCamera_MiniMap();
}

// Create the main viewport which covers the screen. 
// It is associated with the main camera.
// defult z order == 0
void BasicTutorial_00::createViewport_Main()
{
	Ogre::Viewport* vp;
	vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

// Create the minimap viewport which covers a small part of the screen.
// It is associated with the minimap camera.
//
// Disable the overlay.
// Disable the sky.
// Do not show the selection rectangle.
void BasicTutorial_00::createViewport_MiniMap()
{
	mMiniMap_Dimension.x = 0.75;
	mMiniMap_Dimension.y = 0.0;
	mMiniMap_Dimension.w = 0.25;
	mMiniMap_Dimension.z = 0.25;

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(
		mCameraMap, 1,
		mMiniMap_Dimension.x
		, mMiniMap_Dimension.y
		, mMiniMap_Dimension.w
		, mMiniMap_Dimension.z);
	
	vp->setOverlaysEnabled(false);

	vp->setVisibilityMask(VIS_MASK_MINMAP);

	vp->setSkiesEnabled(false);

	
	mMiniMapFactor = 1.0;
	
	vp->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 00));
	mCameraMap->setAspectRatio(mMiniMapFactor*
		(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight())));

	mViewport_Map = vp;
	

}

//
// Create the main viewport which covers the entire screen. 
// It is associated with the main camera.
// 
// Create the minimap viewport which covers a small part of the screen.
// It is associated with the minimap camera.
//
void BasicTutorial_00::createViewports(void)
{
	createViewport_MiniMap( );
	createViewport_Main( );
}

