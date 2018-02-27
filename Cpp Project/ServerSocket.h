/*
 * ServerSocket.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "ClientSocket.h"
#include "Utils.h"
#include <memory>
#include <vector>
#include <thread>

namespace soc
{
	class ServerSocket : public ISocket
	{
	public:
		ServerSocket(sock_port port);
		virtual ~ServerSocket();
		//methods for sockets
		void setUpSocket();
		void listenToClient();
		void closeSocket() override;
		const std::string && getAddress();
		void acceptConnection();

		void start();
		//connection handler for socket
		void connectionHandler(std::shared_ptr<ClientSocket>);

	private:
		//std::shared_ptr<ClientSocket> m_clientSoc;

		/* declare address variable */
		struct sockaddr_in m_sockAddress;
		std::vector<std::thread> m_clientsHandlingThreads;
	};
};

#endif //SERVER_SOCKET_H
