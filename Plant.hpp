#ifndef PLANT_HPP
#define PLANT_HPP

#include "Environment.hpp"
#include "Actor.hpp"

#include <utility>
#include <string>
#include <regex>
#include <memory>

namespace cppfinal
{
	class Plant : public Actor
	{

	private:
		int maxcooldown;
		int cooldown;
		bool eaten;

		
	public:
		Plant(char t, int c, int e, std::pair<int, int> p) : Actor{ t, e, p }, cooldown(c) {}; //TODO: make private so no one EVER uses this

		virtual std::shared_ptr<Actor> clone_to(std::pair<int, int> p) const override 
		{
			auto pl = std::make_unique<Plant>(*this);
			pl->set_pos(p);
			return pl;
		}

		void act() override;
		char print() override;


		~Plant();
	};
}
#endif