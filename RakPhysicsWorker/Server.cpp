#include "Server.h"
#include "easylogging++.h"
#include "NetworkListener.h"
#include "ConfigLoader.h"

#if !defined(_WIN64) && !defined(_WIN32)
#include <time.h>
#endif

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

void Server::removeClient(RakNet::RakNetGUID guid)
{
	for (map<RakNet::RakNetGUID, ConnectedClient>::iterator ii = this->_connections.begin(); ii != this->_connections.end(); ++ii)
	{
		if ((*ii).first == guid) {
			this->_connections.erase((*ii).first);
			return;
		}
	}
	LOG(INFO) << "There's no client with given guid!";
}


void Server::setPeer(RakNet::RakPeerInterface* i)
{
	_peer = i;
}

RakNet::RakPeerInterface* Server::getPeer(){
	return _peer;
};

NetworkListener* Server::getListener(){
	return _listener;
};

bool Server::initSecurity(const char* pub, const char* priv)
{
	if (this->_peer->InitializeSecurity(pub, priv, false))
	{
		_secure = true;
		return true;
	}
	else
	{
		_secure = false;
	}
	return false;

}

#if defined(_WIN64) || defined(_WIN32)

void Server::setThread(std::thread* trd)
{
	_networkTrd = trd;
};

std::thread* Server::getThread()
{
	return _networkTrd;
};

void Server::startMainNetworkThread(Server* instance, int port, int maxPlayers){
	RakNet::Packet *packet;

	RakNet::SocketDescriptor sd(port, 0);
	instance->getPeer()->Startup(maxPlayers, &sd, 1);
	LOG(INFO) << "Starting the server...";
	instance->getPeer()->SetMaximumIncomingConnections(maxPlayers);
	if (instance->_secure)
		LOG(INFO) << "Secure server has been started! Listening for conections...";
	else
		LOG(INFO) << "Server has been started! Listening for conections...";

	instance->setRunning(true);
	while (instance->getRunning())
	{
		Sleep(1);
		for (packet = instance->getPeer()->Receive(); packet; instance->getPeer()->DeallocatePacket(packet), packet = instance->getPeer()->Receive())
		{
			instance->getListener()->handle(packet);
		}
	}
	instance->getPeer()->Shutdown(10.0);
	RakNet::RakPeerInterface::DestroyInstance(instance->getPeer());
}

#else

void Server::setThread(pthread_t* trd)
{
	_networkTrd = trd;
};

pthread_t* Server::getThread()
{
	return _networkTrd;
};

void* Server::startMainNetworkThread(void* data){
	LOG(INFO) << "Starting the server...";
	
	server_data* serv_data = (struct server_data *)data;
	Server* instance = serv_data->instance;
	int port = serv_data->port;
	int maxPlayers = serv_data->max_players;
	
	//LOG(INFO) << "Data was read...";
	RakNet::RakPeerInterface* inst = RakNet::RakPeerInterface::GetInstance();
	instance->setPeer(inst);
	RakNet::Packet *packet;
    	//LOG(INFO) << "Instance assigned...";

	RakNet::SocketDescriptor sd(port, 0);
	instance->getPeer()->Startup(maxPlayers, &sd, 1);
	instance->getPeer()->SetMaximumIncomingConnections(maxPlayers);
        if (instance->_secure)
		LOG(INFO) << "Secure server has been started! Listening for conections...";
	else
		LOG(INFO) << "Server has been started! Listening for conections...";

	instance->setRunning(true);
	
	struct timespec req;
	req.tv_sec = 0;
	req.tv_nsec = 25000L;
	//LOG(INFO) << "Starting loop...";

	while (instance->getRunning())
	{
		nanosleep(&req, NULL);
		for (packet = instance->getPeer()->Receive(); packet; instance->getPeer()->DeallocatePacket(packet), packet = instance->getPeer()->Receive())
		{
			instance->getListener()->handle(packet);
		}
	}
	instance->getPeer()->Shutdown(10.0);
	RakNet::RakPeerInterface::DestroyInstance(instance->getPeer());
}

#endif

void Server::setRunning(bool r)
{
	_networkRunning = r;
}

bool Server::getRunning()
{
	return _networkRunning;
}

map<RakNet::RakNetGUID, ConnectedClient>* Server::getConnections()
{
	return &_connections;
}

Server::Server(NetworkListener * lis){
	this->_listener = lis;
	this->setPeer(RakNet::RakPeerInterface::GetInstance());

};