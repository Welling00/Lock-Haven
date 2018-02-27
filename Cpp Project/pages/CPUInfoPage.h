/*
 * CPUInfoPage.h
 *
 *  Created on: Dec 13, 2017
 *      Author: ntt3013
 */
#ifndef CPU_INFO_PAGE_H
#define CPU_INFO_PAGE_H

#include "IPage.h"

namespace soc
{
	class CPUInfoPage : public IPage
	{
	public:
		CPUInfoPage();
		~CPUInfoPage();
		//for write info to page
		int write(std::shared_ptr<ClientSocket> client, const::std::string& buffer) override;
	};
};

#endif //CPU_INFO_PAGE_H
