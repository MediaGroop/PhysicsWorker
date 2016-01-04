#pragma once
#include "stdafx.h"
#include "NetworkListener.h"
#include "RPC4Plugin.h"

#if defined(_WIN64) || defined(_WIN32)
#include <thread>
#else
#include <pthread.h>
#endif

class Client
{
private:
	NetworkListener* _listener;

	RakNet::SystemAddress _serverAddress;

	bool _running = false;

	RakNet::RakPeerInterface * _peer;

	RakNet::RPC4* _rpc;
	
#if defined(_WIN64) && defined(_WIN32)
	std::thread* _networkTrd;
	static void startNetworkTrd(Client*, std::string, int);
#else
	pthread_t* _networkTrd;
	static void* startNetworkTrd(void* data);
#endif



public:

	void setRPC(RakNet::RPC4* addr);
  
	RakNet::RPC4* getRPC();
  
	bool getRunning();

	void setRunning(bool r);

	void setPeer(RakNet::RakPeerInterface* i);

	void setServerAddr(RakNet::SystemAddress a);
	
#if defined(_WIN64) && defined(_WIN32)
	void setThread(std::thread* trd);
	std::thread* getThread();
#else
	void setThread(pthread_t* trd);
	pthread_t* getThread();
#endif

	RakNet::SystemAddress* getServerAddr();

	RakNet::RakPeerInterface* getPeer();

	NetworkListener* getListener();

	Client(NetworkListener * l);

	~Client();

	Client();

	void connect(const char* host, int port);

};

#if !defined(_WIN64) && !defined(_WIN32)
struct client_data{
   Client*      	instance;
   const char*  	address;
   int          	port;
};
#endif
