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

		std::shared_ptr<Actor> clone_to(std::pair<int, int> p) const override 
		{
			auto pl = std::make_unique<Plant>(*this);
			pl->set_pos(p);
			return pl;
		}

		void act() override;
		char print() override;

		bool is_dangerous(char t) override { return false; }
		bool can_reproduce(char t) override { return false; }
		bool is_reproducing() override { return false; }
		int get_energy() override { if (eaten) return 0; return get_max_energy(); }
		int eat() override { eaten = true; return get_max_energy(); }
		bool is_eaten() override {	return eaten; }

		~Plant();
	};
}
#endif