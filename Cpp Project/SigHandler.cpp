/*
 * SigHandler.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#include <stdbool.h>    //Needed for booleans
#include <signal.h>     //Needed for signals
#include <string.h>     //Needed for strings
#include <string>

#include "SigHandler.h"
#include "Utils.h"

#define _XOPEN_SOURCE   //Needed for later versions of C sigaction, etc. is not part of the ansi standard
int Exit = 0;
// signal handler
void myHandler(int signal) {
	//Set exit to stop the loop
	Exit = signal;

	//Put the handler exit message into a string
	std::string handlerExitMsg;
	handlerExitMsg = "myHandler caught signal: " + std::to_string(Exit)
			+ ". Exiting program...";
	soc::Utils::printToFile(handlerExitMsg, false);

	//Return finished
	return;
}

void setUpHandler() {
	Exit = 0;

	// declare sigaction structure
	//      struct sigaction is a structure that used by the sigaction
	//      function. It contains the information needed to change
	//      the signal handlers
	struct sigaction sa;

	// clear the sigaction structure
	//      without this, there may be problems
	//      depending on what is in the memory
	//      that is allocated for the structure
	memset(&sa, 0, sizeof(sa));

	// set up what to do
	//      sa_handler is the function to call
	//      sa_mask - leave it empty for now
	sa.sa_handler = myHandler;
	sigemptyset(&sa.sa_mask);

	// set the flags
	//      do nothing if you want the system calls to raise an error
	//      add this line to have the system call automatically restart
	// sa.sa_flags = SA_RESTART;

	// set the SIGINT handler
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		soc::Utils::printToFile("Error assigning the handler for SIGINT", true);
		throw std::runtime_error("Error assigning the handler for SIGINT");
		;
	}

	// set the SIGINT handler
	if (sigaction(SIGTERM, &sa, NULL) == -1) {
		soc::Utils::printToFile("Error assigning the handler for SIGTERM",
				true);
		throw std::runtime_error("Error assigning the handler for SIGTERM");
		;
	}

	//Inform the user that the handler was set up successfully
	soc::Utils::printToFile("Handler was set up successfully.", false);
}
