/*
 * StatusPage.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#include "StatusPage.h"
#include "../Utils.h"
#include <stdbool.h>    //Needed for booleans
#include <sys/time.h>   //Needed for time related functions
#include <time.h>        //Needed for time functions
#include <sys/utsname.h>//Needed for uname information
#include <sys/sysinfo.h>//Needed for system information
#include <string.h>     //Needed for strings
#include <stdio.h>      //Needed for prints (I/O)
#include <stdlib.h>
#include <unistd.h>     //Needed for some POSIX functions
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>      //Needed for error numbers
#include <sys/stat.h>
#include <stdint.h>

soc::StatusPage::StatusPage() {
}

soc::StatusPage::~StatusPage() {
}

/***
 * This function controls what gets displayed to the browser when the client requests to see the status.
 */
int soc::StatusPage::write(std::shared_ptr<ClientSocket> client,
		const ::std::string& buffer) {
	int bytesWritten = 0;
	//Inform the user that the status is being written out via socket
	Utils::printToFile("Writing status out via socket...", false);

	//bytesWritten will contain the amount of bytes written out and is returned at the end of the function

	//Initial HTML code
	std::string initialHTML = "HTTP/1.1 200 OK\r\n"
			"Content-type: text/html\r\n"
			"\r\n"
			"<html>\r\n"
			" <head>"
			"  <title>Status Page</title>"
			"  <style>body { background-color: #0080ff; }</style>"
			" </head>"
			" <body>\r\n"
			"  <h1>System Status</h1>\r\n";

	//Define a time_t type named time
	time_t currTime;
	//time gets the current time
	currTime = time(&currTime);
	//timeBuffer will contain all the HTML code for the time
	char timeBuff[64] = "<table style=\"width:100%\"><tr><td>";
	//Concatenate the time (formatted by asctime) to the timeBuff string
	strcat(timeBuff, asctime(localtime(&currTime)));
	//Concatenate a closing cell tag
	strcat(timeBuff, "</td>");

	//ipAddrBuff will contain the HTML code for the IP address and port
	std::string ipAddrBuff;
	//Put the IP address passed to this function in the ipAddrBuff with table tags
	ipAddrBuff = "<td>" + client->getAddress() + "</td></tr></table><hr>";

	//unameBuff will contain all the user machine information
	std::string unameBuff;
	//Create a utsname structure called unameDatas
	struct utsname unameData;
	//Put the user machine information inside the unameData
	uname(&unameData);
	//Get all the elements of unameData and put them into the unameBuff string along with HTML tags
	unameBuff = "<h2>User Machine Information</h2>\r\n<ul>";
	unameBuff += "<li>OS: " + std::string(unameData.sysname) + "</li>";
	unameBuff += "<li>Node name: " + std::string(unameData.nodename) + "</li>";
	unameBuff += "<li>Release: " + std::string(unameData.release) + "</li>";
	unameBuff += "<li>Version: " + std::string(unameData.version) + "</li>";
	unameBuff += "<li>Machine: " + std::string(unameData.machine) + "</li>";
	unameBuff += "</ul>\r\n<hr>\r\n";

	//sysInfoBuff will contain all the user's system information
	std::string sysInfoBuff;
	//Create a sysinfo structure called sysinfoData
	struct sysinfo sysinfoData;
	//Put the system information inside the sysinfoData structure
	sysinfo(&sysinfoData);
	//Get all the elements of sysinfoData and put them into the sysInfoBuff string along with HTML tags
	sysInfoBuff = "<h2>System Information</h2>\r\n<ul>"
			"<li>Up-time: " + std::to_string(sysinfoData.uptime) + "</li>"
			"<li>Load Avg (1 min.): " + std::to_string(sysinfoData.loads[0])
			+ "</li>"
					"<li>Load Avg (5 min.): "
			+ std::to_string(sysinfoData.loads[1]) + "</li>"
					"<li>Load Avg (15 min.): "
			+ std::to_string(sysinfoData.loads[2]) + "</li>"
					"<li>Total RAM: " + std::to_string(sysinfoData.totalram)
			+ "</li>"
					"<li>Free RAM: " + std::to_string(sysinfoData.freeram)
			+ "</li>"
					"<li>Shared RAM: " + std::to_string(sysinfoData.sharedram)
			+ "</li>"
					"<li>Buffered RAM: " + std::to_string(sysinfoData.bufferram)
			+ "</li>"
					"<li>Total swap space: "
			+ std::to_string(sysinfoData.totalswap) + "</li>"
					"<li>Free swap space: "
			+ std::to_string(sysinfoData.freeswap) + "</li>"
					"<li>Current Processes: "
			+ std::to_string(sysinfoData.procs) + "</li>"
					"<li>Total high memory: "
			+ std::to_string(sysinfoData.totalhigh) + "</li>"
					"<li>Available high memory: "
			+ std::to_string(sysinfoData.freehigh) + "</li>"
					"<li>Memory unit size: "
			+ std::to_string(sysinfoData.mem_unit) + "</li>"
					"</ul>\r\n\n";

	//The ending HTML tags
	std::string endHTML = "  </body>\r\n"
			"</html>\r\n";

	/* write the messages */

	try {
		bytesWritten += client->writeBuffer(initialHTML);
		bytesWritten += client->writeBuffer(timeBuff);
		bytesWritten += client->writeBuffer(ipAddrBuff);
		bytesWritten += client->writeBuffer(unameBuff);
		bytesWritten += client->writeBuffer(sysInfoBuff);
		bytesWritten += client->writeBuffer(endHTML);
	} catch (const std::runtime_error& err) {
		throw err;
	}

	//Return the total bytesWritten
	return bytesWritten;
}

