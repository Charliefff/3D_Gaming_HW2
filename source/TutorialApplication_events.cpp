//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2023/10/05
//
//
// The functions will be invoked at the runtime stage.
//
// Example: 
// Handle key events to control the camera.
// Handle key events to set viewports.
// Handle key events to obtain the camera information.
// Handle key events to activate the objects/items, e.g., spheres, robots, etc
// And others
// 
// A key handler handles simple tasks, 
// such as setting the states of game objects. 
//
//

#include "TutorialApplication.h"
#include "BasicTools.h"

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

bool BasicTutorial_00::mouseMoved(const OIS::MouseEvent& arg)
{
	Ogre::String ss = Ogre::StringConverter::toString(arg.state.Z.rel);

	Ogre::LogManager::getSingletonPtr()->logMessage(ss);

	int d = 120;
	if (arg.state.Z.rel != 0) {

		Vector3 p = mCamera->getPosition();
		Real dy = arg.state.Z.rel / d; // set d to a proper value
		p.y -= dy;
		Real dz = arg.state.Z.rel / d;
		p.z -= dz;
		mCamera->setPosition(p);
	}


	if (mFlgSelectNow == true) {
		Ray mRay = mTrayMgr->getCursorRay(mCamera);

		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		left = scn.x;
		top = scn.y;

		Ogre::String ss;
		ss = Ogre::StringConverter::toString(left);
		Ogre::LogManager::getSingletonPtr()->logMessage("left:" + ss);

		ss = Ogre::StringConverter::toString(top);
		Ogre::LogManager::getSingletonPtr()->logMessage("top:" + ss);

		mSelectionRect->setCorners(left, top, right, bottom);
		mSelectionRect->setVisible(true);
	}
	return BaseApplication::mouseMoved(arg);

}

int BasicTutorial_00::singleClickSelect(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	int numSelectedObj = 0;	// number of selected objects

	Ray mRay = mTrayMgr->getCursorRay(mCamera);

	RaySceneQuery* mRaySceneQuery = mSceneMgr->createRayQuery(Ray());

	//«ö·Ó¶ZÂ÷
	mRaySceneQuery->setSortByDistance(true);

	mRaySceneQuery->setRay(mRay);
	// Perform the scene query
	RaySceneQueryResult& result = mRaySceneQuery->execute();
	RaySceneQueryResult::iterator itr = result.begin();

	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.

	if (itr != result.end()) {

		if (itr->movable->getName() == "sphere" || itr->movable->getName() == "GroundEntity" || itr->movable->getName() == "FloorEnity") {
			return numSelectedObj;
		}
		mCurrentObject = itr->movable ? itr->movable->getParentSceneNode() : 0;
		if (mCurrentObject) {
			bool flgShow = mCurrentObject->getShowBoundingBox();
			mCurrentObject->showBoundingBox(!flgShow);
			numSelectedObj = 1;
		}
	}

	return numSelectedObj;
}

int BasicTutorial_00::volumeSelection(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{

	int numSelectedObj = 0;

	if (right < left) {
		Real tmp = left;
		left = right;
		right = tmp;
	}
	if (bottom < top) {
		Real tmp = bottom;
		bottom = top;
		top = tmp;
	}

	Real nleft = left;
	Real nright = right;
	Real ntop = top + 1;
	Real nbottom = bottom + 1;

	Ray topLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, ntop));		// compute the corner coordinates
	Ray topRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, ntop));
	Ray bottomLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, nbottom));
	Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, nbottom));

	PlaneBoundedVolume vol;
	int np = 100;		// any large enough value is good
	vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         	// front plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), topRight.getPoint(np)));         	// top plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), topLeft.getPoint(np)));       	// left plane
	vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), bottomLeft.getPoint(np)));   	// bottom plane
	vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), bottomRight.getPoint(np)));     	// right plane 

	PlaneBoundedVolumeList volList;
	volList.push_back(vol);
	mVolQuery->setVolumes(volList);

	SceneQueryResult result = mVolQuery->execute();

	SceneQueryResultMovableList::iterator itr = result.movables.begin();
	for (itr = result.movables.begin(); itr != result.movables.end(); ++itr) {
		
		if (*itr) {
			MovableObject* movable = *itr;
			if (movable->getName() == "sphere" || movable->getName() == "GroundEntity" || movable->getName() == "FloorEnity") {
				continue;
			}
			SceneNode* objectNode = movable->getParentSceneNode();
			bool flgShow = objectNode->getShowBoundingBox();
			objectNode->showBoundingBox(!flgShow);
			numSelectedObj++;
		} 
	}

	return numSelectedObj;
}


void BasicTutorial_00::computeTargetPosition()
{
	static bool flg = false;

	mFlgTarget = true;
	Vector3 planeNormal = Vector3(0, 1, 0);
	Ray ray;
	ray = mTrayMgr->getCursorRay(mCamera);
	Plane p(Vector3(0, 1, 0), -30); 
	std::pair<bool, Real> result = ray.intersects(p);
	mTargetPosition = ray.getOrigin() + result.second * ray.getDirection();

	//if (flg) mTargetPosition = Vector3(0, -30, 0);

}


