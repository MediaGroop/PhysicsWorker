#pragma once
#include "stdafx.h"
#include "Server.h"
#include <map>
#include "World.h"

//#region vars
static Server* mainServer;
static std::map<int, World> worlds;
//#endregion