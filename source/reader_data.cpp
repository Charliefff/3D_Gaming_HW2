//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
#include <fstream>
#include <iostream>
#include <string>
#include "reader_data.h"
#define DATA_FILE_NAME	"game_data.txt"

using namespace std;
std::string READER_DATA::mMeshName_Pet = "robot.mesh";
double READER_DATA::mYawAngleDegreeOffset_Pet = 0;
double READER_DATA::mMeshScale_Pet = 0.8;
bool READER_DATA::mEnabledAutoPlaceObject = true;
std::string READER_DATA::mMaterialName_Floor ="Examples/WaterStream";
std::string READER_DATA::mMaterialName_Wall ="Examples/WaterStream";
bool READER_DATA::mEnabledShadow = true;
double READER_DATA::mAimAtSphereAngleDeviation = 10.0;
double READER_DATA::mWalkSpeed = 0.25;
//
READER_DATA::READER_DATA()
{

}

void READER_DATA::readData()
{
    readData(DATA_FILE_NAME);
}

void READER_DATA::readData(const std::string &fileName)
{
    cout << "Begin READER_DATA::readData()" << endl;
    using namespace std;
    std::ifstream *fp;
    fp = new std::ifstream(DATA_FILE_NAME, ios::in | ios::binary);
    if (fp == 0 || fp->fail()) {
        cout << "Cannot open data file:" << DATA_FILE_NAME << endl;
        return;
    }

    std::string key;
    double value;
    std::string vstring;
    while (!fp->eof()) {
        *fp >> key;

        cout << "Reading:" << key <<endl;

        if (key.compare("MESH_NAME_PET") == 0) {
            *fp >> mMeshName_Pet;
        }

        if (key.compare("YAW_ANGLE_DEGREE_OFFSET_PET") == 0) {
            *fp >> value;
            mYawAngleDegreeOffset_Pet = value;
        }

        if (key.compare("AUTO_PLACE_OBJECT") == 0) {
            *fp >> vstring;

            mEnabledAutoPlaceObject = true;
            if (vstring.compare("false") == 0) {
                mEnabledAutoPlaceObject = false;
            }
        }
        

        if (key.compare("MATERIAL_NAME_FLOOR") == 0) {
            *fp >> mMaterialName_Floor;
        }

        if (key.compare("MATERIAL_NAME_WALL") == 0) {
            *fp >> mMaterialName_Wall;
        }

        if (key.compare("FLAG_ENABLED_SHADOW") == 0) {
            *fp >> vstring;

            mEnabledShadow = true;
            if (vstring.compare("false") == 0) {
                mEnabledShadow = false;
            }
        }

        if (key.compare("AIM_AT_SPHERE_ANGLE_DEVIATION") == 0) {
            *fp >> mAimAtSphereAngleDeviation;
        }

        if (key.compare("WALK_SPEED") == 0) {
            *fp >> mWalkSpeed;
        }

        
        //////////////////////////////////////////////
        key.clear(); // clear the key content
        //////////////////////////////////////////////
    }
    report();
    cout << "End READER_DATA::readData()" << endl;
}

double READER_DATA::getWalkSpeed()
{
    return mWalkSpeed;
}

std::string READER_DATA::getMeshName_Pet() {
    return mMeshName_Pet;
}

     double READER_DATA::getYawAngleDegreeOffset_Pet(){
         return mYawAngleDegreeOffset_Pet;
}

     double READER_DATA::getMeshScale_Pet(){
         return mMeshScale_Pet;
}

     bool READER_DATA::getFlgAutoPlaceObj() {
         return mEnabledAutoPlaceObject;
     }
    //
	 std::string READER_DATA::getMaterialName_Floor(){
         return mMaterialName_Floor;
}

	 std::string READER_DATA::getMaterialName_Wall(){
         return mMaterialName_Wall;
}

     double READER_DATA::getAimAtSphereAngleDeviation( )
     {
         return mAimAtSphereAngleDeviation;
     }

     bool READER_DATA::isEnabledShadow() {
         return mEnabledShadow;
     }
void READER_DATA::report()
{
    cout << "=======================================" << endl;
    cout << "READER_DATA::report" << endl;
    cout << "mMeshName_Pet:\t" <<               mMeshName_Pet << endl;
    cout << "mYawAngleDegreeOffset_Pet:\t" <<   mYawAngleDegreeOffset_Pet << endl;
    cout << "mEnabledAutoPlaceObject:\t" <<     mEnabledAutoPlaceObject << endl;
    cout << "mMaterialName_Floor:\t" <<         mMaterialName_Floor << endl;
    cout << "mMaterialName_Wall:\t" <<          mMaterialName_Wall << endl;
    cout << "mEnabledShadow:\t" <<              mEnabledShadow << endl;
    cout << "mAimAtSphereAngleDeviation:\t" <<  mAimAtSphereAngleDeviation << endl;
    cout << "=======================================" << endl;
}




