/*
 * Util.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: ntt3013
 */
#include <stdbool.h>    //Needed for booleans
#include "pages/IPage.h"
#include "Utils.h"
#include <sys/time.h>   //Needed for time related functions
#include <time.h>        //Needed for time functions
#include <ctype.h>      //Needed for the use of the tolower() function
#include <string.h>     //Needed for strings
#include <dirent.h>
#include <ctime>
#include <iomanip>
#include "pages/DirectoryPage.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <clocale>

#define Debug            //Whether or not messages should be printed to the console depends on
//    if Debug is enabled

std::ofstream soc::Utils::OutFile;
std::mutex soc::Utils::logMutex;
std::mutex soc::Utils::checkRequestMutex;
//method for initializing log
void soc::Utils::initLog(const std::string& logFileName) {
	if (OutFile.is_open()) {
		OutFile.flush();
		OutFile.close();
	}
	OutFile.open(logFileName.c_str(), std::ofstream::out | std::ofstream::app);

	//If OutFile is null, print that there is an error and return 1
	if (!OutFile.is_open()) {
		std::cerr << "File unable to be opened. Exiting program...\n";
		throw std::runtime_error("File unable to be opened");
	}
}

/***
 *    This function controls writing to the file and printing to the console.
 */
void soc::Utils::printToFile(const std::string& message, bool error) {
	std::lock_guard < std::mutex > scopedLock(logMutex);
	std::stringstream ss;
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);
	ss << std::put_time(&tm, "%c %Z");
	std::string dateMsg = ss.str();

	//If it's an error message
	if (error == true) {
		//Print to stderr
		std::cerr << dateMsg << " | " << message << std::endl;
		////Flush the stderr buffer of the stream
		//fflush(stderr);
	} else		//If it's a normal message
	{
		//If Debug is enabled
#ifdef Debug
		//Print to stdout
		std::cout << dateMsg << " | " << message << std::endl;
		////Flush the stdout buffer of the stream
		//fflush(stdout);
#endif
	}

	//Print to the file
	OutFile << dateMsg << " | " << message << std::endl;
	//Flush the output buffer of the stream
	OutFile.flush();
}

/***
 * This function checks if the user's requested page exists, and calls the appropriate function if it does
 */
soc::IPage* soc::Utils::checkRequest(std::shared_ptr<ClientSocket> client,
		const ::std::string& buffer) {
	std::lock_guard < std::mutex > scopedLock(checkRequestMutex);
	IPage* createdPage = nullptr;
	//This string will hold the first 11 characters of the buffer
	std::string checkBuffer = "";
	std::locale loc;

	//Start on 4 because we can skip the "get". The purpose of the loop is to find when the
	//    request stops
	for (int i = 5; i < 1023; i++) {
		//Stop when the next characters are HTTP/
		if (buffer.substr(i + 1, 5) == std::string("HTTP/")) {
			//Break out of the loop
			break;
		} else {
			char onChar;

			//Copy the current character to the onChar string
			onChar = buffer[i];

			//Convert the first element in the onChar string to lowercase
			onChar = std::tolower(onChar);
			//Concatenate the first element of onChar to the checkBuffer string
			checkBuffer.append(1, onChar);
		}
	}

	int bytesWritten = 0;
	//Check if the user is requesting the status page
	if (checkBuffer == "status") {
		printToFile("Utils.c: Requested the status page.", false);
		createdPage = IPage::createPage(IPage::PageType::StatusPageType);
	}
	//Check if the user is requesting the directory
	else if ((checkBuffer == "dir/") || (checkBuffer == "dir")) {
		printToFile("Utils.c: Requested the directory page.", false);
		createdPage = IPage::createPage(IPage::PageType::DirectorypageType);
	} else if ((checkBuffer == "favicon.ico") || (checkBuffer == "")) {
		//Nothing. Ignore this for now
	} else if (checkBuffer == "cpuinfo") {
		printToFile("Utils.c: Requested the CPU Info page.", false);
		createdPage = IPage::createPage(IPage::PageType::CPUInfoPageType);
	} else //else throw the 404 error page
	{
		//Prepare the error message
		std::string requestErrorMsg = "";
		requestErrorMsg += "Utils.c: Request \"" + std::string(checkBuffer)
				+ "\" does not match any pages.";
		printToFile(requestErrorMsg, true);
		createdPage = IPage::createPage(IPage::PageType::ErrorPageType);
	}

	if (createdPage != nullptr) {
		bytesWritten = createdPage->write(client, buffer);
	}

	//Return out of the function
	return createdPage;
}
