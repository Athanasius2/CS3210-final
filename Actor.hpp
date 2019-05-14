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
		bool dead;

		
		void set_environment(const std::shared_ptr<Environment> env) { environment = env; };
	public:
		//TODO: If I ever rewrite this, start using this typedef
		typedef std::shared_ptr<Actor> s_ptr;

		Actor(char t, int nrg, std::pair<int, int> p) : type(t), max_energy(nrg), pos(p), dead(false) {};
		~Actor();

		//factory method to clear up some problems I was having with shared ptr
		//Turns out I didn't need to do this, but I spent a long time on it, so we're keeping it for now
		static std::shared_ptr<Actor> evaluate_species(std::string input, std::shared_ptr<Environment> env);
		//Actor(const Actor &a, std::pair<int, int> p) : Actor(a), pos{ p } {} //constructor for copying prototypes into map with new position
		virtual void act() = 0;
		virtual char print() = 0;
		
		virtual std::shared_ptr<Actor> clone_to(std::pair<int, int> p)  const = 0;

		bool is_dead() { return dead; }
		void die() { dead = true; }
		
		char gettype() { return type; }
		int get_max_energy() { return max_energy; }
		virtual int get_energy() = 0;
		
		void set_pos(std::pair<int, int> p) { pos = p; }
		std::pair<int, int> get_pos() { return pos; }

		const std::shared_ptr<Environment> &get_environment() const { return environment; }

		//Functions intended to be called by other Actors
		//returns true if actor can eat t
		virtual bool is_dangerous(char t) = 0;
		//Inquire about the edibility of Actor
		//virtual bool can_eat(char t) = 0;
		virtual bool is_eaten() = 0; //The Plants need this function
		//called by the eating Actor on the eaten Actor.  Returns current energy level.
		virtual int eat() = 0;

		//TODO: I don't think I need this
		bool is_adjacent(std::pair<int, int> p) { if ((abs(pos.first - p.first) <= 1) && (abs(pos.second - p.second) <= 1)) return true;  return false; }

		//make sure there is an adjacent empty space.  Make sure is_reproducing == false.
		virtual bool can_reproduce(char t) = 0;
		virtual bool is_reproducing() = 0;
	};
}
#endif