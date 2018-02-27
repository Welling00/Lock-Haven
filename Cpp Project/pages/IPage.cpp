/*
 * IPage.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ntt3013
 */
//include headers
#include "IPage.h"
#include "ErrorPage.h"
#include "DirectoryPage.h"
#include "StatusPage.h"
#include "CPUInfoPage.h"
//creating page type
soc::IPage* soc::IPage::createPage(PageType pageType) {
	//case for different page
	switch (pageType) {
	case PageType::StatusPageType: {
		return new StatusPage();
	}
		break;
	case PageType::DirectorypageType: {
		return new DirectoryPage();
	}
		break;
	case PageType::ErrorPageType: {
		return new ErrorPage();
	}
		break;
	case PageType::CPUInfoPageType: {
		return new CPUInfoPage();
	}
		break;
	default:
		return nullptr;
	};
}
