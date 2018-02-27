/*
 * DirectoryPage.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#include "DirectoryPage.h"
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

soc::DirectoryPage::DirectoryPage() {
}

soc::DirectoryPage::~DirectoryPage() {
}

/**
 *
 * Writes the data of DirectoryPage to the buffer and returns bytesWritten
 *
 */
int soc::DirectoryPage::write(std::shared_ptr<ClientSocket> client,
		const ::std::string& buffer) {
	int bytesWritten = 0;
	//Inform the user that the status is being written out via socket
	Utils::printToFile("Pages.c: Writing directory out via socket...", false);
	int portnum = client->getPort();

	//bytesWritten will contain the amount of bytes written out and is returned at the end of the function

	//Initial HTML code
	//the html meta data
	std::string initialHTML =
			"HTTP/1.1 200 OK\r\n"
					"Content-type: text/html\r\n"
					"\r\n"
					"<html>\r\n"
					" <head>\r\n"
					"  <title>Directory</title>\r\n"
					"  <style>"
					"        table, th, td {border: 1px solid black; border-collapse: collapse;}"
					"    </style>\r\n"
					" </head>\r\n"
					" <body>\r\n"
					"  <h1>Directory Page</h1>\r\n";

	//Create an array for the directory
	std::string directory = "";

	//Start on 8 because we can skip the "get dir/". The purpose of the loop is to find when the
	//    directory requested stops
	for (int i = 8; i < 1023; i++) {
		//Stop when the next characters are HTTP/
		if (buffer.substr(i + 1, 5) == std::string("HTTP/")) {
			//Break out of the loop
			break;
		}
		//Look for the space unicode, remove the unicode, and replace it with a space
		else if (buffer.substr(i, 3) == std::string("%20")) {
			//Replace with a space
			directory += " ";
			//Increase past the unicode
			i += 2;
			//Go to the start of the loop
			continue;
		}

		//Add the current character
		directory.append(1, static_cast<char>(buffer.c_str() + 8 + (i - 8), 1));
	}

	//Make a utsname structure
	struct utsname unameData;
	//Put the user machine information inside the unameData
	if (uname(&unameData) == -1) {
		int errsv = errno;
		std::string message = "Error occured:" + std::string(strerror(errsv))
				+ " function:uname err code:" + std::to_string(errsv);
		Utils::printToFile(message, true);
		return 0;
	}

	//Because Windows uses \ instead of /, compatibility for Windows is not intentionally supported yet
	std::string unsupportedOSMsg = "";

	//If the OS is not Linux
	if (strcmp(unameData.sysname, "Linux") != 0) {
		//Warn the user that their operating system is not fully supported
		unsupportedOSMsg = "Pages.c: " + std::string(unameData.sysname)
				+ " is not fully supported yet. Errors may occur.";
		Utils::printToFile(unsupportedOSMsg, false);

		unsupportedOSMsg += "<p>" + std::string(unameData.sysname)
				+ " is not fully supported yet. Errors may occur.<p>";
	}

	//A string to hold the address path
	std::string addrPath;
	//Put the base directory into the addrPath variable
	addrPath = directory;

	//If the user didn't put anything in for the request, default them to home
	if ((addrPath == "") || (addrPath == "/")) {
		//Make the user aware of what is happening
		Utils::printToFile(
				"Pages.c: No request specified. Redirecting to Home...", false);
		//Make /home the address path
		addrPath = "/home";
	}

	//Print out what the directory that was requested is
	std::string dirMsg;
	dirMsg = "Pages.c: Directory requested: " + directory;
	Utils::printToFile(dirMsg, false);

	//currDir gets printed to the screen, letting the user know which index they're on
	std::string currDir = "";
	//tableStart contains the beginning html tags for the table
	std::string tableStart = "";
	//tableBody contains all the table elements to be printed to the screen
	std::string tableBody = "";
	//tableEnd contains contains the html closing tags for the table
	std::string tableEnd = "";

	//Declare a dirent structure to get the names of the files in the directory
	struct dirent **namelist;

	//scanDir returns the number of files in the current directory
	int numOfFiles = scandir(addrPath.c_str(), &namelist, NULL, alphasort);

	//If number of files is -1, that directory could not be found
	if (numOfFiles == -1) {
		//Prepare the error message and put it in the tableBody
		tableBody = "    <error style=\"color:red; text-align:center;\">\r\n"
				"        <h2>The directory \"" + directory
				+ "\" is not found.</h2>\r\n"
						"    </error>\r\n";

		//Print to the file (and console if Debug is enabled) that the directory was invalid
		std::string dirErrMsg;
		dirErrMsg = "Pages.c: Invalid directory - error: "
				+ std::to_string(errno) + " [" + std::string(strerror(errno))
				+ "]";
		Utils::printToFile(dirErrMsg, true);
	} else	//Directory could be found
	{
		//Declare a structure for the stats
		struct stat statbuff;
		//Declare a structure for time
		struct tm *tm;

		//currDir gets what the index is currently
		currDir = "    <h2>Index of " + directory + "</h2>\r\n<hr>\r\n";

		//Put in the table tags
		tableStart = "    <table style=\"width:50%\">\r\n"
				"        <tr>\r\n"
				"            <th>File Name</th>\r\n"
				"            <th>Size</th>\r\n"
				"            <th>Last Modified</th>\r\n"
				"        </tr>\r\n";

		//Iterate through a loop for as many of the number of files there are
		for (int i = 1; i < numOfFiles; i++) {
			//Create the full links
			std::string addrLink = "";
			addrLink = addrPath + "/" + std::string(namelist[i]->d_name);

			/* Get entry's information. */
			if (stat(addrLink.c_str(), &statbuff) == -1) {
				std::string statErrMsg;
				statErrMsg = "Pages.c: Error acquiring stats - error: "
						+ std::to_string(errno) + " ["
						+ std::string(strerror(errno)) + "]";
				Utils::printToFile(statErrMsg, true);
			}

			//This string gets the size of the file at the current iteration
			char sizeStr[32];
			sprintf(sizeStr, "%9jd", (intmax_t) statbuff.st_size);

			//This string gets the last time modified of the file at the current iteration
			char time[24];
			tm = localtime(&statbuff.st_mtime);
			strftime(time, 23, "%m/%d/%y %I:%M:%S %p", tm);

			//rowBuff contains table
			char rowBuff[2048] = "";

			//If it is a directory
			if (S_ISDIR(statbuff.st_mode)) {
				//If the current iteration is the double dot, it needs a different behavior
				if (strcmp(namelist[i]->d_name, "..") == 0) {
					//A new string for the address for the double dots
					char addrWDots[512];

					//A for loop that will decrement after starting on the string length of the
					//	original address path string
					for (int i = addrPath.length(); i > 0; i--) {
						//When it finds the first slash (going in reverse order)...
						if (strncmp(&addrPath[i], "/", 1) == 0) {
							//This memset clears the string addrWDots
							memset(addrWDots, 0, strlen(addrWDots));
							//addrWDots in the destination, addrPath is the source. Copy i amount of characters
							//	from addrPath to addrWDots
							strncpy(addrWDots, addrPath.c_str(), i);
							//Break out of the loop
							break;
						}
					}

					//rowBuff gets the table entry for the two dots (the only difference from the normal one is
					//	the hyperlink is now the addrWDots string)
					sprintf(rowBuff,
							"<tr>\r\n"
									"    <td><a href=\"localhost:%d/dir%s\">%s</a></td>\r\n"
									"    <td>  ---</td>\r\n"
									"    <td>%s</td>\r\n"
									"</tr>\r\n", portnum, addrWDots,
							namelist[i]->d_name, time);
				} else					//Not two dots (normal table entry)
				{
					//rowBuff gets the table entry pointing to the current iteration of the namelist (which was already
					//	appended to addrLink)
					sprintf(rowBuff,
							"<tr>\r\n"
									"    <td><a href=\"localhost:%d/dir%s\">%s</a></td>\r\n"
									"    <td>  ---</td>\r\n"
									"    <td>%s</td>\r\n"
									"</tr>\r\n", portnum, addrLink,
							namelist[i]->d_name, time);
				}
			} else					//Else it is not a directory
			{
				//rowBuff gets a table entry but without a hyperlink because it is a file
				sprintf(rowBuff, "<tr>\r\n"
						"    <td>%s</td>\r\n"
						"    <td>%s</td>\r\n"
						"    <td>%s</td>\r\n"
						"</tr>\r\n", namelist[i]->d_name, sizeStr, time);
			}

			//Concatenate the rowBuff to the tableBody string
			tableBody += std::string(rowBuff);
			//Go to the next iteration of the loop
		}

		//The table end tags
		tableEnd = "</table>\r\n";
	}

	//The ending HTML tags
	std::string endHTML = "  </body>\r\n"
			"</html>\r\n";

	/* write the messages */
	int writeResult = 0;
	try {
		bytesWritten += client->writeBuffer(initialHTML);
		bytesWritten += client->writeBuffer(unsupportedOSMsg);
		bytesWritten += client->writeBuffer(currDir);
		bytesWritten += client->writeBuffer(tableStart);
		bytesWritten += client->writeBuffer(tableBody);
		bytesWritten += client->writeBuffer(tableEnd);
		bytesWritten += client->writeBuffer(endHTML);
	} catch (const std::runtime_error& err) {
		throw err;
	}

	return writeResult;
}
