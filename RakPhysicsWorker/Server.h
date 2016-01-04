#pragma once
#include "RakPeerInterface.h"

#if defined(_WIN64) || defined(_WIN32)
#include <thread>
#else 
#include <pthread.h>
#endif

#include "ConnectedClient.h"
#include <map>
#include "NetworkListener.h"

using namespace std;

class Server
{
private:
	RakNet::RakPeerInterface *_peer;
	NetworkListener* _listener;

#if defined(_WIN64) || defined(_WIN32)
	std::thread * _networkTrd;
#else
	pthread_t * _networkTrd;
#endif
	
	bool _networkRunning = false;

	map<RakNet::RakNetGUID, ConnectedClient> _connections;
public:
	bool _secure;

	void setPeer(RakNet::RakPeerInterface* i);

	RakNet::RakPeerInterface* getPeer();

	NetworkListener* getListener();

#if defined(_WIN64) || defined(_WIN32)
	void setThread(std::thread * trd);
	std::thread* getThread();
	static void startMainNetworkThread(Server*, int, int);
#else
	void setThread(pthread_t * trd);
	pthread_t* getThread();
	static void* startMainNetworkThread(void* data);
#endif
	void setRunning(bool r);

	bool getRunning();

	map<RakNet::RakNetGUID, ConnectedClient>* getConnections();

	Server(NetworkListener * lis);

	~Server(){
	  this->setPeer(RakNet::RakPeerInterface::GetInstance());
	};
	
        void addClient(RakNet::RakNetGUID, ConnectedClient);
    
	bool hasClient(RakNet::RakNetGUID);
    
	ConnectedClient* getClient(RakNet::RakNetGUID);

	//Another ghost function
	bool initSecurity(const char*, const char*);
	
	void removeClient(RakNet::RakNetGUID);

	map<RakNet::RakNetGUID, ConnectedClient>* get_connections(){

		return &_connections;

	}
	
};

#if !defined(_WIN64) && !defined(_WIN32)
struct server_data{
   Server* instance;
   int  port;
   int  max_players;
};
#endif
