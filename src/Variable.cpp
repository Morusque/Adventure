
#include "Variable.h"

// TODO is there a way to make an "actual" C++ variable whose name is equal to the _name string ?
Variable::Variable(string _name)
{
    name = _name;
    value = 0;
}

Variable::~Variable()
{
    //dtor
}

void Variable::add(int _i) {
    value += _i;
}

void Variable::sub(int _i) {
    value -= _i;
}

void Variable::set(int _i) {
    value = _i;
}