bool BasicTutorial_00::keyPressed(const OIS::KeyEvent& arg)
{
	if (arg.key == OIS::KC_B)
	{
		mEnabledObjectAnimation = !mEnabledObjectAnimation;

	}

	if (arg.key == OIS::KC_V)
	{
		mEnabledLightAnimation = !mEnabledLightAnimation;

	}

	if (arg.key == OIS::KC_SLASH)
	{

		mMiniMap_Dimension == Vector4(0.75, 0.0, 0.25, 0.25) ? mMiniMap_Dimension = Vector4::ZERO : mMiniMap_Dimension = Vector4(0.75, 0.0, 0.25, 0.25);
		mViewport_Map->setDimensions(mMiniMap_Dimension.x, mMiniMap_Dimension.y, mMiniMap_Dimension.w, mMiniMap_Dimension.z);

	}



	if (arg.key == OIS::KC_N) {
		mFlg_EnableParticleSystem = !mFlg_EnableParticleSystem;
		return true;
	}

	if (arg.key == OIS::KC_P) {
		mFlg_Floor_Motion = !mFlg_Floor_Motion;
		return true;
	}

	if (arg.key == OIS::KC_F1) {
		if (mEntity_Floor) mEntity_Floor->setMaterialName("Examples/Rocky");
		return true;
	}


	if (arg.key == OIS::KC_F2) {
		if (mEntity_Floor) {
			MaterialPtr material = MaterialManager::getSingleton().getByName("Examples/Tens");
			material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("10points.png");
			mEntity_Floor->setMaterial(material);
		}
		return true;
	}


	if (arg.key == OIS::KC_F3) {
		if (mEntity_Floor) {
			MaterialPtr material = MaterialManager::getSingleton().getByName("Examples/WaterStream");
			material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("Water02.jpg");
			mEntity_Floor->setMaterial(material);
		}
		return true;
	}

	if (arg.key == OIS::KC_F5) {
		if (mSceneMgr->getFogMode() == Ogre::FOG_NONE) {
			createfog();
		}
		else {
			mSceneMgr->setFog(Ogre::FOG_NONE);
		}
		return true;
	}


	if (arg.key == OIS::KC_1) {

		if (mEntity_Ground) {
			Vector3 position = mEntity_Ground->getParentSceneNode()->getPosition();
			position.y -= 2.0;
			mEntity_Ground->getParentSceneNode()->setPosition(position);
		}
		return true;
	}

	if (arg.key == OIS::KC_2) {
		if (mEntity_Ground) {
			Vector3 position = mEntity_Ground->getParentSceneNode()->getPosition();
			position.y += 2.0;
			mEntity_Ground->getParentSceneNode()->setPosition(position);
		}
		return true;
	}

	if (arg.key == OIS::KC_3) {
		if (mEntity_Ground) {
			bool isVisible = mEntity_Ground->getVisible();
			mEntity_Ground->setVisible(!isVisible);
		}
		return true;
	}

	if (arg.key == OIS::KC_4) {

		if (mMiniMapFactor >= 0.25 && mMiniMapFactor <= 4) {
			mMiniMapFactor -= 0.25;
			mUpdateMiniMapAspectRatio();
		}
		return true;
	}

	if (arg.key == OIS::KC_5) {
		if (mMiniMapFactor >= 0.25 && mMiniMapFactor <= 4) {
			mMiniMapFactor += 0.25;
			mUpdateMiniMapAspectRatio();
		}
		return true;
	}

	if (arg.key == OIS::KC_7) {
		Ogre::ColourValue ambientLight = mSceneMgr->getAmbientLight();
		ambientLight.r -= 0.2;
		ambientLight.g -= 0.2;
		ambientLight.b -= 0.2;
		mSceneMgr->setAmbientLight(ambientLight);
		return true;
	}


	if (arg.key == OIS::KC_8) {
		Ogre::ColourValue ambientLight = mSceneMgr->getAmbientLight();
		ambientLight.r += 0.2;
		ambientLight.g += 0.2;
		ambientLight.b += 0.2;
		mSceneMgr->setAmbientLight(ambientLight);
		return true;
	}


	if (arg.key == OIS::KC_9) {
		Ogre::ColourValue diffuseColor = mLight_Point->getDiffuseColour();
		diffuseColor -= Ogre::ColourValue(0.2, 0.2, 0.2);
		mLight_Point->setDiffuseColour(diffuseColor);
		return true;
	}

	if (arg.key == OIS::KC_0) {
		Ogre::ColourValue diffuseColor = mLight_Point->getDiffuseColour();
		diffuseColor += Ogre::ColourValue(0.2, 0.2, 0.2);
		mLight_Point->setDiffuseColour(diffuseColor);
		return true;
	}

	return BaseApplication::keyPressed(arg);
}

bool BasicTutorial_00::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right) {
		computeTargetPosition();
		mFlgAllObjectsReachedTarget = false;
		return BaseApplication::mouseReleased(arg, id);
	}
	mFlgTarget = false;
	if (mFlgSelectNow == true) {

		for (int i = 0; i < mNumofObjects; ++i) {
			mMotionStateArr[i] = 1;
		}

		if (left == right
			&&
			top == bottom)
		{
			mNumberOfPets = singleClickSelect(arg, id);
		}
		else {
			mNumberOfPets = volumeSelection(arg, id);
		}
		mFlgSelectNow = false;
		mSelectionRect->setVisible(false);
	}

	return BaseApplication::mouseReleased(arg, id);

}

bool BasicTutorial_00::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right) {
		return BaseApplication::mousePressed(arg, id);
	}

	Ogre::String ss = Ogre::StringConverter::toString(id);

	Ogre::LogManager::getSingletonPtr()->logMessage(ss);


	mFlgSelectNow = true;

	Ray mRay = mTrayMgr->getCursorRay(mCamera);

	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	left = scn.x;
	top = scn.y;
	right = scn.x;
	bottom = scn.y;

	mSelectionRect->setCorners(left, top, right, bottom);
	mSelectionRect->setVisible(true);
	return BaseApplication::mousePressed(arg, id);
}
