/*
 * Isocket.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef ISOCKET_H
#define ISOCKET_H

namespace soc {
class ISocket {
public:
	//declare to read the socket
	using sock_fd = int;
	using sock_port = int;

	ISocket() :
			m_sockfd(-1), m_port(-1) {
	}

	virtual ~ISocket() {
	}
	//get socket
	sock_fd getSocketFD() const {
		return m_sockfd;
	}
	//get the port
	sock_port getPort() const {
		return m_port;
	}
	//close socket set to equal 0
	virtual void closeSocket() = 0;

protected:
	sock_fd m_sockfd;
	sock_port m_port;
	};
};

#endif //ISOCKET_H
