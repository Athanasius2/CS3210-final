#ifndef	ACTOR_HPP
#define ACTOR_HPP
#include <utility>
#include <memory>
#include <string>

namespace cppfinal
{	
	class Plant;
	class Animal;
	class Environment; //circular depency BAD

	class Actor
	{
	private:
		std::shared_ptr<Environment> environment;
		std::pair<int, int> pos; //X, Y coordinates
		char type;
		int max_energy;

		
		void set_environment(const std::shared_ptr<Environment> env) { environment = env; };
	public:
		Actor(char t, int nrg, std::pair<int, int> p) : type(t), max_energy(nrg), pos(p) {};
		//factory method to clear up some problems I was having with shared ptr
		//Turns out I didn't need to do this, but I spent a long time on it, so we're keeping it for now
		static std::shared_ptr<Actor> evaluate_species(std::string input, std::shared_ptr<Environment> env);
		//Actor(const Actor &a, std::pair<int, int> p) : Actor(a), pos{ p } {} //constructor for copying prototypes into map with new position
		virtual void act() = 0;
		virtual char print() = 0;
		
		virtual std::shared_ptr<Actor> clone_to(std::pair<int, int> p)  const = 0;
		
		char gettype() { return type; }
		void set_pos(std::pair<int, int> p) { pos = p; }
		std::pair<int, int> get_pos() { return pos; }
	};
}
#endif