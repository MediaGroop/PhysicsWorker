#pragma once
#include "stdafx.h"
#include "Server.h"
#include <map>
#include "World.h"

//#region vars
extern Server* mainServer;
extern std::map<int, World*> worlds;
//#endregion