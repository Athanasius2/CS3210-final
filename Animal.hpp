#ifndef ANIMAL_HPP
#define ANIMAL_HPP



#include "Actor.hpp"
#include "Environment.hpp"


#include <vector>
#include <memory>
#include <utility>

namespace cppfinal
{
	class Animal : public Actor
	{
	private:
		std::vector<char> eats;
		int energy;

		

	public:
		Animal(char t, int nrg, std::vector<char>  e, std::pair<int, int> p) : Actor{t, nrg, p }, energy{ nrg }, eats{ e } {};

		virtual std::shared_ptr<Actor> clone_to(std::pair<int, int> p) const override
		{
			auto an = std::make_unique<Animal>(*this);
			an->set_pos(p);
			return an;
		}
		
		void act() override;
		char print() override {return gettype();};

		~Animal();
	};
}
#endif
