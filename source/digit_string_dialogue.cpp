//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
#include "digit_string_dialogue.h"

namespace {
inline void genNameUsingIndex(const String & prefix, int index, String &out_name)
{
	out_name= prefix + StringConverter::toString(static_cast<int>(index));
}
};

DIGIT_STRING_DIALOGUE::DIGIT_STRING_DIALOGUE(SceneManager *s)
{
	mSceneMgr = s;
	mMaterialName = "Examples/Digits";
	createObjs(mMaterialName);
}

DIGIT_STRING_DIALOGUE::DIGIT_STRING_DIALOGUE(SceneManager *s, const String &a_Material_Name)
{
	mMaterialName = a_Material_Name;
	mSceneMgr = s;
	createObjs(mMaterialName);
}

void DIGIT_STRING_DIALOGUE::createObjs(const String &a_Material_Name)
{
	int i, j;
	String name;
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 10; i++) {
			int index = i + j*10;			
			genNameUsingIndex("fName", index, name);
			fr[index] = new FilledRectangle(name, mMaterialName, i);
			mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(fr[index]);
			fr[index]->setVisible(false);
		}
	}
	
}

/*
	
*/
void DIGIT_STRING_DIALOGUE::setScore(int score, Real x, Real y, Real dx, Real dy)
{
	
	char msg[64];
	sprintf(msg, "%d", score);
	int len = strlen(msg);
	
	for (int j = 0; j < 50; j++) {
		fr[j]->setVisible(false);
	}
	if (len >=5 ) len = 5;
	for (int i = 0; i < len; i++, x += dx) {
		int index = i*10 + msg[i] -'0';
		fr[index]->setCorners(Vector2(x, y), Vector2(x+dx, y+dy));
		fr[index]->setVisible(true);
	}
}