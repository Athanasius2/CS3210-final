#include "Plant.hpp"
#include <regex>

namespace cppfinal
{

	void Plant::act()
	{
		if (cooldown > 0)
		{
			cooldown--;
		}
		else if (eaten)
		{
			cooldown = maxcooldown;
		}
		else if (cooldown == 0 && eaten)
		{
			eaten = false;
		}
	}

	char Plant::print()
	{
		if (eaten)
			return ' ';
		else
			return gettype();
	}

	Plant::~Plant()
	{
	}
}