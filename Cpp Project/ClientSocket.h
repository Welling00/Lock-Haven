/*
 * ClientSocket.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "ISocket.h"
#include "Utils.h"
#include <netinet/in.h>

namespace soc
{
	class ClientSocket : public ISocket
	{
	public:

		ClientSocket();
		virtual ~ClientSocket();
		//methods for getting buffer
		int readBuffer(std::string& readBuffer);
		int writeBuffer(const std::string& writeBuffer);
		std::string getAddress();
		//close socket declare as void
		void closeSocket() override;

		friend class ServerSocket;

	private:
		////Define a 2^16 sized buffer for the user request
		//char m_buffer[65535];

		std::string m_buffer;
		// create the address structure to get the client address
		struct sockaddr_in ClientAddress;
	};
};

#endif //CLIENT_SOCKET_H
