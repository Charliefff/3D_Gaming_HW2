//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
//header guard
#ifndef __READER_CONFIG_H__
#define __READER_CONFIG_H__

#include <string>

class READER_CONFIG {
protected:

	static std::string mDataFileName;
public:
	READER_CONFIG();
	static void readData();
	static void report();
	static std::string getDataFileName();
};

#endif