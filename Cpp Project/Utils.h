/*
 * Util.h
 *
 *  Created on: Nov 14, 2017
 *      Author: ntt3013
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>      //Needed for prints (I/O)
#include <string>
#include <memory>
#include <fstream>
#include <mutex>

//#include "ClientSocket.h"

namespace soc
{
	//using class from Ipage and ClientSocket
	class IPage;
	class ClientSocket;

	//	some error codes
	//		use these to make
	//		the code more readable

	class Utils
	{
	public:
		//methods for logs
		static void initLog(const std::string& logFileName);
		static void printToFile(const std::string& message, bool error);
		static IPage* checkRequest(std::shared_ptr<ClientSocket> client, const::std::string& buffer);

	private:
		//declare mutex and outfile
		static std::ofstream OutFile;
		static std::mutex logMutex;
		static std::mutex checkRequestMutex;
	};
};
#endif //UTILS_H
