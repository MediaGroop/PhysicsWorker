#pragma once
#include "Server.h"
#include <map>
#include "World.h"

extern Server* mainServer;
extern std::map<int, World*> worlds;
