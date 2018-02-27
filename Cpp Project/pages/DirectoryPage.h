/*
 * DirectoryPage.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef DIRECTORY_PAGE
#define DIRECTORY_PAGE

#include "IPage.h"

namespace soc {
class DirectoryPage: public IPage {
public:
	DirectoryPage();
	~DirectoryPage();
	//write info to page
	int write(std::shared_ptr<ClientSocket> client, const ::std::string& buffer)
			override;
	};
};

#endif //DIRECTORY_PAGE
