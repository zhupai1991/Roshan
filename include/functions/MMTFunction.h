
#pragma once

#include "Function.h"

class MMTFunction : public Function
{
public:
	MMTFunction(const std::string & name, InputParameters parameters);
    virtual Real value(Real t, const Point & p);
};

template<> InputParameters validParams<MMTFunction>();
