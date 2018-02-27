/*
 * ErrorPage.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#include "ErrorPage.h"
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

soc::ErrorPage::ErrorPage() {
}

soc::ErrorPage::~ErrorPage() {
}

int soc::ErrorPage::write(std::shared_ptr<ClientSocket> client,
		const ::std::string& buffer) {
	int bytesWritten = 0;
	Utils::printToFile("Writing error out via socket...", false);
	//The entire error and HTML is included in this defined string
	std::string error404 =
			"HTTP/1.1 404 Not Found\r\n"
					"Content-type: text/html\r\n"
					"\r\n"
					"<html>\r\n"
					"     <head>"
					"        <title>ERROR 404</title>"
					"          <style>"
					"            body"
					"                { "
					"                background-color: #b42000; "
					"                text-align:center; "
					"            } "
					"        </style>"
					"     </head>"
					"     <body>"
					"          <h1>Not Found</h1>\r\n"
					"          <p>The requested URL was not found on this server.</p>\r\n"
					"     </body>\r\n"
					"</html>\r\n";

	//Return the bytes written after writing the error404 string to the browser
	int writeResult = 0;
	//try and catch result, throw  error if failure
	try {
		writeResult = client->writeBuffer(error404);
	} catch (const std::runtime_error& err) {
		throw err;
	}
	return writeResult;
}
