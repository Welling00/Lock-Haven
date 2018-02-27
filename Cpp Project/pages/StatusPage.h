/*
 * StatusPage.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef STATUS_PAGE_H
#define STATUS_PAGE_H

#include "IPage.h"

namespace soc {
class StatusPage: public IPage {
public:
	StatusPage();
	~StatusPage();
	//declare to write to page
	int write(std::shared_ptr<ClientSocket> client, const ::std::string& buffer)
			override;
	};
};

#endif //STATUS_PAGE_H
