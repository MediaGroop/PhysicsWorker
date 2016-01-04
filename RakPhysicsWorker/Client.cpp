#include "Client.h"
#include "easylogging++.h"

#if defined(_WIN64) || defined(_WIN32)
void Client::startNetworkTrd(Client* clnt, std::string h, int port)
{
	RakNet::Packet *packet;

	RakNet::SocketDescriptor sd;
	clnt->getPeer()->Startup(1, &sd, 1);
	clnt->getPeer()->Connect(h.c_str(), port, 0, 0);
	clnt->setRunning(true);
	while (clnt->getRunning())
	{
		Sleep(1);
		for (packet = clnt->getPeer()->Receive(); packet; clnt->getPeer()->DeallocatePacket(packet), packet = clnt->getPeer()->Receive())
		{
			clnt->getListener()->handle(packet);
		}
	}
	clnt->getPeer()->CloseConnection(*clnt->getServerAddr(), true);
	clnt->getPeer()->Shutdown(10.0);
	RakNet::RakPeerInterface::DestroyInstance(clnt->getPeer());
}

void Client::setThread(std::thread* trd)
{
	this->_networkTrd = trd;
};

std::thread* Client::getThread()
{
	return _networkTrd;
};
#else
void* Client::startNetworkTrd(void* data)
{
	
	client_data* clnt_data = (struct client_data *)data;

	Client* clnt = clnt_data->instance;
	const char* h = clnt_data->address;
	int port = clnt_data->port;

	RakNet::RakPeerInterface* inst = RakNet::RakPeerInterface::GetInstance();
	inst->AllowConnectionResponseIPMigration(false);

	RakNet::Packet *packet;
	RakNet::SocketDescriptor sd(port + 100, 0);
	inst->Startup(1, &sd, 1);
	inst->Connect(h, port, 0, 0);
	clnt->setPeer(inst);

	//LOG(INFO) << "[Client]SOcket was configured...";

	struct timespec req;
	req.tv_sec = 0;
	req.tv_nsec = 25000L;
	
	RakNet::RPC4 rpc;
	inst->AttachPlugin(&rpc);
	clnt->setRPC(&rpc);
	clnt->setRunning(true);
		
	while (clnt->getRunning())
	{
		nanosleep(&req, NULL);
		for (packet = clnt->getPeer()->Receive(); packet; clnt->getPeer()->DeallocatePacket(packet), packet = clnt->getPeer()->Receive())
		{
			clnt->getListener()->handle(packet);
		}
	}
	
	clnt->getPeer()->CloseConnection(*clnt->getServerAddr(), true);
	clnt->getPeer()->Shutdown(10.0);
	RakNet::RakPeerInterface::DestroyInstance(clnt->getPeer());
}

void Client::setThread(pthread_t* trd)
{
	this->_networkTrd = trd;
};

pthread_t* Client::getThread()
{
	return _networkTrd;
};
#endif

void Client::connect(const char* host, int port)
{
 #if defined(_WIN64) || defined(_WIN32)
	this->setThread(new std::thread(startNetworkTrd, this, host, port));
#else
	pthread_t* trd = new pthread_t();
	//LOG(INFO) << host;
	client_data data;
	data.instance = this;
	data.address = host;
	data.port = port;
	
	//LOG(INFO) << "[Client]starting thread...";
	pthread_create(trd, NULL, &this->startNetworkTrd, (void *)&data);
	this->setThread(trd);
#endif
}


bool Client::getRunning()
{
	return _running;
};

void Client::setRunning(bool r)
{
	this->_running = r;
};

void Client::setPeer(RakNet::RakPeerInterface* i)
{
	this->_peer = i;
};

void Client::setServerAddr(RakNet::SystemAddress a)
{
	this->_serverAddress = a;
};

RakNet::SystemAddress* Client::getServerAddr()
{
	return &_serverAddress;
};

RakNet::RakPeerInterface* Client::getPeer(){
	return this->_peer;
};

NetworkListener* Client::getListener(){
	return _listener;
};

Client::Client(NetworkListener * l){
	this->_listener = l;
};

RakNet::RPC4* Client::getRPC(){
	return _rpc;
};

void Client::setRPC(RakNet::RPC4* addr){
	_rpc = addr;
};

Client::~Client(){};

Client::Client() :Client(new NetworkListener()){};
