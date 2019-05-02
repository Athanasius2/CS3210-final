#ifndef ANIMAL_HPP
#define ANIMAL_HPP



#include "Actor.hpp"
#include "Environment.hpp"


#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <algorithm>

namespace cppfinal
{
	class Animal : public Actor
	{
	private:
		std::vector<char> eats;

		//TODO: move energy to actor
		int energy;

		enum State {REPRODUCE, HUNGRY, HUNTED};

		State state;	//Default state = REPRODUCE
		bool reproducing;

		void move(Environment::Direction d);
		bool can_move(Environment::Direction d);
		

	public:
		Animal(char t, int nrg, std::vector<char>  e, std::pair<int, int> p) : Actor{ t, nrg, p }, energy{ nrg }, eats{ e }, reproducing{false} {}


		std::shared_ptr<Actor> clone_to(std::pair<int, int> p) const override
		{
			auto an = std::make_unique<Animal>(*this);
			an->set_pos(p);
			return an;
		}
		
		

		void act() override;
		char print() override {return gettype();}

		//TODO: move energy to actor.
		int get_energy() override { return energy; }

		bool is_eaten() override { return is_dead(); } //not totally logical, but it works.
		bool is_reproducing() override { return reproducing; }
		bool can_reproduce(char t) override;
		int eat() override { die(); return energy; }

		bool is_dangerous(char t) override { return std::any_of(eats.begin(), eats.end(), [t](char c) { return (c == t); }); }


		~Animal();
	};
}
#endif
