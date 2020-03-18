#ifndef COMMON_HPP
#define COMMON_HPP

#include "3ds.h"
#include "csvc.h"
#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework
{
	using StringVector = std::vector<std::string>;
	
	struct Item
	{
		const char *name;
	};

	void Calculator(MenuEntry *entry);
	void LedFlah(MenuEntry* entry);
}

#endif