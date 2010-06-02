// cond_act.hpp

#ifndef COND_ACT_HPP
#define COND_ACT_HPP 1

// from Prolog, etc.
template<typename Guard,typename Action>
class conditional_action 
{
private:
	Guard _guard;
	Action _action;
public:
	conditional_action(Guard guard, Action action) : _guard(guard),_action(action) {};
	template<class T> void operator()(T& x) {if (_guard(x)) _action(x);} 
};

#endif
