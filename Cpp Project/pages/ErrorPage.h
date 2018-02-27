/*
 * ErrorPage.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef ERROR_PAGE_H
#define ERROR_PAGE_H

#include "IPage.h"

namespace soc {
class ErrorPage: public IPage {
public:
	ErrorPage();
	~ErrorPage();
	//for writing into page
	int write(std::shared_ptr<ClientSocket> client, const ::std::string& buffer)
			override;
	};
};

#endif //ERROR_PAGE_H
