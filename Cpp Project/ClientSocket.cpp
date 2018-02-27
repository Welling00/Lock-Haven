/*
 * ClientSocket.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#include "ClientSocket.h"
#include "Utils.h"
#include <sstream>
#include <sys/socket.h> //Needed for socket functions
#include <string.h>     //Needed for strings
#include <unistd.h>     //Needed for some POSIX functions
#include <errno.h>      //Needed for error numbers
#include <arpa/inet.h>  //Needed to get the IP address of the client
#include <signal.h>
#include <poll.h>
#include <fcntl.h>

soc::ClientSocket::ClientSocket() {
}

soc::ClientSocket::~ClientSocket() {
	this->ClientSocket::closeSocket();
}

int soc::ClientSocket::readBuffer(std::string& readBuffert) {
	int readResult = 0;
	m_buffer = "";

	// loop until there is an error
	while (true) {
		char Buffer[256];
		memset(Buffer, 0, sizeof(Buffer));

		//Read the request to the buffer. Returns -1 if failed. Sets readResult to whatever read() returns.
		int rslt = read(m_sockfd, Buffer, sizeof(Buffer) - 1);

		if (rslt == -1) {
			break;
		} else {
			readResult += rslt;
		}
		m_buffer += std::string(Buffer);
	}
	//Inform the user the server is reading from the socket
	Utils::printToFile("Read from socket...", false);
	readBuffert = m_buffer;
	return readResult;
}

int soc::ClientSocket::writeBuffer(const std::string& writeBuffer) {
	int writeResult = 0;
	if (m_sockfd < 0) {
		throw std::runtime_error("Socket is not initialized");
	}
	writeResult = write(m_sockfd, writeBuffer.c_str(), writeBuffer.length());
	return writeResult;
}

std::string soc::ClientSocket::getAddress() {
	return std::string(inet_ntoa(ClientAddress.sin_addr));
}

void soc::ClientSocket::closeSocket() {
	std::stringstream message;
	if (close(m_sockfd) == -1) {
		//Put the error message into a string
		message << "Error in closing the socket - error: " << errno << " ["
				<< strerror(errno) << "]";
		Utils::printToFile(message.str(), true);
	}
	//Inform the user the socket closed without issue
	Utils::printToFile("Socket closed successfully.", false);
}
