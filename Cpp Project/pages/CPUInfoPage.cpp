/*
 * CPUInfoPage.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: ntt3013
 */
#include "CPUInfoPage.h"
#include "PageStyle.h"
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
#include <fstream>
#include <iostream>

soc::CPUInfoPage::CPUInfoPage()
{
}

soc::CPUInfoPage::~CPUInfoPage()
{
}

/**
 * Writes the cpu info page to the buffer and returns the bytesWritten
 */
int soc::CPUInfoPage::write(std::shared_ptr<ClientSocket> client, const::std::string & buffer)
{
	int bytesWritten = 0;

	Utils::printToFile("Writing CPU info out via socket...", false);

	//Header html metadata
	std::string initialHTML =
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"<html>\r\n"
		" <head>"
		"  <title>CPU Info Page</title>"
		"<meta name=\"viewport\" content=\"width = device - width\">"
		"  <style>body " + s_cssStyle + "</style>"
		" </head>"
		" <body>\r\n"
		"  <div class=\"container\">"
		"	<table class=\"responsive-table\">\n"
		;
	//Create a utsname structure called unameDatas
	struct utsname unameData;
	//Put the user machine information inside the unameData
	uname(&unameData);

	//Table content
	std::string tableContent = "<caption>CPU Info - Node name: " + std::string(unameData.nodename) + "</caption>\n";
	tableContent +=
		"<thead>"
		"<tr>"
		"<th scope = \"col\">Parameter</th>\n"
		"<th scope = \"col\">Value</th>\n"
		"</tr>\n"
		"</thead>\n";

	//records the current time
	time_t currTime;
	currTime = time(&currTime);

	tableContent +=
		"<tfoot>\n"
		"<tr>\n"
		"<td colspan = \"7\">Time: " + std::string(asctime(localtime(&currTime))) + "</td>\n"
		"</tr>\n"
		"</tfoot>\n";

	std::string tableBody = "<tbody>\n";

	//open the cpuinfo file
	std::ifstream cpuInfoFile;
	cpuInfoFile.open("/proc/cpuinfo");

	//Get all the elements of cpuinfo and put them into the sysInfoBuff string along with HTML tags
	if (cpuInfoFile.is_open() == false)
	{

	}
	else
	{
		std::string readLine;
		while (cpuInfoFile.good())
		{
			getline(cpuInfoFile, readLine);
			size_t splitPos = readLine.find_first_of(':');
			if (splitPos != std::string::npos)
			{
				std::string text_1 = "";
				std::string text_2 = "";
				size_t lastPos = readLine.find_last_not_of(" \t\f\v\n\r", splitPos - 1);
				if(lastPos != std::string::npos)
					text_1 = readLine.substr(0, lastPos + 1);

				size_t firstPos = readLine.find_first_not_of(" \t\f\v\n\r", splitPos + 1);
				if (firstPos != std::string::npos)
					text_2 = readLine.substr(firstPos, readLine.length());

				std::cout << text_1 << std::endl;
				tableBody +=
					"<tr>\n"
					"<th scope = \"row\"> " + text_1 + " </th>\n"
					"<td data - title = \"Released\"> " + text_2 + "</td>\n"
				"</tr>\n";
			}
		}
	}

	tableBody += "</tbody>\n";
	//The ending HTML tags
	std::string endHTML =
		"	  </table>\n"
		"	</div>\n"
		"  </body>\r\n"
		"</html>\r\n";
	/* write the messages */

	bytesWritten += client->writeBuffer(initialHTML);
	bytesWritten += client->writeBuffer(tableContent);
	bytesWritten += client->writeBuffer(tableBody);
	bytesWritten += client->writeBuffer(endHTML);


	cpuInfoFile.close();
	return bytesWritten;
}
