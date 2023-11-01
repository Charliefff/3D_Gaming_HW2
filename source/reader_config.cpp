//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
#include <fstream>
#include <iostream>
#include <string>
#include "reader_config.h"

#define CONFIG_FILE_NAME	    "game_config.txt"
#define Default_Data_File_Name  "game_data.txt"


using namespace std;

std::string READER_CONFIG::mDataFileName = Default_Data_File_Name;

READER_CONFIG::READER_CONFIG()
{

}


void READER_CONFIG::readData()
{
    cout << "Begin READER_CONFIG::readData()" << endl;
    using namespace std;
    std::ifstream* fp;
    fp = new std::ifstream(CONFIG_FILE_NAME, ios::in | ios::binary);
    if (fp == 0 || fp->fail()) {
        cout << "Cannot open data file:" << CONFIG_FILE_NAME << endl;
        return;
    }

    std::string key;
    double value;
    std::string vstring;
    while (!fp->eof()) {
        *fp >> key;

        cout << "Reading:" << key << endl;

        if (key.compare("GAME_DATA_FILE") == 0) {
            *fp >> mDataFileName;
        }
        //////////////////////////////////////////////
        key.clear(); // clear the key content
        //////////////////////////////////////////////
    }
    report();
    cout << "End READER_DATA::readData()" << endl;
}

void READER_CONFIG::report()
{
    cout << "Begin READER_CONFIG::report()" << endl;
    cout << "mDataFileName:\t" << mDataFileName << endl;
    cout << "End READER_DATA::report()" << endl;
}


std::string READER_CONFIG::getDataFileName()
{
    return mDataFileName;
}