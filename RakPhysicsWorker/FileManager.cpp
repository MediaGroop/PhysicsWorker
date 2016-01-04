#include "FileManager.h"
#include "easylogging++.h"
#include "stdafx.h"
#if defined(_WIN64) && defined(_WIN32)
#include <io.h>
#endif
#include <string>

using namespace std;

bool FileManager::DirExists(const char * fname){
    struct stat sb;
    if (stat(fname, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }
    return false;
}

bool FileManager::fileExists(std::string path)
{
	return access(path.c_str(), 0) != -1;
}

void FileManager::createFile(std::string fname)
{
	ofstream ofs(fname);
	ofs.close();
}