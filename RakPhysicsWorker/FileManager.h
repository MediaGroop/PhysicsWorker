#pragma once
#include "stdafx.h"
#include <string>

using namespace std;

class FileManager{
private:
	FileManager(){};
	~FileManager(){};
public:
	static FileManager& getInstance(){
		static FileManager mgr;
		return mgr;
	}
	static bool fileExists(std::string);
	static bool DirExists(const char *);
	static void createFile(std::string);
};