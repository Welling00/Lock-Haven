/*
 * IPage.h
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
#ifndef IPAGE_H
#define IPAGE_H

#include "../Utils.h"
#include "../ClientSocket.h"
#include <memory>

namespace soc
{
	class IPage
	{
	public:
		//declare different page type
		enum class PageType
		{
			StatusPageType = 0,
			DirectorypageType,
			ErrorPageType,
			CPUInfoPageType,
			Undefined
		};
		//create page
		static IPage* createPage(PageType pageType);
		//for writing to page, throw error if fail
		virtual int write(std::shared_ptr<ClientSocket> client, const::std::string& buffer)
		{
			throw std::runtime_error("Not Implemented");
		}

		IPage(){}
		virtual ~IPage()
		{

		}
	};
};

#endif //IPAGE_H
