/*
 * ServerSocket.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#include <sys/socket.h> //Needed for socket functions
#include <string.h>     //Needed for strings
#include <unistd.h>     //Needed for some POSIX functions
#include <errno.h>      //Needed for error numbers
#include <arpa/inet.h>  //Needed to get the IP address of the client
#include <signal.h>
#include <poll.h>
#include <fcntl.h>

#include <sstream>

#include "ServerSocket.h"
#include "Utils.h"
#include "SigHandler.h"

soc::ServerSocket::ServerSocket(sock_port port) {
	m_port = port;
}

soc::ServerSocket::~ServerSocket() {
	this->ServerSocket::closeSocket();
}

void soc::ServerSocket::setUpSocket() {
	std::stringstream message;

	//Create the socket file descriptor. socket() returns -1 if failed. Returns the file socket file
	//    descriptor and sets it to sockfd.
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//If sockfd is equal to -1, inform the user there was an error.
	if (sockfd == -1) {
		message << "Socket creation error - error: " << errno << " ["
				<< strerror(errno) << "]";
		Utils::printToFile(message.str(), true);
		throw std::runtime_error("Socket creation error");
		;
	}

	//Inform the user that the socket was created successfully
	message.str("");
	message << "Socket was created successfully. Using port " << m_port;
	Utils::printToFile(message.str(), false);

	/* set the address to the computer's */
	memset(&(m_sockAddress), 0, sizeof((m_sockAddress)));
	m_sockAddress.sin_family = AF_INET;
	m_sockAddress.sin_addr.s_addr = INADDR_ANY;
	m_sockAddress.sin_port = htons(m_port);

	//Bind the port and get the returned result
	int bindResult = bind(sockfd, (struct sockaddr *) &(m_sockAddress),
			sizeof(m_sockAddress));

	//If the bind function returned a -1 (error), inform the user
	if (bindResult == -1) {
		//Put the error message in a string
		message.str("");
		message << "Error in binding socket - error: " << errno << " ["
				<< strerror(errno) << "]";
		Utils::printToFile(message.str(), true);
		//Return out of the function with -1 (error)
		throw std::runtime_error("Error in binding socket");
		;
	}

	//Inform the user the socket was bound successfully
	Utils::printToFile("Socket binded successfully.", false);

	//Set the socket up for listening and get the returned result
	int listenResult = listen(sockfd, 5);

	//If the listen function returned a -1 (error), inform the user
	if (listenResult == -1) {
		//Put the error message in a string
		message.str("");
		message << "Wasn't able to set the socket up for listening - error: "
				<< errno << " [" << strerror(errno) << "]";
		Utils::printToFile(message.str(), true);
		//Return out of the function with a -1 (error)
		throw std::runtime_error(
				"Wasn't able to set the socket up for listening");
		;
	}

	//Print out that the socket is set up for listening
	Utils::printToFile("Socket is set up for listening.", false);

	//Return the socket file descriptor
	m_sockfd = sockfd;
}

void soc::ServerSocket::listenToClient() {
	if (m_sockfd < 0) {
		throw std::runtime_error("Socket not intialized");
		;
	}

	//Enter a loop which can only be exited by a signal
	while (Exit == 0) {
		try {
			acceptConnection();
		} catch (int err) {
			//Put the error message into a string
			std::string message;
			message = "Was unable to accept connection - error: "
					+ std::to_string(errno) + " ["
					+ std::string(strerror(errno)) + "]";
			soc::Utils::printToFile(message, true);
			throw err;
		}

		//If the Exit signal is no longer 0, break out of the while loop
		if (Exit != 0) {
			break;
		}
	}
	for (auto& t : m_clientsHandlingThreads) {
		t.join();
	}
}

void soc::ServerSocket::closeSocket() {
	std::stringstream message;
	if (close(m_sockfd) == -1) {
		//Put the error message into a string
		message << "Error in closing the socket - error: " << errno << " ["
				<< strerror(errno) << "]";
		Utils::printToFile(message.str(), true);
	} else {
		//Inform the user the socket closed without issue
		Utils::printToFile("Socket closed successfully.", false);
	}

}

const std::string&& soc::ServerSocket::getAddress() {
	return std::string(inet_ntoa(m_sockAddress.sin_addr));
}

void soc::ServerSocket::acceptConnection() {
	std::shared_ptr<ClientSocket> clientSoc(new ClientSocket);
	socklen_t ClientAddressLength = sizeof(clientSoc->ClientAddress);
	//	initialize the address
	memset(&(clientSoc->ClientAddress), 0, ClientAddressLength);

	/* call accept to get a connection */
	clientSoc->m_sockfd = accept(m_sockfd,
			(struct sockaddr *) &(clientSoc->ClientAddress),
			(unsigned int *) &ClientAddressLength);

	if (clientSoc->m_sockfd < 0) {
		throw std::runtime_error("Client socket not initialized");
		;
	} else {
		// read in the request one chunk at a time

		// first set the socket to non-blocking
		//	call fcntl to set the socket flags

		int Result = fcntl(clientSoc->m_sockfd, F_SETFL, O_NONBLOCK);
		if (Result == -1) {
			perror("fcntl"); // error occurred in fcntl()
			throw std::runtime_error("call fcntl failed");
			;
		}

		// use poll to wait for data to read
		// initialize the file descriptor list
		// there can be many int the list but
		// there is only one herr
		struct pollfd polllist[1];
		// set the socket to pool
		polllist[0].fd = clientSoc->m_sockfd;
		// POLLIN is event when data is
		// available to read
		polllist[0].events = POLLIN;

		// wait for data to read
		// poll can timeout and
		// return an error so you
		// will have to check for these
		Result = poll(polllist, 1, 10000);
		if (Result == -1) {
			perror("poll"); // error occurred in poll()
			throw std::runtime_error("poll faled");
			;
		} else if (Result == 0) {
			printf("Timeout occurred!  No data after 10 seconds.\n");
			throw std::runtime_error("poll time out");
			;
		}

		clientSoc->m_port = clientSoc->ClientAddress.sin_port;

		std::thread clientHandleThread(&ServerSocket::connectionHandler, this,
				clientSoc);
		m_clientsHandlingThreads.push_back(std::move(clientHandleThread));
	}
}

void soc::ServerSocket::start() {
	try {
		setUpSocket();
		listenToClient();
	} catch (const std::runtime_error& err) {
		throw err;
	}
}

void soc::ServerSocket::connectionHandler(
		std::shared_ptr<ClientSocket> client) {
	std::string ipmessage;
	ipmessage = client->getAddress() + " is connected.";
	soc::Utils::printToFile(ipmessage, false);

	std::string readBuffer;
	int readResult = 0;
	readResult = client->readBuffer(readBuffer);

	//If readResult is equal to -1, inform the user there was an error.
	if (readResult == -1) {
		//Put the error message into a string
		std::string message;
		message = "Error reading socket - error: " + std::to_string(errno)
				+ " [" + std::string(strerror(errno)) + "]";
		soc::Utils::printToFile(message, true);
		//throw std::runtime_error("Error reading socket");;
	} else {
		//Inform the user the server is reading from the socket
		soc::Utils::printToFile("Read from socket...", false);

		//Check if the request matches anything that can be performed
		soc::Utils::checkRequest(client, readBuffer);
		client->closeSocket();
	}
}
