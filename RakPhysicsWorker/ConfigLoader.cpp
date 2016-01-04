#include "ConfigLoader.h"
#include "minIni.h"
#include "FileManager.h"
#include "easylogging++.h"

ConfigLoader& ConfigLoader::getInstance()
{
	static ConfigLoader l;
	return l;
}

//Creates default config at given local path
void ConfigLoader::createDefault(std::string path){
	static char str[64];
	getcwd(str, 64);
	strcat(str, "\\");
	strcat(str, path.c_str());
	FileManager::createFile(str);

	minIni ini(path);

	ini.put("Network", "Port", "66635");
	ini.put("Network", "MaxCons", "1");

}

//Returns value from config
std::string ConfigLoader::getVal(std::string key)
{
	//LOG(INFO) << "iterating through map, size";
	for (map<string, string>::iterator ii = ConfigLoader::getInstance()._vals.begin(); ii != ConfigLoader::getInstance()._vals.end(); ++ii)
	{
		//LOG(INFO) << key + " " + (*ii).first;
		if ((*ii).first == key) {
			return (*ii).second;
		}
	}
	//LOG(INFO) << "end of iterating through map...";

	LOG(ERROR) << "There's no value with given name!";
	return NULL;
}

//Just parsing value to int, use only when you certainly know about var type!
int ConfigLoader::getIntVal(std::string key)
{
	return atoi((getVal(key)).c_str());
}

//Loading config from file, which passed as argument
void ConfigLoader::init(std::string path){
	if (FileManager::fileExists(path)){
		minIni ini(path);
		string s, section;
		for (int is = 0; section = ini.getsection(is), section.length() > 0; is++) {
			for (int ik = 0; s = ini.getkey(section, ik), s.length() > 0; ik++) {
				ConfigLoader::getInstance()._vals.insert(pair<string, string>(section + "-" + s, ini.gets(section, s)));
				LOG(INFO) << section + "-" + s + " " + ini.gets(section, s);
			}
		}
	}
	else
	{
		ConfigLoader::createDefault(path);
		ConfigLoader::init(path);
	}
}