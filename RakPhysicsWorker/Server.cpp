#include "Server.h"
#include "NetworkListener.h"
#include "easylogging++.h"
#include "ConfigLoader.h"

bool Server::hasClient(RakNet::RakNetGUID guid)
{
	if (getClient(guid) == nullptr)
		return false;
	return true;
}

ConnectedClient* Server::getClient(RakNet::RakNetGUID guid){
	for (map<RakNet::RakNetGUID, ConnectedClient>::iterator ii = this->_connections.begin(); ii != this->_connections.end(); ++ii)
	{
		if ((*ii).first == guid) {
			return &(*ii).second;
		}
	}
	return NULL;
}

void Server::addClient(RakNet::RakNetGUID guid, ConnectedClient cl)
{
	this->_connections.insert(pair<RakNet::RakNetGUID, ConnectedClient>(guid, cl));
}

void Server::startNetworkTrd(Server* instance, int port, int maxPlayers)
{
	RakNet::Packet *packet;

	RakNet::SocketDescriptor sd(port, 0);
	instance->peer->Startup(maxPlayers, &sd, 1);
	instance->peer->SetMaximumIncomingConnections(maxPlayers);
	
	if (instance->secure)
		LOG(INFO) << "Secure server has been started! Listening for conections...";
	else
		LOG(INFO) << "Server has been started! Listening for conections...";
	instance->running = true;
	while (instance->running)
	{
		Sleep(1);
		for (packet = instance->peer->Receive(); packet; instance->peer->DeallocatePacket(packet), packet = instance->peer->Receive())
		{
			instance->listener->handle(packet);
		}
	}

	instance->peer->Shutdown(5);
	RakNet::RakPeerInterface::DestroyInstance(instance->peer);
}

bool Server::initSecurity(const char* pub, const char* priv)
{
	if (this->peer->InitializeSecurity(pub, priv, false))
	{
		secure = true;
		return true;
	}
	else
	{
		secure = false;
	}
	return false;
}

void Server::removeClient(RakNet::RakNetGUID guid)
{
//	LOG(INFO) << _connections.size();
	for (map<RakNet::RakNetGUID, ConnectedClient>::iterator ii = this->_connections.begin(); ii != this->_connections.end(); ++ii)
	{
//		LOG(INFO) << "1 - ";
//		LOG(INFO) << (*ii).first.ToString();
//		LOG(INFO) << "2 - ";
//		LOG(INFO) << guid.ToString();
		if ((*ii).first == guid) {
			this->_connections.erase((*ii).first);
			return;
		}
	}
	LOG(INFO) << "There's no client with given guid!";
}
