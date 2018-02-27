/*
 * main.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: ntt3013
 */
#include <string.h>     //Needed for strings
#include <stdlib.h>     //Needed for the use of the atio() function
#include <unistd.h>     //Needed for some POSIX functions
#include <errno.h>      //Needed for error numbers
#include <arpa/inet.h>  //Needed to get the IP address of the client
#include "Utils.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "SigHandler.h"
#include <iostream>

int main(int argc, char **argv) {
	//Disables stream buffering
	setvbuf(stdout, NULL, _IONBF, 0);

	//Open the file to be used for writing output
	try {
		soc::Utils::initLog("Output.txt");
	} catch (...) {
		std::cerr << "[ERROR][Log file could not open][Output.txt]";
		return 1;
	}

	//Call a function to set up the handler. throw std::runtime_error(""); if there was an error
	try {
		setUpHandler();
	} catch (std::runtime_error err) {
		soc::Utils::printToFile("Handler failed during set up.", true);
	} catch (...) {
		std::cerr << "Exception occured";
		return 1;
	}

	//Make sure there is an argument
	if (argc >= 3) {
		//Inform the user there was too many arguments
		soc::Utils::printToFile(
				"WARNING: More than one argument was inputed. All other arguments will be ignored.",
				false);
	} else if (argc <= 1) {
		//Inform the user there was no argument
		soc::Utils::printToFile("No arguments inputed. Exiting program...",
				true);
		return 1;
	}

	//Define variable portnum. This will be used to store the port number
	int portnum;

	//If the argument is 0
	if (strcmp(argv[1], "0") == 0) {
		//portnum is set to 0
		portnum = 0;
	} else {
		//portnum is set to whatever the first argument is after being converted to an integer
		portnum = atoi(argv[1]); //Returns 0 if argument is not an integer

		//Since we know that atoi returns 0 if the argument is not an integer...
		if (portnum == 0) {
			//...set portnum to -1
			portnum = -1;
		}
	}

	//If portnum is less than 0 or greater than 2^16
	if (portnum < 0 || portnum > 65535) {
		//Inform the user the port number is bad
		soc::Utils::printToFile("Invalid port number inputed as argument.",
				true);
		return 1;
	}

	soc::ServerSocket serverSocket(portnum);

	//Call a function to set up the socket and return the socket file descriptor
	try {
		serverSocket.start();
	} catch (const std::runtime_error& err) {
		std::cerr << err.what();
		return 1;
	} catch (...) {
		std::cerr << "Exception occured";
		return 1;
	}
	return 0;
}
